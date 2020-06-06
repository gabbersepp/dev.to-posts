---
published: false
title: "Call function in unmanaged DLL from C# and pass custom data types [Marshal]"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/net-internals/marshal-example/assets/header.jpg"
description: "Introducing a VSCode extension that provides intellisense for cypress fixture file paths"
tags: cypress, javascript, vscode, productivity
series:
canonical_url:
---

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
    std::cout << "Hello from dll" << sizeof(TestFnParams);
}
```

## The C# code
First we define the class:

```cs
// ./code/ConsoleApp1/Program.cs#L71-L75

public class TestFnParams
{
  public int PropertyA { get; set; }
  public string PropertyB { get; set; }
}
```

And the main function:

```cs
// ./code/ConsoleApp1/Program.cs#L59-L68

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
// ./code/ConsoleApp1/Program.cs#L56-L57

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

  From what I've understand, we can savely return `-1` here because `MarshalManagedToNative` returns a pointer that always have a fixed size. In my test this method was not called at all.

+ **public void CleanUpNativeData (IntPtr pNativeData);**

  Here we have to do the memory cleanup. Always remember, native data structures are unmanaged and thus you are responsible for the garbage collection.

+ **public void CleanUpManagedData (object ManagedObj);**

  The cleanup hook for the managed objects. In your simple case we don't need it.

Also there is [one constraint](https://docs.microsoft.com/de-de/dotnet/api/system.runtime.interopservices.icustommarshaler?view=netcore-3.1#implementing-the-getinstance-method) that must be fullfilled by the implementing class. It must provide a static method named `GetInstance`:

```cs
static ICustomMarshaler GetInstance(string pstrCookie);  
```

The CLR is calling it in order to get an instance of the marshaler. This is necessary because the CLR will call that method once per application lifetime for every `pstrCookie`. So if you use the same function with different cookies, you can provide the CLR different instances. Nevertheless it is meant to be a singleton.

## Use different instances
The cookie can be specified within the `MarshalAs` attribute. Following code will call the same function but use another instance of the marshaler:

```cs
[DllImport("Dll1.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "TestFunction")]
public static extern void TestFunctionOtherCookie([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(CustomMarshaler), MarshalCookie = "OtherCookie")]TestFnParams p);
```

Our implementation lokks like this until now:

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

In the memory you just have 8 bytes somewherre in the memory where the first four bytes represent the integer `A` and the subsequent four bytes represent the pointer to the string `B`.


# Own datatypes

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
