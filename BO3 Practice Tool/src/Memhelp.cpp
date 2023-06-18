#include "memHelp.h"

#include <TlHelp32.h>

#include "Walnut/Logger.h"

namespace BO3PT
{
    DWORD MemState::s_PID;
    HANDLE MemState::s_Handle;
    uintptr_t* MemState::s_BaseAddress;
    uintptr_t* MemState::s_MapNameAddress;
    uintptr_t* MemState::s_RoundAddress;
    int MemState::s_RoundValue;
    char MemState::s_MapNameValue[13];
    MemStates MemState::s_State;

    uintptr_t* MemState::GetModuleBaseAddress(uint32_t processID, const wchar_t* moduleName)
    {
        void* hSnap = nullptr;
        MODULEENTRY32W Mod32 = { 0 };

        if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID)) == INVALID_HANDLE_VALUE) {
            return 0;
        }

        Mod32.dwSize = sizeof(MODULEENTRY32W);
        while (Module32NextW(hSnap, &Mod32)) {
            if (!_wcsicmp(moduleName, Mod32.szModule)) {
                CloseHandle(hSnap);
                return (uintptr_t*)Mod32.modBaseAddr;
            }
        }

        CloseHandle(hSnap);
        return nullptr;
    }

    DWORD MemState::GetProcessIdByName(const wchar_t* processName)
    {
        PROCESSENTRY32W pt = { 0 };
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32W);
        Walnut::Logger::Log(Walnut::MessageType::Info, "Starting Enumeration");
        if (Process32FirstW(hsnap, &pt)) {
            do {
                if (!_wcsicmp(pt.szExeFile, processName)) {
                    Walnut::Logger::Log(Walnut::MessageType::Success, "BlackOps3.exe Found");
                    CloseHandle(hsnap);
                    return pt.th32ProcessID;
                }
            } while (Process32NextW(hsnap, &pt));
        }
        Walnut::Logger::Log(Walnut::MessageType::Warning, "BlackOps3.exe Not Found");
        CloseHandle(hsnap);
        return 0;
    }

    bool MemState::SetPID(const wchar_t* moduleName)
    {
        s_PID = GetProcessIdByName(moduleName);
        if (s_PID != NULL) {
            s_State = Loaded;
            return true;
        }
        return false;
    }

    bool MemState::SetAddresses(const wchar_t* moduleName)
    {
        s_BaseAddress = GetModuleBaseAddress(s_PID, moduleName);
        if (s_BaseAddress == nullptr) {
            s_State = Unloaded;
            return false;
        }
        s_MapNameAddress = s_BaseAddress + 0x179DF840 / 8;
        s_RoundAddress = s_BaseAddress + 0x1140DC30 / 8;
        return true;
    }

    bool MemState::SetHandle()
    {
        s_Handle = OpenProcess(PROCESS_VM_READ, false, s_PID);
        if (s_Handle == INVALID_HANDLE_VALUE) {
            s_State = Unloaded;
            return false;
        }
        return true;
    }

    bool MemState::ReadData()
    {
        if (s_Handle == INVALID_HANDLE_VALUE ||
            !ReadProcessMemory(s_Handle, s_MapNameAddress, &s_MapNameValue, sizeof(s_MapNameValue), NULL) ||
            !ReadProcessMemory(s_Handle, s_RoundAddress, &s_RoundValue, sizeof(s_RoundValue), NULL))
        {
            s_State = Unloaded;
            CloseHandle(s_Handle);
            s_Handle = NULL;
            return false;
        }
    }
}
