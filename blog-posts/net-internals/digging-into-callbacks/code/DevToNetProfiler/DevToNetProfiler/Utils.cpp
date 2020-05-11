#include "pch.h"
#include "cor.h"
#include "corprof.h"
#include "Utils.h"

Utils::Utils(ICorProfilerInfo2* info) {
  this->iCorProfilerInfo = info;
}

void Utils::GetClassName(ObjectID objectId, char* output, ULONG outputLength) {
  ClassID classId;
  ModuleID moduleId;
  mdTypeDef typeDefToken;
  IMetaDataImport* metadata;
  wchar_t* className = new wchar_t[100];
  ULONG read = 0;

  iCorProfilerInfo->GetClassFromObject(objectId, &classId);
  iCorProfilerInfo->GetClassIDInfo(classId, &moduleId, &typeDefToken);
  iCorProfilerInfo->GetModuleMetaData(moduleId, ofRead, IID_IMetaDataImport, (IUnknown**)&metadata);
  metadata->GetTypeDefProps(typeDefToken, className, outputLength, &read, NULL, NULL);
  metadata->Release();

  memset(output, 0, 100);
  wcstombs(output, className, 100);
  delete[] className;
}
