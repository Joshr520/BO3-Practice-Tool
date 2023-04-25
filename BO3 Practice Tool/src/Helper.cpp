#include "Helper.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "PlayerOptions.h"
#include "ZombieOptions.h"
#include "PowerupOptions.h"
#include "Craftables.h"
#include "Resources.h"
#include "GUIWindow.h"
#include "../Fonts/Icons.h"
#include "dirent.h"
#include <algorithm>
#include <direct.h>
#include <fstream>
#include <thread>

using namespace ImageHelp;
using namespace BGB;
using namespace Weapons;
using namespace Perks;
using namespace PlayerOptions;
using namespace ZombieOptions;
using namespace PowerupOptions;
using namespace Craftables;
using namespace ZombieCalc;
using namespace GKValveSolver;
using namespace IceCodePractice;

std::string ToLower(const char* str, int length);
void LogFile(std::string text);
void WaitToKillCompiler(PROCESS_INFORMATION lpExecInfo);

namespace GUIWindow
{
    void InitVariables()
    {
        InitBGBDescriptions();
        InitClassicGumsList();
        InitMegaGumsList();
        LoadGumProfiles();
        InitWeaponsList();
        InitPerksList();
        InitPowerupList();
        InitCraftablesList();
        InitIceCodePairs();
        zombiesForRound = GetZombieCountForRound(1, 1);
        specialZombiesForRound = GetZombieCountForRound(1, 1);
        hordesForRound = zombiesForRound / 24.0f;
        zombiesUpToRound = GetZombiesUpToRound(1, 1);
        zombieHealthForRound = GetZombieHealthForRound(1);
        zombieSpawnRate = GetZombieSpawnRateForRound(1, 1);
        roundTime = CalcRoundTime(1, 1);
        expectedRoundTime = CalcRoundTime(1, 1, corpseDelay[playerCount - 1][roundNumber - 1]);
        specialEnemiesRoundTime = CalcRoundTime(roundNumber, playerCount, corpseDelay[playerCount - 1][roundNumber - 1], true);
        soeExpectedRoundTime = SpecialRoundTime(1, 1, corpseDelay[playerCount - 1][roundNumber - 1], true);
        moonExpectedRoundTime = SpecialRoundTime(1, 1, corpseDelay[playerCount - 1][roundNumber - 1], false);
        customCalcExpectedRoundTime = CustomRoundTime(1, 1, 0);
        zombieSpawnRateStr = ParseTimeFromMilli(zombieSpawnRate);
        meatballDelay = "Meatball Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Meatballs"));
        dogDelay = "Dog Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
        spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
        manglerDelay = "Mangler Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Manglers"));
        valkDelay = "Valk Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Valks"));
        furyDelay = "Fury Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Furys"));
        keeperDelay = "Keeper Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Keepers"));
        CalcLockdownTime(1, 1);
        InitValveSolutions();
        CalcValveProbabilities();
    }

    void DummySpace(float x, float y)
    {
        ImGui::Dummy(ImVec2(x, y));
    }

    void SetToggleButtonColor(bool active)
    {
        if (!active)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 128, 0, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 128, 0, 255));
        }
    }

    bool CreateButton(std::string name, ImVec2 size, bool* value, bool toggle, ImVec4 color, bool inGame)
    {
        if (toggle)
            SetToggleButtonColor(*value);
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(color.x, color.y, color.z, color.w));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(color.x, color.y, color.z, color.w));
        }
        if (ImGui::ButtonEx(name.c_str(), size))
        {
            if (!inGame || inGame && appStatus == "Status: Active")
            {
                if (toggle)
                    *value = !*value;
            }
            ImGui::PopStyleColor(2);
            return 1;
        }

        ImGui::PopStyleColor(2);
        return 0;
    }

    void FakeButton(std::string name, ImVec2 size, ImVec4 color)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(color.x, color.y, color.z, color.w));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(color.x, color.y, color.z, color.w));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(color.x, color.y, color.z, color.w));
        ImGui::Button(name.c_str(), size);
        ImGui::PopStyleColor(3);
    }

    bool CreateListBox(const char* name, std::vector<std::string> items, int& currentItem, ImVec2 size)
    {
        int prevItem = currentItem;
        if (ImGui::ListBoxHeader(name, size))
        {
            for (int i = 0; i < items.size(); i++)
            {
                const bool is_selected = (currentItem == i);
                if (ImGui::Selectable(items[i].c_str(), is_selected))
                    currentItem = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::ListBoxFooter();
            if (prevItem != currentItem)
                return true;
        }
        return false;
    }

    void HelpMarker(const char* text)
    {
        ImGui::TextDisabled(ICON_FA_CIRCLE_QUESTION);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(text);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    bool CreateGumImages(std::vector<int>& gumArr, ImVec2 imgSize, int numOnLine, const char* type, std::function<void(int input)> funcOnPress)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(25, 100, 128, 100));
        float leftPos = ImGui::GetCursorPosX();
        if (numOnLine < 1)
            numOnLine = 1;
        else if (numOnLine > (int)gumArr.size())
            numOnLine = (int)gumArr.size();

        for (int i = 0; i < gumArr.size(); i++)
        {
            if (ImGui::ImageButton(bgbImgList[gumArr[i]].imgTexture, imgSize))
            {
                if (!strcmp(type, "Selection"))
                    funcOnPress(i);
                else
                    funcOnPress(bgbImgList[gumArr[i]].index - 1);
                ImGui::PopStyleColor(2);
                return 1;
            }
            if (ImGui::IsItemHovered())
                gumContextIndex = bgbImgList[gumArr[i]].index - 1;
            if ((i + 1) % numOnLine != 0)
                ImGui::SameLine();
            else
                ImGui::SetCursorPosX(leftPos);
        }

        ImGui::PopStyleColor(2);
        return 0;
    }

    void SwapGumSelection(int newGum, int gumSlot)
    {
        for (int i = 0; i < gumPresets[currentPreset].presetGums.size(); i++)
        {
            if (gumPresets[currentPreset].presetGums[i] == newGum)
            {
                gumPresets[currentPreset].presetGums[i] = gumPresets[currentPreset].presetGums[gumSlot];
                break;
            }
        }
        gumPresets[currentPreset].presetGums[gumSlot] = newGum;
        WriteGumPreset(gumPresets[currentPreset].presetGums);
    }

    std::vector<int> GumSearch(std::vector<int> inGumArr, char searchText[64])
    {
        if (!strcmp(searchText, ""))
            return inGumArr;
        std::vector<int> outGumArr = { };
        for (int i = 0; i < inGumArr.size(); i++)
        {
            std::string inGumLower = ToLower(bgbImgList[inGumArr[i]].imgRelativePath.c_str(), (int)bgbImgList[inGumArr[i]].imgRelativePath.size());
            std::string searchTextLower = ToLower(searchText, (int)strlen(searchText));
            if (inGumLower.substr(0, searchTextLower.size()) == searchTextLower)
                outGumArr.push_back(inGumArr[i]);
        }
        return outGumArr;
    }

    bool DoesPathExist(const std::string& s)
    {
        struct stat buffer;
        return (stat(s.c_str(), &buffer) == 0);
    }

    void VerifyFileStructure()
    {
        std::string practiceToolDirectory = bo3Directory + "\\Practice Tool";
        std::string gscDirectory = practiceToolDirectory + "\\GSC";
        std::string settingsFolder = practiceToolDirectory + "\\Settings";
        if (!DoesPathExist(practiceToolDirectory))
            int success = _mkdir(practiceToolDirectory.c_str());
        if (!DoesPathExist(gscDirectory))
            int success = _mkdir(gscDirectory.c_str());
        if (!DoesPathExist(settingsFolder))
            int success = _mkdir(settingsFolder.c_str());
        if (!DoesPathExist(practiceToolDirectory + "\\Tool-Game Interface.txt"))
        {
            std::ofstream outFile(practiceToolDirectory + "\\Tool-Game Interface.txt");
            outFile.close();
        }
        if (!DoesPathExist(practiceToolDirectory + "\\Game-Tool Interface.txt"))
        {
            std::ofstream outFile(practiceToolDirectory + "\\Game-Tool Interface.txt");
            outFile.close();
        }
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Active Gum Preset.txt"))
        {
            std::ofstream outFile(practiceToolDirectory + "\\Settings\\Active Gum Preset.txt");
            outFile.close();
        }
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Practice Presets.txt"))
        {
            std::ofstream outFile(practiceToolDirectory + "\\Settings\\Practice Presets.txt");
            outFile.close();
        }
        if (DoesPathExist("\\GSC"))
        {
            std::string startDirectory = "\\GSC";
            DIR* dir;
            struct dirent* ent;
            if ((dir = opendir(startDirectory.c_str())) != NULL)
            {
                while ((ent = readdir(dir)) != NULL)
                {
                    if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                        continue;
                    std::string name = ent->d_name;
                    std::string oldFile = startDirectory + "\\" + name;
                    std::string newFile = gscDirectory + "\\" + name;
                    if (MoveFileEx(oldFile.c_str(), newFile.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING) == 0)
                    {
                        LogFile("Move " + oldFile + " To " + newFile + " Failed");
                        LogFile("Error Code: " + std::to_string(GetLastError()));
                        if (remove(oldFile.c_str()) < 0)
                        {
                            LogFile("Couldn't Remove File: " + oldFile);
                            LogFile("Error Code: " + errno);
                        }
                    }
                }
                closedir(dir);
            }
            if (_rmdir(startDirectory.c_str()) < 0)
            {
                LogFile("Couldn't Remove GSC Directory");
                LogFile("Error Code: " + errno);
            }
        }
    }

    void WritePracticePatches(int patch[8])
    {
        std::string outData;
        for (int i = 0; i < 8; i++)
        {
            outData.append(std::to_string(patch[i] - 1) + ",");
        }
        outData.replace(outData.size() - 1, 1, "");
        std::ofstream outFile(bo3Directory + "\\Practice Tool\\Settings\\Practice Presets.txt");
        outFile << outData;
        outFile.close();
    }

    void NotifyGame(std::vector<int> passList)
    {
        if (appStatus == "Status: Inactive")
            return;
        std::string outData;
        for (int i = 0; i < passList.size(); i++)
        {
            outData.append(std::to_string(passList[i]) + ",");
        }
        outData.replace(outData.size() - 1, 1, "");
        std::ofstream outFile(bo3Directory + "\\Practice Tool\\Tool-Game Interface.txt");
        outFile << outData;
        outFile.close();
    }

    void InjectTool(bool enable)
    {
        std::string compiler = "\"E:\\Steam\\steamapps\\common\\Call of Duty Black Ops III\\Practice Tool\\GSC\\DebugCompiler.exe\"";
        std::string gsc = "\"E:\\Steam\\steamapps\\common\\Call of Duty Black Ops III\\Practice Tool\\GSC";
        std::string ptPass = compiler + " --inject " + gsc + "\\Practice Tool.gsc\" T7 scripts/shared/duplicaterender_mgr.gsc";
        std::string nmPass = compiler + " --inject " + gsc + "\\No Mods.gsc\" T7 scripts/shared/duplicaterender_mgr.gsc";

        char args[1028];

        if (enable)
            strcpy_s(args, ptPass.c_str());
        else
            strcpy_s(args, nmPass.c_str());

        STARTUPINFO startupInfo = { sizeof(startupInfo) };
        PROCESS_INFORMATION processInfo;
        if (CreateProcess(NULL, args, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo))
        {
            std::thread checkProc(WaitToKillCompiler, processInfo);
            checkProc.detach();
        }
        else
            LogFile("Error Code: " + std::to_string(GetLastError()));

    }

    std::vector<int> GetWeaponIndex(std::string currentMap, std::string weaponSelectName)
    {
        std::vector<std::string> weaponTypes = { "_ar", "_smg", "_lmg", "_shotgun", "_sniper", "_pistol", "_launcher", "_melee", "_special", "_equipment", "_hero" };
        int list = 0;
        for (const std::string& weaponType : weaponTypes)
        {
            int index = 0;
            for (const std::string& weapon : Weapons::weaponList[currentMap + weaponType])
            {
                if (weaponSelectName == weapon)
                {
                    return { list, index };
                }
                index++;
            }
            list++;
        }
        return { 0, 0 };
    }

    void ResetToggles()
    {
        godActive = false;
        infAmmoActive = false;
        zombiesIgnore = false;
        zombiesFreeze = false;
        zombiesSpawn = false;
        zombieSpeedWalk = false;
        zombieSpeedRun = false;
        zombieSpeedSprint = false;
    }
}

std::string ToLower(const char* str, int length)
{
    std::string newStr = str;
    for (int i = 0; i < length; i++)
    {
        newStr[i] = tolower(str[i]);
    }
    return newStr;
}

void LogFile(std::string text)
{
    std::ofstream logFile("\\log.txt", std::ios::app);
    logFile << text << "\n";
    logFile.close();
}

void WaitToKillCompiler(PROCESS_INFORMATION processInfo)
{
    Sleep(3000);

    TerminateProcess(processInfo.hProcess, 0);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
}