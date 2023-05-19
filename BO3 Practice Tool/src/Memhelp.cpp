#include "memHelp.h"
#include "GlobalData.h"

using namespace BO3PT;

namespace MemHelp
{
    uintptr_t* GetModuleBaseAddress(uint32_t ProcessID, const wchar_t* ModuleName)
    {
        void* hSnap = nullptr;
        MODULEENTRY32W Mod32 = { 0 };

        if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID)) == INVALID_HANDLE_VALUE)
            return 0;

        Mod32.dwSize = sizeof(MODULEENTRY32W);
        while (Module32NextW(hSnap, &Mod32))
        {
            if (!_wcsicmp(ModuleName, Mod32.szModule))
            {
                CloseHandle(hSnap);
                return (uintptr_t*)Mod32.modBaseAddr;
            }
        }

        CloseHandle(hSnap);
        return nullptr;
    }

    DWORD GetProcessIdByName(const wchar_t* name)
    {
        PROCESSENTRY32W pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32W);
        LogFile("Starting Enumeration");
        if (Process32FirstW(hsnap, &pt)) {
            do {
                if (!_wcsicmp(pt.szExeFile, name)) {
                    LogFile("BlackOps3.exe Found");
                    CloseHandle(hsnap);
                    return pt.th32ProcessID;
                }
            } while (Process32NextW(hsnap, &pt));
        }
        LogFile("BlackOps3.exe Not Found");
        CloseHandle(hsnap);
        return 0;
    }

    HANDLE GetProcHandle(const wchar_t* szProcName)
    {
        HANDLE hProc = NULL;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        PROCESSENTRY32W pe32 = { 0 };
        pe32.dwSize = sizeof(PROCESSENTRY32W);

        if (Process32FirstW(hSnap, &pe32))
        {
            if (!wcscmp(szProcName, pe32.szExeFile))
            {
                hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
            }
            else
            {
                while (Process32NextW(hSnap, &pe32))
                {
                    if (!wcscmp(szProcName, pe32.szExeFile))
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
