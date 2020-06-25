---
published: false
title: "Trace 'function enter/leave' events with a .NET profiler"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/net-internals/profiler-fn-enter-leave/assets/header.jpg"
description: "Using the 'function enter/leave' event can"
tags: dotnet, cpp, asm, tutorial
series:
canonical_url:
---

>**Note:** Get the full running example here: <TODO>

The last time I showed some of the callbacks from `ICorProfilerCallback` and how you can obtain more information about the event. This time we want to take a look at the `Function Enter/Leave` callbacks.

# Refactoring
As usual I took the project from the last post in this series. You may notice that I have changed the structure of the files. I moved all of the `virtual` dummy functions, that exists only because of the implementation of the `ICorProfilerCallback2` interface, into an own class. This helps us to keep an overview about what we really want to focus on. Also I made a small fix in the project settings to ensure that both, x32 and x64 builds, are put into the same output directory. This was made to keep the `start.bat` as simple as possible.

# COR_PRF_MONITOR_ENTERLEAVE          
Today we are looking at the `COR_PRF_MONITOR_ENTERLEAVE` option which makes it possible to get notified if a function is entered and left. The callbacks are not declared on the `ICorProfilerCallback` interface but must be [registered](https://docs.microsoft.com/de-de/dotnet/framework/unmanaged-api/profiling/icorprofilerinfo2-setenterleavefunctionhooks2-method) on the `ICorProfilerInfo` object. Use `ICorProfilerInfo.SetEnterLeaveFunctionHooks2` for this task. Please note that the call to this method must occur during `Initialize()`, otherwise it is not valid. Also the callbacks are a bit special in the way you have to implement them :smile:

# Now Assembler comes into play
If we look into the documentation of [FunctionEnter2](https://docs.microsoft.com/de-de/dotnet/framework/unmanaged-api/profiling/functionenter2-function), we can read a inconspicuous paragraph that tells us that:

![](assets/fnenter2-paragraph.jpg)

`naked` advices the compiler to neither insert function prologue nor the epilogue at machine code level. The `prologue` consists of a few lines of code that prepares the CPU registers and the stack for the use within the function while the `epilogue` is the counterpart that restores the stack and registers before the function is left. 

--    
Das wurde so gebaut, damit man den Code mit inline Assembler schreiben kann, um so nur soviel Opcodes wie nötig zu erzeugen. Benötigt man z.b. nur das Register EAX, reicht es, auch nur dieses auf dem Stack zu sichern. Mit den anderen Aufrufkonventionen wäre dies so nicht möglich. Durch die Reduzierung auf den minimalsten Code soll verhindert werden, dass die Performance zu stark darunter leidet.

Wer nun bei `inline Assembler` sofort dran denken muss, dass C++ für die X64 platform kein inline Assembler anbietet, dem sei gesagt, dass wir das noch betrachten werden. Im Folgenden werden wir uns zuerst auf X32 fokussieren :smile:

Doch wie muss dieser Inline Assembler Code aussehen? Natürlich könnte man sich das alles selbst überlegen, oder man schaut in ein [offizielles MS Beispiel](https://github.com/Microsoft/clr-samples/blob/master/ProfilingAPI/ELTProfiler/CorProfiler.cpp#L27) und holt sich Anregungen :smile:

Um die Übersicht zu wahren, habe ich eine neue Datei - `naked32Bit.cpp` -  angelegt, welche die Callbacks beinhaltet.

# Base Assembler Code

Das Grundgerüst sieht folgendermaßen aus:

```cpp
void __declspec(naked) FnEnterCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    ret 16
  }
}

void __declspec(naked) FnLeaveCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    ret 16
  }
}

void __declspec(naked) FnTailcallCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func) {
  __asm {
    ret 12
  }
}
```

>**Note:** Die einzelnen Parameter können in der Doku nachgelesen werden. Den `TailCallCallback` werde ich links liegen lassen, da dieser, nach allem was ich gelesen habe, auch nicht genutzt wird.  

Was ist der Sinn von `ret 16`? Nun, sowohl die Enter- als auch der Leavecallback bekommen vier Parameter übergeben. Die Übergabe erfolgt im Stack. Da es keinen Epilog gibt, der Aufrufer aber erwartet, dass wir den Stack aufräumen, müssen wir die übergebenen Argumente vom Stack nehmen, andernfalls wäre der Stack beim Aufrufer nach der Ausführung der Funktion kaputt. Da jeder Parameter vier Bytes grioß ist, müssen wir 4*4 = 16 Bytes vom Stack entfernen. Das geht mit `ret` ganz einfach. Die Zahl dahinter gibt an, um wieviel Bytes der STackpointer verschoben werden soll.+

## Accessing the callback's arguments
Funktionsargumente werden vor dem Aufruf auf den Stack gepusht, wobei der letzte Parameter in der Defnition zuerst gepusht wird. Beim Aufruf von `CALL` wird noch die Adrsse des nächsten Opcodes gepusht. In der Funktion angekommen, zeigt der Stackpointer somit auf den nächsten auszuührenden Befehl. Wir wollen das auf die Schnelle verifizieren und schreiben ein kleines Konsolenprogramm:

```cpp
#include<iostream>

__declspec(naked) void __stdcall Test(int input, int* output) {
    __asm {
        push EAX
        push EBX
        mov EAX, [ESP + 12] ;input
        mov EBX, [ESP + 16] ;output
        mov [EBX], EAX
        pop EBX
        pop EAX
        ret 12
    }
}

int main()
{
    int output = 0;
    Test(100, &output);
    std::cout << output;
}
```

Zu beachten ist für diesen Test, dass die Funktion mit der Calling Convention `stdcall` versehen wird, um genau das Verhalten zu simulieren, welches wir später im Profiler haben werden. Dort nämlich muss sich der aufgerufene um den Stack kümmern, was im wesentlichen der **stdcall** Konvention entspricht. Da wir die Funktion selber im Code aufrufen, müssen wir dem Compiler sagen, dass wir **stdcall** nutzen wollen.

**Wieso wird das erste Arguent mit [ESP+12] geladen?** Nun, `ESP` zeigt auf die R+cksprungadresse. Dann folgen zwei `PUSH`Befehle. Um also zum ersten Argument zu kommen, müssen wir `ESP` um 3*4 Bytes nach oben schieben.

Übrigens könnte man auch genauso die Namen der Funktionsparameter nutzen:

```cpp
#include<iostream>

__declspec(naked) void __stdcall Test(int input, int* output) {
    __asm {
        push EBP
        mov EBP, ESP
        push EAX
        push EBX

        mov EAX, input
        mov EBX, output
        mov [EBX], EAX

        pop EBX
        pop EAX
        pop EBP
        ret 8
    }
}

int main()
{
    int output = 0;
    Test(100, &output);
    std::cout << output;
}
```

Das funktioniert, weil der COmpiler die Funktionsargumente mit den erwarteten Positionen im Stack übersetzt:

```asm
_TEXT	SEGMENT
_input$ = 8						; size = 4
_output$ = 12						; size = 4
?Test@@YGXHPAH@Z PROC					; Test, COMDAT
; 4    :     __asm {
; 5    :         push EBP
  00000	55		 push	 ebp
; 6    :         mov EBP, ESP
  00001	8b ec		 mov	 ebp, esp
; 7    :         push EAX
  00003	50		 push	 eax
; 8    :         push EBX
  00004	53		 push	 ebx
; 9    : 
; 10   :         mov EAX, input
  00005	8b 45 08	 mov	 eax, DWORD PTR _input$[ebp]
```

In Zeile zwei und drei wird die Position im Stack definiert. Wieso der Compiler automatisch bei 8 startet statt bei 4, liegt vermutlich daran, dass er ein `PUSH EBP` am Anfang der Funktion vermutet. Ich hab das aber nicht weiter recherchiert.

## A very simple approach to reduce the ASM code to as few lines as possible
Die einfachste Variante dieser Callbacks könnte einfach eine in C++ geschriebene Funktion aufrufen. Das einzige, was dann beachtet werden muss, ist die verwendete Callingconvention:

```cpp
void _stdcall EnterCpp(
  FunctionID funcId,
  int identifier) {
  std::cout << "enter funcion id: " << funcId << ", Arguments in correct order: " << (identifier == 12345) << "\r\n";
}

void __declspec(naked) FnEnterCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    ; push last parameter first!
    push 12345
    push [ESP+8]
    call EnterCpp
    ret 16
  }
}
```

## Use more ASM code
I also want to show you an example that makes "heavy" use of Assembler code. Angenommen man möchte in einem profiler das Loggen von Enter/Leave zur zeitweise aktivieren. Da die Callbacks bereits während der Initialisierung angegebene werden müssen, sind diese immer "aktiv", d.h. werden immer aufgerufen. Um nun per Flag das verarbeiten des Aufrufs zu steuern, könnte man ASM code schreiben, welcher ein Flag überprüft.

First we have to introduce a flag. This code should be in the same file where the Assembler code is:

```cpp
bool* activateCallbacks;

void InitEnterLeaveCallbacks(bool* activate) {
  activateCallbacks = activate;
}
```

Then call this function in the `Initialize()`:

```cpp
bool activateCallbacks = false;

HRESULT __stdcall ProfilerConcreteImpl::Initialize(IUnknown* pICorProfilerInfoUnk)
{
  //...
  InitEnterLeaveCallbacks(&activateCallbacks);
  //...
}
```

Was nun folgt ist simpler Assemblercode:

```cpp
void __declspec(naked) FnEnterCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    push ebx
    mov ebx, [activateCallbacks]
    cmp byte ptr [ebx], 1
    JNE skipCallback

    ; push last parameter first!
    push 12345
    push [ESP+12]
    call EnterCpp

    skipCallback:
    pop ebx
    ret 16
  }
}
```

Bitte beachten: Durch die verwendung von `EBX` als Zwischenspeicher, musste ich dies im Stack sichern und somit den ESP um vier weitere bytes nach oben schieben, um den `FunctionID` Parameter zu erhalten.

## Stackoverflow detection
Was könnte man nun damit noch anstellen? Nun, in .NEt Framework ist die Stackoverflow Exception der Tod einer Applikation. Aus meiner erfahrung heraus ist es auch nur manchmalk so, dass man einen Crashdump erhält. Was also tun? Man könnte mit der Kombination FunctionEnter/leave eine Art Erkennung entwickeln.

Dazu legen wir zuerst mal ein Integer array an. Dieses dient als "HashMap", welche eine FunctionID auf einen Counter mappt:

```cpp
bool* activateCallbacks;
int* hashMap;
const int mapSize = 10000;

void InitEnterLeaveCallbacks(bool* activate) {
  activateCallbacks = activate;
  hashMap = new int[mapSize];
  memset(hashMap, 0, mapSize);
}
```

Den Code zur behandlung des Stackoverflows würde ich mit CPP entwickeln, da man dort vermutlich irgendwelche Interaktionen machen muss, welche mit ASM zu aufwendig wären:

```cpp
void _stdcall StackOverflowDetected(FunctionID funcId, int count) {
  std::cout << "stackoverflow: " << funcId << ", count: " << count;
}
```

Den obigen Code, welcher das Flag prüft, erweitern wir adäquat:

```cpp
void __declspec(naked) FnEnterCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    push ebx
    mov ebx, [activateCallbacks]
    cmp byte ptr[ebx], 1
    JNE skipCallback

    ; check stackoverflow
    mov ebx, [hashMap]
    mov eax, [ESP + 8]
    xor edx, edx
    div dword ptr [mapSize]
    add ebx, edx
    inc dword ptr [ebx]
    cmp dword ptr [ebx], 30
    jb skipStackOverflow

    push [ebx]
    push [ESP + 8]
    CALL StackOverflowDetected

    skipStackOverflow:

    ; push last parameter first!
    push 12345
    push [ESP+12]
    call EnterCpp

    skipCallback:    

    pop ebx
    ret 16
  }
}
```
Ich denke, der Code sollte soweit klar sein. Mittels einer Modulooperation berechnen wir den Hash der `FunctionID` und können so die Aufruftiefe mittracken. Allerdings fehlt noch das Aufräumen im Falle eins Returns der Funktion. Schließlich könnte man die Funktion auch in einer Schleife zig mal aufrufen, was hier fälschlicherweise auch einen SO erkennen würde:

```cpp
void __declspec(naked) FnLeaveCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    push ebx
    mov ebx, [activateCallbacks]
    cmp byte ptr[ebx], 1
    JNE skipCallback

    mov ebx, [hashMap]
    mov eax, [ESP + 8]
    xor edx, edx
    div dword ptr [mapSize]
    add ebx, edx
    dec dword ptr [ebx]

    skipCallback:

    pop ebx
    ret 16
  }
}
```

Hier passiert quasi das gleiche, nur dass der Vergleich wegfällt und `dec` statt `inc` benutzt wird.

# Summary
I showed you how you can use the Enter/Leave callbacks on a x86 platform. In the next article we are going to extend this to 64 bit. This differs a bit because there is no inline asembler support for 64 bit platforms. So stay tuned!

# Additional Links
[Official example about how to write Enter/Leave callbacks](https://github.com/Microsoft/clr-samples/blob/master/ProfilingAPI/ELTProfiler/CorProfiler.cpp#L27)
[Another example for Enter/Leave](https://searchcode.com/codesearch/view/777153/)
[Additional ASM Code from MS for X64](https://github.com/microsoftarchive/clrprofiler/blob/master/CLRProfiler/profilerOBJ/amd64/asmhelpers.asm)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
