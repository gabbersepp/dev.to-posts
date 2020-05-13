---
published: false
title: "Digging into several callbacks and how you can obtain more information about an event"
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
// ./code/DevToNetProfiler/DevToNetProfiler/Utils.cpp#L10-L37

bool Utils::GetClassNameByObjectId(ObjectID objectId, char* output, ULONG outputLength) {
  ClassID classId;
  iCorProfilerInfo->GetClassFromObject(objectId, &classId);
  return this->GetClassNameByClassId(classId, output, outputLength);
}

bool Utils::GetClassNameByClassId(ClassID classId, char* output, ULONG outputLength) {
  ModuleID moduleId;
  mdTypeDef typeDefToken;
  IMetaDataImport* metadata;
  wchar_t* className = new wchar_t[outputLength];
  ULONG read = 0;

  HRESULT hresult = iCorProfilerInfo->GetClassIDInfo(classId, &moduleId, &typeDefToken);

  if (hresult < 0 || moduleId == 0) {
    return false;
  }

  hresult = iCorProfilerInfo->GetModuleMetaData(moduleId, ofRead, IID_IMetaDataImport, (IUnknown**)&metadata);
  hresult = metadata->GetTypeDefProps(typeDefToken, className, outputLength, &read, NULL, NULL);
  metadata->Release();

  memset(output, 0, outputLength);
  wcstombs(output, className, outputLength);
  delete[] className;
  return true;
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

And this callback implementation:
```cpp
// ./code/DevToNetProfiler/DevToNetProfiler/ProfilerCallback.cpp#L29-L36

HRESULT __stdcall ProfilerCallback::ExceptionThrown(ObjectID thrownObjectID)
{
  char* className = new char[100];
  utils->GetClassNameByObjectId(thrownObjectID, className, 100);
  cout << "\t\nfrom profiler: exception thrown: " << className << "\r\n";
  delete[] className;
  return S_OK;
}
```

I'm getting that output:

![](./assets/exception-name.jpg)

# ObjectAllocated 
Set following flags to get notified about very allocated object: `COR_PRF_MONITOR_OBJECT_ALLOCATED | COR_PRF_ENABLE_OBJECT_ALLOCATED`. 

Implement the callback:

```cpp
// ./code/DevToNetProfiler/DevToNetProfiler/ProfilerCallback.cpp#L38-L46

HRESULT __stdcall ProfilerCallback::ObjectAllocated(ObjectID objectID, ClassID classID)
{
  char* className = new char[1000];
  if (utils->GetClassNameByClassId(classID, className, 1000)) {
    cout << "\t\nfrom profiler: class allocated: " << className << "\r\n";
  }
  delete[] className;
  return S_OK;
}
```


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
