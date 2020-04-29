---
published: true
title: "Call a C library from C++"
description: "How can you write a DLL in C and use it in C++."
tags: c, cpp, library, beginner
series:
canonical_url:
---

Yesterday I needed to write a small C DLL and use it in my C++ sample application. It took some time to figure out how I must do this on my Windows PC with Visual Studio installed. 
This is just a little reminder for me, so don't expect too much :-).

# C code
The C app contains just a little function:

```c
// ./code/c/lib.c

#include <stdio.h>

__declspec(dllexport) void f()
{
    printf("\n This is a C code\n");
}
```

The keyword `__declspec(dllexport)` is only valid within the Microsoft compiler world and exports that function.

# Compile C
Open the Visual Studio Developer Command Prompt and navigate to the folder, where the C project is. Then simply type: `cl /LD lib.c`. This will create two files. A `lib.lib` and a `lib.dll`.

# C++ Code
Also nothing special. First we need the header file:
```cpp
// ./code/cpp/Console/Console/Header.h

#pragma once

extern "C" {
  __declspec(dllimport) void f();
}

```
You find `declspec` again, but this time it is specified with `dllimport` which makes perfectly sense as we are importing our function :-)

Our app:
```cpp
// ./code/cpp/Console/Console/Console.cpp

#include <iostream>
#include "Header.h"
#pragma comment(lib, "../../../c/lib.lib")

int main()
{
  f();
}

```

Note the line `#pragma comment(lib, "../../../c/lib.lib")` which specifies the location of the `.lib` so the linker is able to reference the function `f()`.

# Execute
If you now execute `Console.exe` you will get an error because the `dll` can not be find. To fix this, just copy the `lib.dll` to the folder where `Console.exe` is.

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
