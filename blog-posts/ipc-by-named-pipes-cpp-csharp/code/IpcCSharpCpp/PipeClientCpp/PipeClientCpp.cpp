#include <windows.h>
#include <iostream>

int main()
{
  HANDLE fileHandle = CreateFileW(TEXT("\\\\.\\pipe\\my-very-cool-pipe-example"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  WriteFile(fileHandle, "test from c++\r\n", 6, nullptr, NULL);
}
