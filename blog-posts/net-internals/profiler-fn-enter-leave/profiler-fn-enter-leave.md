---
published: false
title: "Trace 'function enter/leave' events with a .NET profiler"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/net-internals/profiler-fn-enter-leave/assets/header.jpg"
description: "Using the 'function enter/leave' event can"
tags: dotnet, cpp, asm, tutorial
series:
canonical_url:
---

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

# Asm Code

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

Was ist der Sinn von `ret 16`? Nun, sowohl die Enter- als auch der Leavecallback bekommen vier Parameter übergeben. Die Übergabe erfolgt im Stack. Da es keinen Epilog gibt, müssen wir uns selber um das Aufräumen des Stacks kümmern, andernfalls wäre der Stack beim Aufrufer nach der Ausf+hrung der Funktion kaputt. Da jeder Parameter vier Bytes grioß ist, müssen wir 4*4 = 16 Bytes vom Stack entfernen. Das geht mit `ret` ganz einfach. Die Zahl dahinter gibt an, um wieviel Bytes der STackpointer verschoben werden soll.+


# Additional Links
[Official example about how to write Enter/Leave callbacks](https://github.com/Microsoft/clr-samples/blob/master/ProfilingAPI/ELTProfiler/CorProfiler.cpp#L27)
[Another example for Enter/Leave](https://searchcode.com/codesearch/view/777153/)
[Additional ASM Code from MS for X64](https://github.com/microsoftarchive/clrprofiler/blob/master/CLRProfiler/profilerOBJ/amd64/asmhelpers.asm)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
