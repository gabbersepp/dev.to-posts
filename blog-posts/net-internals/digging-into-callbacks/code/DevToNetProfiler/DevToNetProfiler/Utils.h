#pragma warning (disable : 4996)

#pragma once

#include "cor.h"
#include "corprof.h"

class Utils {
public:
  Utils(ICorProfilerInfo2* info);
  void GetClassName(ObjectID objectId, char* output, ULONG outputLength);
private:
  ICorProfilerInfo2* iCorProfilerInfo;
};
