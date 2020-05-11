---
published: false
title: "Digging into several callbacks, its parameters and how you can obtain more information about an event."
description: "I list some callbacks, what can be done with them and how you can obtain more information about an event."
tags: dotnet, debug, tutorial, cpp
series: Net-Profiler
canonical_url:
---

# A few callbacks explained
Until now we setup a simple profiler that is capable of logging every exception. Do you remember the output? It was just `exception thrown`, which may be useful but surely it will be much more useful if the exception name would be printed.

If you looked at several callback methods, you may have noticed, that often you get only an id. E.g. a `FunctionId` or a `ClassId` and such things. This makes sense as passing more information to the callbacks will increase the overall CPU & RAM load. On the other hand, you have to call a few methods to get more information. Unfortunatelly it is often not very obviously what kind of method you have to use. At least I found it a bit confusing. But maybe the whole process is documented somewhere in the .NET documentation :-)

To make your life easier, I will show you some use cases. I will not write too much about it. I mean, I'm only calling some functions in a very stupid manner. So if you have questions, please ask and I'll try to answer them.

# ExceptionThrown callback
Let's take the example project from the last lesson. There we implemented the `ExceptionThrown` callback. As parameter you only get an `ObjectID`. Wouldn't it be nice if we could print the name of the exception? Let's do this.

## Utils
I create an own class for the methods that retrive more information about an event. Let's call it `Utils`. You should be aware that I will not write the most performant code and you may encounter a lot of things than could be improoved. But be sure this was all done for an easier understanding.

## Get class name by object ID

```cpp
// ./code/DevToNetProfiler/DevToNetProfiler/Utils.cpp#L10-L27

void Utils::GetClassName(ObjectID objectId, char* output, ULONG outputLength) {
  ClassID classId;
  ModuleID moduleId;
  mdTypeDef typeDefToken;
  IMetaDataImport* metadata;
  wchar_t* className = new wchar_t[100];
  ULONG read = 0;

  iCorProfilerInfo->GetClassFromObject(objectId, &classId);
  iCorProfilerInfo->GetClassIDInfo(classId, &moduleId, &typeDefToken);
  iCorProfilerInfo->GetModuleMetaData(moduleId, ofRead, IID_IMetaDataImport, (IUnknown**)&metadata);
  metadata->GetTypeDefProps(typeDefToken, className, outputLength, &read, NULL, NULL);
  metadata->Release();

  memset(output, 0, 100);
  wcstombs(output, className, 100);
  delete[] className;
}
```

Using this code:

```cs
static void Main(string[] args)
{
    try
    {
    throw new CoronaException();
    }
    catch
    {
    }

    Console.Read();
}
```

I'm getting that output:

![](./assets/exception-name.jpg)




----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
