---
published: true
title: "Call function in unmanaged DLL from C# and pass custom data types [Marshal]"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/net-internals/marshal-example/assets/header.jpg"
description: "This article shows how you can write a custom marshaler in order to call into unmanaged DLL's and pass custom data to the functions."
tags: cpp, csharp, dotnet, tutorial
series:
canonical_url:
---

>**Note:** Get the full runnable example here: [Marshal example](https://github.com/gabbersepp/dev.to-posts/tree/master/blog-posts/net-internals/marshal-example/code)

During my [last doings](https://dev.to/gabbersepp/win32-hooks-spy-how-i-got-rid-of-those-useless-mouse-buttons-using-windows-hooks-16el) I needed to call a unmanaged library from .NET. That mechanism is called [`Platform Invoke (P/Invoke)`](https://docs.microsoft.com/de-de/dotnet/standard/native-interop/pinvoke) can easily be done by using the `DllImport` attribute:

```cs
[DllImport("Dll1.dll")]
public static extern void Test();
```

But what if we want to pass some parameters? Let's say you have this C++ function definition:

```cpp
void __stdcall Test(intptr_t pointer)
```

Well, this one is easy:

```cs
[DllImport("Dll1.dll")]
 public static extern void Test(IntPtr pointer);
```

No work must be done by you to get this running. The transition from `IntPtr` to `intptr_t` is done by .NET using `Marshaling`. This is somehow like sending data to a web server. In the client code you have a `Date` object but the server expects a `DateTimeOffset`. The transisition is done by using e.g. JSON representation and a JSON serializer that is capable of converting a string into the `DateTimeOffset` structure. There are [many types](https://docs.microsoft.com/de-de/dotnet/standard/native-interop/type-marshaling) that can be marshaled out of the box.

# Sending custom datatypes
But what can we do, if we have a `C++` signature like this:

```cpp
struct TestFnParams {
    int A;
    char* B;
};

void __stdcall TestFunction(TestFnParams* p)
```

And a `C#` object like this:

```cs
public class TestFnParams
{
    public int PropertyA { get; set; }
    public string PropertyB { get; set; }
}
```

For this case .NET provides us with the capability to write an own `Marshaler`. In the following sections we are going to write a very simple one.

## The C++ code

We just print both, the number and the string, that were passed in the struct. `TestFunction` expects a pointer to a `TestFnParams` struct:

```cpp
// ./code/DLL1/dllmain.cpp#L20-L29

struct TestFnParams {
    int A;
    char* B;
};

void __stdcall TestFunction(TestFnParams* p) {
    std::cout << "Number: " << p->A << "\r\n";
    std::cout << "String: " << p->B << "\r\n";
    std::cout << "Hello from dll";
}
```

## The C# code
First we define the class:

```cs
// ./code/ConsoleApp1/Program.cs#L70-L74

public class TestFnParams
{
  public int PropertyA { get; set; }
  public string PropertyB { get; set; }
}
```

And the main function:

```cs
// ./code/ConsoleApp1/Program.cs#L58-L67

static void Main(string[] args)
{
  var obj = new TestFnParams
  {
    PropertyA = 100,
    PropertyB = "Hello from managed object"
  };
  TestFunction(obj);
  Console.Read();
}
```

Now specify the `DllImport` attribute with a small difference to the normal usage:

```cs
// ./code/ConsoleApp1/Program.cs#L55-L56

[DllImport("Dll1.dll", CallingConvention = CallingConvention.StdCall)]
public static extern void TestFunction([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(CustomMarshaler))] TestFnParams p);
```

By using the `MarshalAs` attribute we can easily tell .NET to use our own marshaler.

## Implementing the CustomMarshaler
Just create a new class and implement `ICustomMarshaler`. It has following methods:

+ **object MarshalNativeToManaged (IntPtr pNativeData)**

  This method must be implemented if you are dealing with **return values of custom types** or **out parameters**. I don't use both so I skip it.

+ **public IntPtr MarshalManagedToNative (object ManagedObj);**

  Used to convert the method parameter into the native representation.

+ **public int GetNativeDataSize ();**

  From what I've understand, we can safely return `-1` here because `MarshalManagedToNative` returns a pointer that always have a fixed size. In my test this method was not called at all.

+ **public void CleanUpNativeData (IntPtr pNativeData);**

  Here we have to do the memory cleanup. Always remember, native data structures are unmanaged and thus you are responsible for the garbage collection.

+ **public void CleanUpManagedData (object ManagedObj);**

  The cleanup hook for the managed objects. In our simple case we don't need it.

Also there is [one constraint](https://docs.microsoft.com/de-de/dotnet/api/system.runtime.interopservices.icustommarshaler?view=netcore-3.1#implementing-the-getinstance-method) that must be fulfilled by the implementing class. It must provide a static method named `GetInstance`:

```cs
static ICustomMarshaler GetInstance(string pstrCookie);  
```

The CLR is calling it in order to get an instance of the marshaler. This is necessary because the CLR will call that method once per application lifetime for every `pstrCookie`. So if you use the same function with different cookies, you can provide the CLR different instances. It is meant to be a singleton and thus should not hold any state.

## Use different instances
The cookie can be specified within the `MarshalAs` attribute. Following code will call the same function but use another instance of the marshaler:

```cs
[DllImport("Dll1.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "TestFunction")]
public static extern void TestFunctionOtherCookie([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(CustomMarshaler), MarshalCookie = "OtherCookie")]TestFnParams p);
```

Until now, our implementation looks like this:

```cs
public class CustomMarshaler : ICustomMarshaler
{
    void ICustomMarshaler.CleanUpManagedData(object ManagedObj)
    {
    }

    void ICustomMarshaler.CleanUpNativeData(IntPtr pNativeData)
    {
        // to be defined
    }

    int ICustomMarshaler.GetNativeDataSize()
    {
        return -1;
    }

    IntPtr ICustomMarshaler.MarshalManagedToNative(object ManagedObj)
    {
        // to be defined
    }

    object ICustomMarshaler.MarshalNativeToManaged(IntPtr pNativeData)
    {
        throw new NotImplementedException();
    }

    public static ICustomMarshaler GetInstance(string cookie)
    {
        return new CustomMarshaler();
    }
}
```

## Implement MarshalManagedToNative
First you have to know what kind of data the native function expects. In this case it is a pointer to a struct. How does the struct look like?

```cpp
struct TestFnParams {
    int A;
    char* B;
};
```

In the memory you just have 8 bytes (in a 32bit process) somewherre in the memory where the first four bytes represent the integer `A` and the subsequent four bytes represent the pointer to the string `B`. To provide native data you have to allocate memory and write into it. Fortunatelly there is the `Marshal` class that provides a few functions to us:
+ `AllocHGlobal(size)`: requests a memory block with the specified size in bytes
+ `Write*`: functions to write into that memory block

So let's start by casting the function parameter:

```cs
// ./code/ConsoleApp1/Program.cs#L24-L30

IntPtr ICustomMarshaler.MarshalManagedToNative(object ManagedObj)
{
  var casted = ManagedObj as TestFnParams;
  if (casted == null)
  {
    return IntPtr.Zero;
  }
```

Now we must use `AllocHGlobal`. But how many bytes do we need? Of course we need the mentioned *8* bytes to represent the struct. To support both, 32bit and 64 bit, use `sizeof`:
```cs
var ptr = Marshal.AllocHGlobal(sizeof(int) + Marshal.SizeOf(typeof(IntPtr)));
```

The next step is to write the integer `PropertyA` into that memory block:

```cs
Marshal.WriteInt32(ptr, casted.PropertyA);
```

To write the string `PropertyB` into the native memory, following steps are required:
+ allocate a memory block with the size: `PropertyB.Length + 1`. Mind the `+1` which is for holding the terminating `\0` byte.
+ write that pointer to `[ptr + 4]`
+ write the char bytes to the newly allocated memory

And this is the code:

```cs
// ./code/ConsoleApp1/Program.cs#L34-L39

var bytes = Encoding.UTF8.GetBytes(casted.PropertyB);
var strPtr = Marshal.AllocHGlobal(bytes.Length + 1);
Marshal.Copy(bytes, 0, strPtr, bytes.Length);
Marshal.WriteByte(strPtr + bytes.Length, 0);
Marshal.WriteIntPtr(ptr + sizeof(int), strPtr);
return ptr;
```

That's all. You just marshaled a managed object successfully to a native function. The last step is to return the pointer to your manually created struct: `return ptr;`. This will be the function parameter which is received by the native function.

# Conclusion
I didn't expect this being so easy to implement (of course I don't have choosen the most complexe example here). The whole code (including reading documentation and troubleshooting) was written in under two hours if I remember correctly. One mistake that costs very much time was that I tried to copy the char bytes to `[ptr + 4]` what was completeley wrong of course:

```cs
var ptr = Marshal.AllocHGlobal(sizeof(int) + casted.PropertyB.Length);
Marshal.WriteInt32(ptr, casted.PropertyA);
var bytes = Encoding.UTF8.GetBytes(casted.PropertyB);
Marshal.Copy(bytes, 0, ptr + 4, bytes.Length);
```

# Additional links

[ICustomMarshaler](https://docs.microsoft.com/de-de/dotnet/api/system.runtime.interopservices.icustommarshaler)
[Long Marshaler](https://limbioliong.wordpress.com/2013/11/23/example-custom-marshaler-the-long-marshaler/)
[Understanding Custom Marshaling](https://limbioliong.wordpress.com/2013/11/03/understanding-custom-marshaling-part-1/)
[More examples using marhsal cookie](https://limbioliong.wordpress.com/2013/11/19/custom-marshaling-comments-and-an-example-use-of-the-marshal-cookie/)
[Types](https://docs.microsoft.com/de-de/dotnet/standard/native-interop/type-marshaling)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
