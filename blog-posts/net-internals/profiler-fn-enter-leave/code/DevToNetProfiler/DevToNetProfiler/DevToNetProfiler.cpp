// DevToNetProfiler.cpp: Implementierung von DLL-Exporten.


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "DevToNetProfiler_i.h"
#include "dllmain.h"


using namespace ATL;

// Wird verwendet, um festzustellen, ob die DLL von OLE entladen werden kann.
_Use_decl_annotations_
STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow();
}

// Gibt eine Klassenfactory zurück, um ein Objekt vom angeforderten Typ zu erstellen.
_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Fügt der Systemregistrierung Einträge hinzu.
_Use_decl_annotations_
STDAPI DllRegisterServer(void)
{
	// Registriert Objekt, Typelib und alle Schnittstellen in Typelib.
	HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}

// DllUnregisterServer - Entfernt Einträge aus der Systemregistrierung.
_Use_decl_annotations_
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

