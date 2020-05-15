// dllmain.h : Deklaration der Modulklasse.

class CDevToNetProfilerModule : public ATL::CAtlDllModuleT< CDevToNetProfilerModule >
{
public :
	DECLARE_LIBID(LIBID_DevToNetProfilerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DEVTONETPROFILER, "{103d660d-1cb4-4410-85dd-67b3aa489626}")
};

extern class CDevToNetProfilerModule _AtlModule;
