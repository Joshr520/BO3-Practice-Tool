#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace MemHelp
{
    uintptr_t* GetModuleBaseAddress(uint32_t ProcessID, const wchar_t* ModuleName);
    DWORD GetProcessIdByName(const wchar_t* name);
    HANDLE GetProcHandle(const wchar_t* szProcName);
}