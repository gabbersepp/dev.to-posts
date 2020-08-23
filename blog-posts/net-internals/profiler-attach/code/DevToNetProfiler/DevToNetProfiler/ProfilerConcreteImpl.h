#pragma once

#include "pch.h"
#include "DevToNetProfiler_i.h"
#include "ICorProfilerCallbackDefaultImpl.h"
#include "Utils.h"

class ATL_NO_VTABLE ProfilerConcreteImpl :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<ProfilerConcreteImpl, &CLSID_NetProfiler>,
  public ICorProfilerCallbackDefaultImpl
{
public:
  ProfilerConcreteImpl();

  DECLARE_REGISTRY_RESOURCEID(IDR_PROFILER)
  BEGIN_COM_MAP(ProfilerConcreteImpl)
    COM_INTERFACE_ENTRY(ICorProfilerCallback)
    COM_INTERFACE_ENTRY(ICorProfilerCallback2)
  END_COM_MAP()
  DECLARE_PROTECT_FINAL_CONSTRUCT()

  HRESULT FinalConstruct();
  void FinalRelease();

  HRESULT __stdcall ProfilerConcreteImpl::Initialize(IUnknown* pICorProfilerInfoUnk);
  HRESULT __stdcall ProfilerConcreteImpl::ExceptionThrown(ObjectID thrownObjectID);
  HRESULT __stdcall ProfilerConcreteImpl::ObjectAllocated(ObjectID objectID, ClassID classID);

private:
  bool ActivateCallbacks;
  int* PHashMap;
};

OBJECT_ENTRY_AUTO(__uuidof(NetProfiler), ProfilerConcreteImpl)
