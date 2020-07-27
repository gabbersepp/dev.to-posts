; -----------------------------------------------------------------------------------------------------------	
; Our variables
; -----------------------------------------------------------------------------------------------------------	
_DATA SEGMENT
	pActivateEnterLeaveCallback qword 0
  pHashMap qword 0
  mapSize dword 0
_DATA ENDS

extern EnterCpp:proc
extern StackOverflowDetected:proc


; -----------------------------------------------------------------------------------------------------------	
; Text or code segment
; -----------------------------------------------------------------------------------------------------------	
_TEXT	SEGMENT

PUBLIC InitEnterLeaveCallbacks

InitEnterLeaveCallbacks PROC
  mov pActivateEnterLeaveCallback, RCX
  mov pHashMap, RDX
  mov mapSize, R8D
  ret
InitEnterLeaveCallbacks ENDP

PUBLIC FnEnterCallback

FnEnterCallback PROC
  mov RAX, pActivateEnterLeaveCallback
  cmp byte ptr [RAX], 1
  JNE skipCallback

  mov R8, pHashMap
  MOV RAX, RCX
  XOR RDX, RDX
  DIV DWORD PTR [mapSize]
  ADD R8, RDX
  INC DWORD PTR [R8]
  CMP DWORD PTR [R8], 30
  JB skipStackOverflow

  push R9
  push rcx
  xor rdx, rdx
  MOV EDX, [R8]
  SUB RSP, 20h
  CALL StackOverflowDetected
  ADD RSP, 20h
  pop rcx
  pop R9

  skipStackOverflow:

  sub RSP, 20h
  MOV RDX, R9
  CALL EnterCpp
  add RSP, 20h

  skipCallback:
  ret
FnEnterCallback ENDP

PUBLIC FnLeaveCallback

FnLeaveCallback PROC
  MOV RAX, pActivateEnterLeaveCallback
  CMP BYTE PTR [RAX], 1
  JNE skipCallback

  MOV R8, pHashMap
  MOV RAX, RCX
  XOR RDX, RDX
  DIV DWORD PTR [mapSize]
  ADD R8, RDX
  DEC DWORD PTR [R8]

  skipCallback:
  ret
FnLeaveCallback ENDP

PUBLIC FnTailcallCallback

FnTailcallCallback PROC
  ret
FnTailcallCallback ENDP

_TEXT	ENDS

END
