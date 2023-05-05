#include "memHelp.h"
#include "Helper.h"

using namespace std;

namespace MemHelp
{
    uintptr_t* GetModuleBaseAddress(uint32_t ProcessID, const char* ModuleName)
    {
        void* hSnap = nullptr;
        MODULEENTRY32 Mod32 = { 0 };

        if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID)) == INVALID_HANDLE_VALUE)
            return 0;

        Mod32.dwSize = sizeof(MODULEENTRY32);
        while (Module32Next(hSnap, &Mod32))
        {
            if (!_stricmp(ModuleName, Mod32.szModule))
            {
                CloseHandle(hSnap);
                return (uintptr_t*)Mod32.modBaseAddr;
            }
        }

        CloseHandle(hSnap);
        return { 0 };
    }

    DWORD GetProcessIdByName(const char* name)
    {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32);
        LogFile("Starting Enumeration");
        if (Process32First(hsnap, &pt)) {
            do {
                if (!_stricmp(pt.szExeFile, name)) {
                    LogFile("BlackOps3.exe Found");
                    CloseHandle(hsnap);
                    return pt.th32ProcessID;
                }
            } while (Process32Next(hsnap, &pt));
        }
        LogFile("BlackOps3.exe Not Found");
        CloseHandle(hsnap);
        return 0;
    }

    HANDLE GetProcHandle(const char* szProcName)
    {
        HANDLE hProc = NULL;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        PROCESSENTRY32 pe32 = { 0 };
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnap, &pe32))
        {
            if (!strcmp(szProcName, pe32.szExeFile))
            {
                hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
            }
            else
            {
                while (Process32Next(hSnap, &pe32))
                {
                    if (!strcmp(szProcName, pe32.szExeFile))
                    {
                        hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                        break;
                    }
                }
            }
        }
        return hProc;
    }
}
