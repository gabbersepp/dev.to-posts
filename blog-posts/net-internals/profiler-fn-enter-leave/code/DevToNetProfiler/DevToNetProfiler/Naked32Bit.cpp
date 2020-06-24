#include "pch.h"

void __declspec(naked) FnEnterCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    ret 16
  }
}

void __declspec(naked) FnLeaveCallback(
  FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func,
  COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
  __asm {
    ret 16
  }
}

void __declspec(naked) FnTailcallCallback(FunctionID funcId,
  UINT_PTR clientData,
  COR_PRF_FRAME_INFO func) {
  __asm {
    ret 12
  }
}
