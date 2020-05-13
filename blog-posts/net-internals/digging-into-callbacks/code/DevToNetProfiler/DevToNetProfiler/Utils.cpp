#include "pch.h"
#include "cor.h"
#include "corprof.h"
#include "Utils.h"

Utils::Utils(ICorProfilerInfo2* info) {
  this->iCorProfilerInfo = info;
}

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
