#include "Memory.h"

#include <TlHelp32.h>
#include <thread>
#include <format>
#include <functional>

#include "Notify.h"
#include "ImGui/ImGuiExtendedWidgets.h"
#include "PracticeTool/Core/Log.h"
#include "PracticeTool/StructDefs/WeaponData.h"
#include "PracticeTool/UI/WindowBody.h"

DWORD GetProcessIDByName(std::wstring_view pName)
{
    PROCESSENTRY32W pt = { 0 };
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(hsnap, &pt)) {
        do {
            if (!wcscmp(pt.szExeFile, pName.data())) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32NextW(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return 0;
}

void* GetModuleBaseAddress(DWORD pID, std::wstring_view pName)
{
    void* hSnap = nullptr;
    MODULEENTRY32W Mod32 = { 0 };

    if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID)) == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Mod32.dwSize = sizeof(MODULEENTRY32W);
    while (Module32NextW(hSnap, &Mod32)) {
        if (!_wcsicmp(pName.data(), Mod32.szModule)) {
            CloseHandle(hSnap);
            return (void*)Mod32.modBaseAddr;
        }
    }

    CloseHandle(hSnap);
    return nullptr;
}

namespace Memory
{
    void* pBaseAddress = nullptr;
    void* levelTimeAddress = nullptr;
    void* roundAddress = nullptr;
    void* mapNameAddress = nullptr;
    void* menuStateAddress = nullptr;
    uintptr_t levelTimeOffset = 0xA6424FC;
    uintptr_t roundOffset = 0xA55BDEC;
    uintptr_t mapNameOffset = 0x179DF840;
    uintptr_t menuStateOffset = 0x1647A4D0;
    DWORD pID = 0;
    HANDLE pHandle = NULL;
    std::string pName = "BlackOps3.exe";
    std::wstring pNameW = L"BlackOps3.exe";

    void WatchMenuState();
    void ManageMemVars();

    void SetAddresses()
    {
        levelTimeAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pBaseAddress) + levelTimeOffset);
        roundAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pBaseAddress) + roundOffset);
        mapNameAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pBaseAddress) + mapNameOffset);
        menuStateAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pBaseAddress) + menuStateOffset);
    }

    void ProcessLifetime()
    {
        while (!BO3PracticeTool::Log::LogReady()) {
            Sleep(50);
        }

        Notify procWatch(std::format("process_watch_{}", pName));
        std::thread(ManageMemVars).detach();
        for (;;) {
            if (procWatch.Terminated()) {
                return;
            }
            if (procWatch.Running()) {
                if (!pID || !pBaseAddress) {
                    pID = GetProcessIDByName(pNameW);
                    if (pID) {
                        LOG_DEBUG("pID for process {}: {}", pName, pID);
                        pBaseAddress = GetModuleBaseAddress(pID, pNameW);
                        if (pBaseAddress) {
                            LOG_DEBUG("Base address for process {}: {}", pName, pBaseAddress);
                            SetAddresses();
                        }
                    }
                }
                if (pHandle == NULL) {
                    pHandle = OpenProcess(PROCESS_VM_READ, false, pID);
                    if (pHandle) {
                        LOG_DEBUG("Process {} opened successfully", pName);
                        std::thread(WatchMenuState).detach();
                    }
                }
            }

            Sleep(1000);
        }
    }

    void WatchMenuState()
    {
        LOG_DEBUG("Watching Menu State");

        char menuState[10] = "\0";
        while (!injected) {
            if (ReadProcessMemory(pHandle, menuStateAddress, &menuState, sizeof(menuState), NULL)) {
                if (!strcmp("room2", menuState) || !strcmp("zm", menuState)) {
                    injected = true;
                    LOG_DEBUG("Menu detected. Injecting tool now");
                    ShellExecuteA(NULL, NULL, "Compiler\\DebugCompiler.exe", "Compiler\\compiled.gsic T7 scripts\\shared\\duplicaterender_mgr.gsc --inject --noupdate", NULL, SW_HIDE);
                }
            }
            Sleep(50);
        }
    }

    void ManageMemVars()
    {
        int prevLevelTime = 0;
        uint8_t prevRound = 0;
        char mapNameRead[14] = "\0";
        std::string prevMapName = "";

        for (;;) {
            if (overrideMap) {
                if (mapName != prevMapName) {
                    mapChanged = true;
                    prevMapName = mapName;
                    static std::vector<std::string> craftMaps = { "zm_zod", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_tomb" };
                    if (std::find(craftMaps.begin(), craftMaps.end(), mapName) != craftMaps.end()) {
                        validCraftMap = true;
                    }
                    else {
                        validCraftMap = false;
                    }
                }
                Sleep(50);
                continue;
            }

            if (!injected) {
                Sleep(50);
                continue;
            }

            if (pHandle != NULL) {
                if (!ReadProcessMemory(pHandle, levelTimeAddress, &levelTime, sizeof(levelTime), NULL) ||
                    !ReadProcessMemory(pHandle, roundAddress, &round, sizeof(round), NULL) ||
                    !ReadProcessMemory(pHandle, mapNameAddress, &mapNameRead, sizeof(mapNameRead), NULL)) {
                    pID = 0;
                    pBaseAddress = NULL;
                    pHandle = NULL;
                    levelTime = 0;
                    round = 0;
                    injected = false;
                    validMap = false;
                    validCraftMap = false;
                    std::memset(mapNameRead, '\0', sizeof(mapNameRead));
                    BO3PracticeTool::OnGameShutdown();
                    LOG_DEBUG("{} Shutdown", pName);
                    continue;
                }
            }
            mapName = mapNameRead;

            if (validMap && mapName.empty() || mapName == "core_frontend") {
                validMap = false;
                validCraftMap = false;
            }
            else if (pHandle != NULL && !(mapName.empty() || mapName == "core_frontend")) {
                validMap = true;
            }

            if (mapName != prevMapName) {
                mapChanged = true;
                static std::vector<std::string> craftMaps = { "zm_zod", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_tomb" };
                if (std::find(craftMaps.begin(), craftMaps.end(), mapName) != craftMaps.end()) {
                    validCraftMap = true;
                }
                else {
                    validCraftMap = false;
                }
            }
            if (levelTime < prevLevelTime && validMap) {
                mapRestarted = true;
            }

            prevLevelTime = levelTime;
            prevRound = round;
            prevMapName = mapName;

            Sleep(50);
        }
    }
}