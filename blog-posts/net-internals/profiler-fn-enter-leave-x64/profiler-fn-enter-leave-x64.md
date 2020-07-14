---
published: true
title: "'function enter/leave' + .NET profiler + 64Bit. Assembler code included!"
description: "This is the 64Bit v ersion of: Trace 'function enter/leave' events with a .NET profiler + detect StackOverflow. Assembler code included!"
tags: dotnet, cpp, asm, tutorial
series: Net-Profiler
canonical_url:
---

>**Note:** Get the full running example [here](https://github.com/gabbersepp/dev.to-posts/tree/master/blog-posts/net-internals/profiler-fn-enter-leave-x64/code/DevToNetProfiler)

While beeing able to use inline assembler within a 32 bit project, you can not use that within a 64bit build. Microsoft just does not support this. You have to use an external `.asm` file instead which can be processed by `MASM` during the build process.

# Assembler in VS
To activate `MASM` support in a `C++` project in `VS2019`, please follow the guidelines in the [official documentation](https://docs.microsoft.com/de-de/cpp/assembler/masm/masm-for-x64-ml64-exe?view=vs-2019). Additionally I suggest you to install the VS extension [AsmDude](https://marketplace.visualstudio.com/items?itemName=Henk-JanLebbink.AsmDude) to get syntax highlighting.

# Switching between both implementations
To access the procedures that are defined in the assembler file, you must declare them with `extern "C"` in the header file. Otherwise the compiler rewrites the function names and thus the linker won't be able to match the CPP and ASM output together.

To see that effect, add a new function in a header file and call it somewhere in your CPP code. Then open the `obj` file and search the method name.

```cpp
extern bool DevToTest(int a, int b);
```

Without the `"C"` addition, the name does not match the original one:

![](./assets/devtotest_extern.jpg)

Using `"C"` fixes this:

```cpp
extern "C" bool DevToTest(int a, int b);
```

![](./assets/devtotest_externc.jpg)


To differentiate between 32 and 64 bit code, you can use preprocessor directives. Adjust `Naked32Bit.h` as following:

```cpp
#pragma once
#include "pch.h"

#ifdef _WIN64

EXTERN_C void InitEnterLeaveCallbacks(bool* activate, int* hashMap, int size);

EXTERN_C void FnEnterCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);

EXTERN_C void FnLeaveCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);

EXTERN_C void FnTailcallCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func);

#else

void InitEnterLeaveCallbacks(bool* activate, int* hashMap, int size);
//....

#endif
```

In case of a 64 bit build, the functions refer to external symbols. I also adjusted the signature of the `Init` function. This was necessary because I wanted to show you how you can build the same logic as in the inline assembler. But this requires a hashmap. To avoid allocating memory in assembler, I just pass the variables from `CPP` into the assembler code. This saves me some time and makes the whole thing more readable. 

*Note*: Of course the naming of the header file is not correct anymore, but this does not matter :smile:

Now adjust `Naked32bit.cpp`:

```cpp
extern "C" void _stdcall StackOverflowDetected(FunctionID funcId, int count) {
  std::cout << "stackoverflow: " << funcId << ", count: " << count;
}

extern "C" void _stdcall EnterCpp(
  FunctionID funcId,
  int identifier) {
  std::cout << "enter funcion id: " << funcId << ", Arguments in correct order: " << (identifier == 12345) << "\r\n";
}

#ifdef _WIN64

#else

bool* activateCallbacks;
int* pHashMap;
int mapSize;

void InitEnterLeaveCallbacks(bool* activate, int* hashMap, int size) {
  activateCallbacks = activate;
  pHashMap = hashMap;
  mapSize = size;
}
```

Both functions, `EnterCpp` and `SODetected` must be marked with `extern "C"`. The `Init` function and the variables must be moved into the 32bit code block. You can leave the 64bit code block empty because everything will be in the assembler file.

Now add the initialize in `ProfilerCOncreteImpl.cpp`:

```cpp
  this->PHashMap = new int[mapSize];
  memset(this->PHashMap, 0, mapSize);
  InitEnterLeaveCallbacks(&this->ActivateCallbacks, this->PHashMap, mapSize);
```

# The ASM Code
What you will see now is no magic. There is only one thing you have to pay attention for: In 64Bt builds there is only one calling convention: `fastcall`. See the links at the end of the post to get an insight into it. The most important points (at least these are the points I came across a few times):
- parameters are passed from left to right in the register: `RCX, RDX, R8, R9`
- The caller must reserve **4*8** bytes in case of the callee wants to store the parameters onto the stack
- The caller has to clean up the stack afterwards

I stumbled a few times over the last two points which led to unwanted behavior. 

```
_DATA SEGMENT
  pActivateEnterLeaveCallback qword 0
  pHashMap qword 0
  mapSize dword 0
_DATA ENDS

extern EnterCpp:proc
extern StackOverflowDetected:proc

_TEXT	SEGMENT

PUBLIC InitEnterLeaveCallbacks

InitEnterLeaveCallbacks PROC
  mov pActivateEnterLeaveCallback, RCX
  mov pHashMap, RDX
  mov mapSize, R8D
  ret
InitEnterLeaveCallbacks ENDP

PUBLIC FnEnterCallback

FnEnterCallback PROC
  mov RAX, pActivateEnterLeaveCallback
  cmp byte ptr [RAX], 1
  JNE skipCallback

  mov R8, pHashMap
  MOV RAX, RCX
  XOR RDX, RDX
  DIV DWORD PTR [mapSize]
  ADD R8, RDX
  INC DWORD PTR [R8]
  CMP DWORD PTR [R8], 30
  JB skipStackOverflow

  xor rdx, rdx
  MOV EDX, [R8]
  SUB RSP, 20h
  CALL StackOverflowDetected
  ADD RSP, 20h

  skipStackOverflow:

  sub RSP, 20h
  mov rdx, 12345
  CALL EnterCpp
  add RSP, 20h

  skipCallback:
  ret
FnEnterCallback ENDP

PUBLIC FnLeaveCallback

FnLeaveCallback PROC
  MOV RAX, pActivateEnterLeaveCallback
  CMP BYTE PTR [RAX], 1
  JNE skipCallback

  MOV R8, pHashMap
  MOV RAX, RCX
  XOR RDX, RDX
  DIV DWORD PTR [mapSize]
  ADD R8, RDX
  DEC DWORD PTR [R8]

  skipCallback:
  ret
FnLeaveCallback ENDP

PUBLIC FnTailcallCallback

FnTailcallCallback PROC
  ret
FnTailcallCallback ENDP

_TEXT	ENDS

END
```

You see, nothing new here. `sub RSP, 20h` and `add RSP, 20h` are used to reserve memory on the stack and clean it up afterwards.

# Using CPP implementations
As it seems that the CLR uses `fastcall` convention for calling the callbacks, you may assume that you can use CPP implementations instead of writing assembler code. Indeed I was able to do this:

```cpp
#ifdef _WIN64
bool* activateCallbacks;
int* pHashMap;
int mapSize;


void InitEnterLeaveCallbacks(bool* activate, int* hashMap, int size) {
  activateCallbacks = activate;
  pHashMap = hashMap;
  mapSize = size;
}

void __fastcall FnEnterCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  if (activateCallbacks) {
    int amount = pHashMap[funcId % mapSize];
    amount++;
    pHashMap[funcId % mapSize] = amount;

    if (amount >= 30) {
      StackOverflowDetected(funcId, amount);
    }
    EnterCpp(funcId, 12345);
  }
}

void __fastcall FnLeaveCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  if (activateCallbacks) {
    pHashMap[funcId % mapSize] = pHashMap[funcId % mapSize] - 1;
  }
}

void __fastcall FnTailcallCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func) {
}
#else
```

During testing the code I don't see any errors but I don't know if this approach is intended by Microsoft.

# Conclusion
The differences between 32 and 63 bit is not so big. I think the most relevant thing is the calling convention.

# Additional Links
[Configure project in VS to enable MASM](https://docs.microsoft.com/de-de/cpp/assembler/masm/masm-for-x64-ml64-exe?view=vs-2019)
[Use correct #define for x86/x64](https://stackoverflow.com/a/8672915/9809950)
[Impact of fastcall to stack consumption](https://www.viva64.com/en/b/0069/)
[Unwind code macros](https://docs.microsoft.com/de-de/cpp/build/exception-handling-x64?view=vs-2019)
[Stack usage on x64](https://docs.microsoft.com/de-de/cpp/build/stack-usage?view=vs-2019)
[Another link about stack frames](https://www.tortall.net/projects/yasm/manual/html/objfmt-win64-exception.html)
[X64 ASM code for the profiler](http://read.pudn.com/downloads64/sourcecode/windows/system/228104/leave_x64.asm__.htm)
[Example about unwind information](https://docs.microsoft.com/de-de/cpp/assembler/masm/proc?view=vs-2019)
[Explanation of fast call asm code](https://stackoverflow.com/questions/44269811/understanding-fastcall-stack-frame)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 