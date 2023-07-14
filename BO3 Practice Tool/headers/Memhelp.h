#pragma once
#include <Windows.h>
#include <stdint.h>

namespace BO3PT {

    typedef int MemStates;

    enum MemStates_ {
        Unloaded,
        Loaded
    };

    class MemState {
    public:
        static void* GetModuleBaseAddress(uint32_t processID, const wchar_t* moduleName);
        static DWORD GetProcessIdByName(const wchar_t* processName);

        static DWORD GetPID() { return s_PID; }
        static HANDLE GetHandle() { return s_Handle; }
        static void* GetBaseAddress() { return s_BaseAddress; }
        static void* GetMapNameAddress() { return s_MapNameAddress; }
        static void* GetRoundAddress() { return s_RoundAddress; }
        static int GetRoundValue() { return s_RoundValue; }
        static char* GetMapNameValue() { return s_MapNameValue; }
        static MemStates GetState() { return s_State; }

        static bool SetPID(const wchar_t* moduleName);
        static bool SetAddresses(const wchar_t* moduleName);
        static bool SetHandle();
        static bool ReadData();
    private:
        static DWORD s_PID;
        static HANDLE s_Handle;
        static void* s_BaseAddress;
        static void* s_MapNameAddress;
        static void* s_RoundAddress;
        static int s_RoundValue;
        static char s_MapNameValue[13];
        static MemStates s_State;
    };

}