#pragma warning (disable : 4996)

#pragma once

#include "cor.h"
#include "corprof.h"

class Utils {
public:
  Utils(ICorProfilerInfo2* info);
  bool GetClassNameByObjectId(ObjectID objectId, char* output, ULONG outputLength);
  bool GetClassNameByClassId(ClassID classId, char* output, ULONG outputLength);
  bool GetFunctionNameById(FunctionID functionId, char* output, ULONG outputLength);
private:
  ICorProfilerInfo2* iCorProfilerInfo;
};

void __declspec(naked) FnEnterCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);

void __declspec(naked) FnLeaveCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);

void __declspec(naked) FnTailcallCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func);
