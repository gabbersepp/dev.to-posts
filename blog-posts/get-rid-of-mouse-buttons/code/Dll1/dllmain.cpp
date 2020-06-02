#include "pch.h"

HHOOK hkKey = NULL;
HINSTANCE hInstHookDll = NULL;
LRESULT CALLBACK getMsgProc(int code, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID lpReserved)
{
    switch (reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        hInstHookDll = (HINSTANCE)hModule;
        break;
    }
    return TRUE;
}

void __stdcall SetHook()
{
    if (hkKey == NULL)
        hkKey = SetWindowsHookEx(WH_GETMESSAGE, getMsgProc, hInstHookDll, 0);
}

void __stdcall RemoveHook()
{
    if (hkKey != NULL)
        UnhookWindowsHookEx(hkKey);
    hkKey = NULL;
}

LRESULT CALLBACK getMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code >= 0 && code == HC_ACTION)
    {
        MSG* msg = (MSG*)lParam;
        if (msg->message == WM_XBUTTONDOWN || msg->message == WM_XBUTTONUP) {
            msg->message = WM_NULL;
        }
    }
    return CallNextHookEx(hkKey, code, wParam, lParam);
}