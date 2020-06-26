// dllmain.cpp : Implementierung von DllMain.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "DevToNetProfiler_i.h"
#include "dllmain.h"

CDevToNetProfilerModule _AtlModule;

// DLL-Einstiegspunkt
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved);
}
