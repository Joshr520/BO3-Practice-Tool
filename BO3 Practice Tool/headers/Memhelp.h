#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace MemHelp
{
    uintptr_t* GetModuleBaseAddress(uint32_t ProcessID, const char* ModuleName);
    DWORD GetProcessIdByName(const char* name);
    HANDLE GetProcHandle(const char* szProcName);
}