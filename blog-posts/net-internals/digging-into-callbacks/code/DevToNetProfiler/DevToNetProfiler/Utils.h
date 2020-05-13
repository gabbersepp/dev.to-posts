#pragma warning (disable : 4996)

#pragma once

#include "cor.h"
#include "corprof.h"

class Utils {
public:
  Utils(ICorProfilerInfo2* info);
  bool GetClassNameByObjectId(ObjectID objectId, char* output, ULONG outputLength);
  bool GetClassNameByClassId(ClassID classId, char* output, ULONG outputLength);
private:
  ICorProfilerInfo2* iCorProfilerInfo;
};
