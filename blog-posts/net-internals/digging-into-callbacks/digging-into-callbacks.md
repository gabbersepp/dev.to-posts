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

To make your life easier, I will show you some use cases. 

# ExceptionThrown callback
Let's take the example project from the last lesson. There we implemented the `ExceptionThrown` callback. As parameter you only get an `ObjectID`. But what we want to receive: The classname.


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
