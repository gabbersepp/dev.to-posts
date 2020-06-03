#include "pch.h"
#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

struct TestFnParams {
    int A;
    char* B;
};

void __stdcall TestFunction(TestFnParams* p) {
    std::cout << "Number: " << p->A << "\r\n";
    std::cout << "String: " << p->B << "\r\n";
    std::cout << "Hello from dll";
}