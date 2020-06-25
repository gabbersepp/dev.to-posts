#pragma once
#include "pch.h"

//void Test(int a, int b, int* output);

void FnEnterCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);

void FnLeaveCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);

void FnTailcallCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func);
