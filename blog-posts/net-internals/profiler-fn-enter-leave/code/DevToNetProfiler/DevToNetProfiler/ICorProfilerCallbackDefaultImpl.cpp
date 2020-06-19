#include "pch.h"
#include "ICorProfilerCallbackDefaultImpl.h"
#include<corprof.h>

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::Initialize(IUnknown* pICorProfilerInfoUnk) {
  return S_OK;
}
HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionThrown(ObjectID thrownObjectID) {
  return S_OK;
}
HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ObjectAllocated(ObjectID objectID, ClassID classID) {
  return S_OK;
}
HRESULT __stdcall ICorProfilerCallbackDefaultImpl::Shutdown()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AppDomainCreationStarted(AppDomainID appDomainID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AppDomainShutdownStarted(AppDomainID appDomainID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AssemblyLoadStarted(AssemblyID assemblyID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AssemblyLoadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AssemblyUnloadStarted(AssemblyID assemblyID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ModuleLoadStarted(ModuleID moduleID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ModuleUnloadStarted(ModuleID moduleID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ClassLoadStarted(ClassID classID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ClassLoadFinished(ClassID classID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ClassUnloadStarted(ClassID classID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ClassUnloadFinished(ClassID classID, HRESULT hrStatus)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::FunctionUnloadStarted(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL* pbUseCachedFunction)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::JITFunctionPitched(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL* pfShouldInline)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ThreadCreated(ThreadID threadID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ThreadDestroyed(ThreadID threadID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingClientInvocationStarted()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingClientSendingMessage(GUID* pCookie, BOOL fIsAsync)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingClientReceivingReply(GUID* pCookie, BOOL fIsAsync)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingClientInvocationFinished()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingServerReceivingMessage(GUID* pCookie, BOOL fIsAsync)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingServerInvocationStarted()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingServerInvocationReturned()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RemotingServerSendingReply(GUID* pCookie, BOOL fIsAsync)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeSuspendFinished()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeSuspendAborted()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeResumeStarted()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeResumeFinished()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeThreadSuspended(ThreadID threadID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RuntimeThreadResumed(ThreadID threadID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionUnwindFunctionEnter(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionUnwindFunctionLeave()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionSearchFunctionEnter(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionSearchFunctionLeave()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionSearchFilterEnter(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionSearchFilterLeave()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionSearchCatcherFound(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionCLRCatcherFound()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionCLRCatcherExecute()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionOSHandlerEnter(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionOSHandlerLeave(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionUnwindFinallyEnter(FunctionID functionID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionUnwindFinallyLeave()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionCatcherEnter(FunctionID functionID,
  ObjectID objectID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ExceptionCatcherLeave()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void* pVTable, ULONG cSlots)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void* pVTable)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::ThreadNameChanged(ThreadID threadID, ULONG cchName, WCHAR name[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::GarbageCollectionFinished()
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::RootReferences2(ULONG cRootRefs, ObjectID rootRefIDs[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIDs[])
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::HandleCreated(GCHandleID handleID, ObjectID initialObjectID)
{
  return S_OK;
}

HRESULT __stdcall ICorProfilerCallbackDefaultImpl::HandleDestroyed(GCHandleID handleID)
{
  return S_OK;
}

