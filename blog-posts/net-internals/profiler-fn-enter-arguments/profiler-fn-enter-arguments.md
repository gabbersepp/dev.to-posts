---
published: true
title: "Read function arguments from .NET applications with .NET ProfilingAPI"
description: "I show you how you can extend your profiler in order to read function parameters within a FunctionEnter event"
tags: dotnet, cpp, csharp, tutorial
series: Net-Profiler
canonical_url:
---

>**Get the full runnable example:** [here](https://github.com/gabbersepp/dev.to-posts/tree/master/blog-posts/net-internals/profiler-fn-enter-arguments/code/DevToNetProfiler)

Today I want to guide you through the process of getting function parameters and return values. As always I am using the code from the last blogpost and extend it where necessary. For this article we have to adjust `EnterCpp` and the part of assembler code that calls this function.

While writing this lines I had no idea how this can be achieved. Of course I had the documentation about the [FunctionEnter2](https://docs.microsoft.com/de-de/dotnet/framework/unmanaged-api/profiling/functionenter2-function) callback and it's parameter `COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo`. But how I had to use it was not described. I also found no example in the world wide web but luckily I found someone who tried the same and was asking for help in the official [dotnet repo](https://github.com/dotnet/docs/issues/6728). He doesn't get a complete working example but only a short explanation how he can get what he wants. This was enough for me to figure out the remaining stuff.

# Modifying the Assembler code
Before we can start writing cool C++ code we must adjust the function signature of `EnterCpp`. When looking at the raw ICorProfiler definition of the enter callback, we can identify one additional argument that we should pass along with the `FunctionId`, named `argumentInfo`:

```cpp
void FnEnterCallback(FunctionID funcId, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO argumentInfo)
```

So let's change the signature of `EnterCpp`:

```cpp
void EnterCpp(FunctionID funcId, COR_PRF_FUNCTION_ARGUMENT_INFO * argumentInfo)
```

In case of **64 bit** assembler `argumentInfo` is the fourth parameter from left and thus is located in the register `R9`. To make it available to `EnterCpp` just copy it to `RDX`: 
```
MOV RDX, R9
```

In case of **32 bit** assembler, we have to push the correct value from the stack:

```
push [ESP+20]
```

Feel free to take a look into the code to see the full assembler code.

# Necessary steps if we know the types

For this first try I assume that we know the types of the parameter and I know that the functions will have only one parameter. Using those preconditions makes it easier to get a first runnable example. 

You need these steps:
+ Skip all functions whose name does not match our predefined set of functions
+ get a pointer to the function parameter
+ skip object header (if exists)
+ process data according to it's type

In the end this is not too hard to build. But as always, I needed a lot of try&error to get it right.

# Skip functions

Well, this is easy. Just get the function name and compare it against our known function names. Utilize the `Utils` class for that:

```cpp
char* fnName = new char[100];
utils->GetFunctionNameById(funcId, fnName, 100);

if (strcmp(fnName, "FunctionWithParameter") == 0) {
    //... do something
}
```

# Get Pointer to function arguments
From what I have understand, the `argumentInfo` points to an struct that describes memory blocks with parameters. The reason for this is that parameters are not accessible in an continuous memory block but are split up for technical reasons. 
- `argumentInfo->numRanges` is the amount of such blocks
- `argumentInfo->ranges` is an array of data

As we just have one parameter, this is not something we care about. So let's focus on `argumentInfo->ranges[0].startAddress`.

```cpp
COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[0];
UINT_PTR valuePtr = range.startAddress;
```

So what is `valuePtr`? I haven't found a documentation about that but from what I have seen by debugging and testing, `valuePtr` is:
+ **value type like 'int'**: a pointer to the value
+ **object**: a pointer to the [Method Table pointer](https://devblogs.microsoft.com/premier-developer/managed-object-internals-part-1-layout/)
+ **Struct:** a pointer to the struct

# Skip object header
[This article](https://devblogs.microsoft.com/premier-developer/managed-object-internals-part-1-layout/) states that an object points to the `Method Table Pointer` and not to the beginning of the object data. Right now we don't need the information contained in the header and thus can skip it. **Attention:** Take care of the correct pointer size on 32 bit systems vs 64 bit systems. Of course you don't have to care about it if the parameter's type is not of type `Object`.

# Process data
How can we interpret the data? Well, this depends on the data type. This raises the question, how we know the internal representation of the data? There are at least two possibilities to get an answer to this question. One is to read books and articles from Microsoft or other people about that topic. Another approach, which is sufficient in many times, is to use a .NET project + Visual Studio + Debugger to inspect the memory.

To do this we first need some C# code:

```cs
static void Main(string[] args)
{
    IntArrayFn(new int[] { 1,2,3,4,5,6,7,8,9,10});
}

static void IntArrayFn(int[] intArray)
{
    // place a breakpoint here and leave the method empty
}
```

Now execute the project as 64 bit application. This will produce function calls following the `fastcall` convention. That means that the parameters of `IntArrayFn` are passed from left to right in the registers `RCX, RDX, R8, R9`. This gives us the possibility to retrieve the memory address of the argument by inspecting the registers. Run the application and as soon as the breakpoint is hit, open **Debugging > Window > Register** to display the registers:

![](./assets/register.jpg)

Copy the value from `RCX` *(000002015C922F40)* into **Debugging > Window > Memory**:

![](./assets/memory.jpg)

The first marked area is the pointer to the `Method Table`. The next 8 bytes represent the length of the array and after that you see the array elements with 4 bytes in size each.

# Examples
Now I show you some examples. In every example I am using this code in `EnterCpp`:

```cpp
extern "C" void _stdcall EnterCpp(
  FunctionID funcId,
  COR_PRF_FUNCTION_ARGUMENT_INFO * argumentInfo) {

  char* fnName = new char[100];
  utils->GetFunctionNameById(funcId, fnName, 100);
```

And this C# code in the test application:

```cs
using System;

namespace TestApp
{
  class Program
  {
    static void Main(string[] args)
    {
      Console.ReadLine();

      StructFn(new TestStruct { Int1 = 101, Int2 = 102, Int3 = 103 });
      IntArrayFn(new[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
      StringFn("Hello from C#");
      IntFn(1234);

      Console.ReadLine();
    }

    static void IntArrayFn(int[] intArray)
    {
    }
    static void StringFn(string str)
    {
    }

    static void IntFn(int i1)
    {
    }

    static void StructFn(TestStruct t)
    {
    }
  }

  struct TestStruct
  {
    public int Int1;
    public int Int2;
    public int Int3;
  }
}
```

## IntFn
This is very easy. An integer is a value type and as such I can access it directly:

```cpp
if (strcmp(fnName, "IntFn") == 0) {
    COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[0];
    UINT_PTR valuePtr = range.startAddress;
    int* ptr = (int*)valuePtr;

    std::cout << "\r\n\r\n## Entered IntFn ##\r\n";
    std::cout << "argument: " << *ptr << "\r\n";
    std::cout << "## IntFn ends ##";
}
```

## StructFn
A struct also is a value type and thus can be handled like an integer. All fields of the struct are in memory one after the other:

```cpp
if (strcmp(fnName, "StructFn") == 0) {
    COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[0];
    UINT_PTR valuePtr = range.startAddress;
    int* ptr = (int*)valuePtr;
    std::cout << "\r\n\r\n## Entered StructFn() ##\r\n";
    std::cout << "size of range: " << range.length << "\r\n";
    std::cout << "arguments: Int1 = " << *ptr << ", Int2 = " << *(ptr + 1) << ", Int3 = " << *(ptr + 2) << "\r\n";
    std::cout << "## StructFn() ends ##";
}
``` 

`range.length` is **12**, because the struct consists of three fields where evry field needs four bytes.

## IntArrayFn
Now we see the handling of the object header.

```cpp
if (strcmp(fnName, "IntArrayFn") == 0) {
    COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[0];
    UINT_PTR valuePtr = range.startAddress;

    int** ptr = (int**)valuePtr;
    int* intArray = *ptr;

    std::cout << "\r\n\r\n## Enter IntArrayFn ##\r\n";
    intArray = intArray + sizeof(int*)/sizeof(int); //skip object header of array
    long arrayLength = *(long*)intArray;
    std::cout << "Length: " << arrayLength << "\r\n";
    intArray += sizeof(int*) / sizeof(int);
    std::cout << "Arguments: ";

    for (int i = 0; i < arrayLength; i++)
    {
      std:cout << "," << *intArray;
      intArray += 1;
    }

    std::cout << "\r\n## IntArrayFn ends ##";
}
```

`sizeof(int*)/sizeof(int)` is **2** in case of a 64 bit application and **1** for 32 bit applications. 
>**Remember**, raising a `int*` by one means to skip four bytes.

After skipping the header and reading the size, we have access to all array elements.

## StringFn
A `string` is a little bit special because we have to convert it to print it to the console. I have copied a few lines of code for that task from a Stackoverflow article. Apart from this it is the same like in the integer array above:

```cpp
if (strcmp(fnName, "StringFn") == 0) {
    COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[0];
    UINT_PTR valuePtr = range.startAddress;

    byte** ptr = (byte**)valuePtr;
    byte* strPtr = *ptr;
    strPtr = strPtr + sizeof(int*);
    long stringLength = *(long*)strPtr;
    strPtr += 4;
    char* dest = new char[100];

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> codecvt;
    std::string u8str = codecvt.to_bytes((char16_t*)strPtr);

    std::cout << "\r\n\r\n## Entered StringFn ##\r\n";
    std::cout << "Length: " << stringLength << "\r\n";
    std::cout << "\r\argument: " << u8str;
    std::cout << "\r\n## StringFn ends ##";

    delete[] dest;
}
```

# Conclusion
As you can see, it is no big deal at all. I think I will try to get the values of unknown functions, too. This will be helpful in some debugging scenarios I think.

# Additional Links
[COR_PRF_FUNCTION_ARGUMENT_RANGE](https://docs.microsoft.com/en-us/dotnet/framework/unmanaged-api/profiling/cor-prf-function-argument-range-structure)
[Theoretical usage of COR_PRF_FUNCTION_ARGUMENT_RANGE](https://github.com/dotnet/docs/issues/6728)
[Memory Layout](https://devblogs.microsoft.com/premier-developer/managed-object-internals-part-1-layout/)
[Layout of managed Arrays](https://windowsdebugging.wordpress.com/2012/04/07/memorylayoutofarrays/)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
