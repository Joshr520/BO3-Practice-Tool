#include "ToolData.h"
#include "GlobalData.h"
#include "Fonts/Icons.h"
#include "Walnut/Random.h"
#include "Walnut/FileFormats/xml.h"
#include "Walnut/FileFormats/json.h"
#include "Keybinds.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <miniz/miniz.h>

#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace BO3PT;

DWORD WaitToKillCompiler(PROCESS_INFORMATION processInfo)
{
    DWORD exitCode = 0;
    DWORD result = WaitForSingleObject(processInfo.hProcess, 3000);
    if (result == WAIT_TIMEOUT)
    {
        LogFile("Killing compiler for timeout");
        TerminateProcess(processInfo.hProcess, 0);
    }
    else if (result == WAIT_OBJECT_0)
    {
        if (!GetExitCodeProcess(processInfo.hProcess, &exitCode))
        {
            LogFile("GetExitCodeProcess failed with error code: " + std::to_string(GetLastError()));
        }
    }
    else
    {
        LogFile("WaitForSingleObject failed with error code: " + std::to_string(GetLastError()));
    }
    return exitCode;
}

static size_t WriteToFile(char* ptr, size_t size, size_t nmemb, void* f)
{
    FILE* file = (FILE*)f;
    return fwrite(ptr, size, nmemb, file);
}

namespace BO3PT
{
#pragma region Craftables
    void InitCraftablesList()
    {
        std::unordered_map<int, std::vector<std::string>> zodCraftables;
        zodCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        zodCraftables.insert({ 1, { "Fuse 1", "Fuse 2", "Fuse 3" } });
        zodCraftables.insert({ 2, { "Heart", "Tentacle", "Xenomatter" } });
        zodCraftables.insert({ 3, { "Boxer", "Detective", "Femme", "Magician" } });
        zodCraftables.insert({ 4, { "Boxer", "Detective", "Femme", "Magician" } });

        std::unordered_map<int, std::string> zodCraftNames;
        zodCraftNames.insert({ 0, "Shield" });
        zodCraftNames.insert({ 1, "Civil Protector" });
        zodCraftNames.insert({ 2, "ID Gun" });
        zodCraftNames.insert({ 3, "Mementos" });
        zodCraftNames.insert({ 4, "Gateworms" });

        std::unordered_map<int, int> zodCraftIndexes;
        zodCraftIndexes.insert({ 0, 0 });
        zodCraftIndexes.insert({ 1, 0 });
        zodCraftIndexes.insert({ 2, 0 });
        zodCraftIndexes.insert({ 3, 0 });
        zodCraftIndexes.insert({ 4, 0 });

        std::unordered_map<int, std::vector<std::string>> castleCraftables;
        castleCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        castleCraftables.insert({ 1, { "Body", "Guards", "Handle" } });

        std::unordered_map<int, std::string> castleCraftNames;
        castleCraftNames.insert({ 0, "Shield" });
        castleCraftNames.insert({ 1, "Rags" });

        std::unordered_map<int, int> castleCraftIndexes;
        castleCraftIndexes.insert({ 0, 0 });
        castleCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> islandCraftables;
        islandCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        islandCraftables.insert({ 1, { "Visor", "Filter", "Strap" } });

        std::unordered_map<int, std::string> islandCraftNames;
        islandCraftNames.insert({ 0, "Shield" });
        islandCraftNames.insert({ 1, "Gasmask" });

        std::unordered_map<int, int> islandCraftIndexes;
        islandCraftIndexes.insert({ 0, 0 });
        islandCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> stalingradCraftables;
        stalingradCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });

        std::unordered_map<int, std::string> stalingradCraftNames;
        stalingradCraftNames.insert({ 0, "Shield" });

        std::unordered_map<int, int> stalingradCraftIndexes;
        stalingradCraftIndexes.insert({ 0, 0 });
        stalingradCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> genesisCraftables;
        genesisCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        genesisCraftables.insert({ 1, { "Totem", "Head", "Gem" } });

        std::unordered_map<int, std::string> genesisCraftNames;
        genesisCraftNames.insert({ 0, "Shield" });
        genesisCraftNames.insert({ 1, "Keeper" });

        std::unordered_map<int, int> genesisCraftIndexes;
        genesisCraftIndexes.insert({ 0, 0 });
        genesisCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> tombCraftables;
        tombCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        tombCraftables.insert({ 1, { "Body", "Brain", "Engine" } });
        tombCraftables.insert({ 2, { "Vinyl Player", "Vinyl Master", "Vinyl Wind", "Vinyl Ice", "Vinyl Fire", "Vinyl Lightning" } });
        tombCraftables.insert({ 3, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });
        tombCraftables.insert({ 4, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });
        tombCraftables.insert({ 5, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });
        tombCraftables.insert({ 6, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });

        std::unordered_map<int, std::string> tombCraftNames;
        tombCraftNames.insert({ 0, "Shield" });
        tombCraftNames.insert({ 1, "Maxis Drone" });
        tombCraftNames.insert({ 2, "Gramophone" });
        tombCraftNames.insert({ 3, "Wind Staff" });
        tombCraftNames.insert({ 4, "Fire Staff" });
        tombCraftNames.insert({ 5, "Lightning Staff" });
        tombCraftNames.insert({ 6, "Ice Staff" });

        std::unordered_map<int, int> tombCraftIndexes;
        tombCraftIndexes.insert({ 0, 0 });
        tombCraftIndexes.insert({ 1, 0 });
        tombCraftIndexes.insert({ 2, 0 });
        tombCraftIndexes.insert({ 3, 0 });
        tombCraftIndexes.insert({ 4, 0 });
        tombCraftIndexes.insert({ 5, 0 });
        tombCraftIndexes.insert({ 6, 0 });

        craftList.insert({ "zm_zod", zodCraftables });
        craftList.insert({ "zm_castle", castleCraftables });
        craftList.insert({ "zm_island", islandCraftables });
        craftList.insert({ "zm_stalingrad", stalingradCraftables });
        craftList.insert({ "zm_genesis", genesisCraftables });
        craftList.insert({ "zm_tomb", tombCraftables });

        craftNames.insert({ "zm_zod", zodCraftNames });
        craftNames.insert({ "zm_castle", castleCraftNames });
        craftNames.insert({ "zm_island", islandCraftNames });
        craftNames.insert({ "zm_stalingrad", stalingradCraftNames });
        craftNames.insert({ "zm_genesis", genesisCraftNames });
        craftNames.insert({ "zm_tomb", tombCraftNames });

        craftComboIndexes.insert({ "zm_zod", zodCraftIndexes });
        craftComboIndexes.insert({ "zm_castle", castleCraftIndexes });
        craftComboIndexes.insert({ "zm_island", islandCraftIndexes });
        craftComboIndexes.insert({ "zm_stalingrad", stalingradCraftIndexes });
        craftComboIndexes.insert({ "zm_genesis", genesisCraftIndexes });
        craftComboIndexes.insert({ "zm_tomb", tombCraftIndexes });
    }
#pragma endregion

#pragma region GUIFunctions
    void InitVariables()
    {
        InitImages();
        InitHotKeyBinds();
        InitBGBDescriptions();
        InitClassicGumsList();
        InitMegaGumsList();
        LoadGumProfiles();
        LoadSplitPresets();
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

    void InitImages()
    {
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/Gum Images/Classics"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            bgbImgList.emplace_back(img);
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/Gum Images/Megas"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            bgbImgList.emplace_back(img);
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/Soe Code"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            codeImgList.emplace_back(img);
            soeCodeCombo.emplace_back(img->GetFilename());
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/GK Valve Solver"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            valveSolverImgList.emplace_back(img);
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/Ice Code"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            iceCodeImgList.insert({ img->GetFilename(), img });
        }
    }

    void HelpMarker(const std::string& text)
    {
        ImGui::TextDisabled(ICON_FA_CIRCLE_QUESTION);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextWrapped(text.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
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

    void FakeButton(const std::string& name, const ImVec2& size, const ImVec4& color)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(color.x, color.y, color.z, color.w));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(color.x, color.y, color.z, color.w));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(color.x, color.y, color.z, color.w));
        ImGui::Button(name.c_str(), size);
        ImGui::PopStyleColor(3);
    }

    void SwapGumSelection(int newGum, int gumSlot)
    {
        for (int i = 0; i < gumPresets[currentGumPreset].presetGums.size(); i++)
        {
            if (gumPresets[currentGumPreset].presetGums[i] == newGum)
            {
                gumPresets[currentGumPreset].presetGums[i] = gumPresets[currentGumPreset].presetGums[gumSlot];
                break;
            }
        }
        gumPresets[currentGumPreset].presetGums[gumSlot] = newGum;
        WriteGumPreset(gumPresets[currentGumPreset].presetGums);
    }

    void VerifyFileStructure()
    {
        std::string practiceToolDirectory = bo3Directory + "\\Practice Tool";
        std::string gscDirectory = practiceToolDirectory + "\\GSC";
        std::string settingsFolder = practiceToolDirectory + "\\Settings";
        if (!DoesPathExist(practiceToolDirectory))
            std::filesystem::create_directory(practiceToolDirectory);
        if (!DoesPathExist(gscDirectory))
            std::filesystem::create_directory(gscDirectory);
        if (!DoesPathExist(settingsFolder))
            std::filesystem::create_directory(settingsFolder);
        if (!DoesPathExist(practiceToolDirectory + "\\Tool-Game Interface.txt"))
            std::ofstream(practiceToolDirectory + "\\Tool-Game Interface.txt");
        if (!DoesPathExist(practiceToolDirectory + "\\Game-Tool Interface.txt"))
            std::ofstream(practiceToolDirectory + "\\Game-Tool Interface.txt");
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Active Gum Preset.txt"))
            std::ofstream(practiceToolDirectory + "\\Settings\\Active Gum Preset.txt");
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Active Autosplit Preset.json"))
            std::ofstream(practiceToolDirectory + "\\Settings\\Active Autosplit Preset.json");
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Practice Presets.txt"))
            std::ofstream(practiceToolDirectory + "\\Settings\\Practice Presets.txt");
        if (!DoesPathExist(selfDirectory + "\\bindings.json"))
            Walnut::JSONBuilder::WriteEmpty(selfDirectory + "\\bindings.json");
        if (!DoesPathExist(selfDirectory + "\\Settings"))
            std::filesystem::create_directory(selfDirectory + "\\Settings");
        if (!DoesPathExist(selfDirectory + "\\Settings\\Autosplits"))
            std::filesystem::create_directory(selfDirectory + "\\Settings\\Autosplits");
        if (!DoesPathExist(selfDirectory + "\\Settings\\Gum Profiles"))
            std::filesystem::create_directory(selfDirectory + "\\Settings\\Gum Profiles");
        if (DoesPathExist(selfDirectory + "\\GSC"))
        {
            std::string startDirectory = selfDirectory + "\\GSC";
            for (const auto& entry : std::filesystem::directory_iterator(startDirectory))
            {
                if (entry.is_directory() || entry.path().filename().string() == "." || entry.path().filename().string() == "..")
                    continue;
                std::string name = entry.path().filename().string();
                std::string oldFile = entry.path().string();
                std::string newFile = gscDirectory + "\\" + name;
                if (!DoesPathExist(oldFile))
                    continue;
                try
                {
                    std::filesystem::rename(oldFile, newFile);
                }
                catch (const std::filesystem::filesystem_error& e)
                {
                    LogFile("Failed to move file " + oldFile + " to " + newFile);
                    LogFile("Error message: " + std::string(e.what()));
                    std::filesystem::remove(oldFile);
                }
            }
            if (!std::filesystem::remove(startDirectory))
                LogFile("Couldn't remove GSC directory with error code: " + std::error_code(errno, std::system_category()).message());
        }
    }

    void WritePracticePatches(const int patch[8])
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

    void NotifyGame(const std::vector<int>& passList)
    {
        // REPLACE WITH PIPE WHEN READY

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

    void InjectTool(bool enable, bool& injectResponse)
    {
        std::string gsc = bo3Directory + "/Practice Tool/GSC";
        std::string compiler = "\"" + gsc + "/DebugCompiler.exe\"";
        std::string ptPass = compiler + " --inject " + "\"" + gsc + "\\Practice Tool.gsc\" T7 scripts/shared/duplicaterender_mgr.gsc";
        std::string nmPass = compiler + " --inject " + "\"" + gsc + "\\No Mods.gsc\" T7 scripts/shared/duplicaterender_mgr.gsc";
        std::unordered_set<std::string_view> wantedFiles = { "DebugCompiler.exe", "DebugCompiler.exe.config", "External.dll", "Ionic.Zip.dll", "Irony.dll", "No Mods.gsc", "Practice Tool.gsc", "System.Buffers.dll", "System.Collections.Immutable.dll", "System.Memory.dll",
            "System.Numerics.Vectors.dll", "System.Reflection.Metadata.dll", "System.Runtime.CompilerServices.Unsafe.dll", "t7cinternal.dll", "T7CompilerLib.dll", "t8cinternal.dll", "T89CompilerLib.dll", "TreyarchCompiler.dll", "xdevkit.dll", "xdrpc.dll" };
        for (const auto& fileName : wantedFiles)
        {
            std::filesystem::path path(std::filesystem::path(gsc) / fileName);
            if (!std::filesystem::exists(path))
            {
                enabled = false;
                appStatus = "Status: Inactive";
                injectResponse = true;
                return;
            }
        }

        LPSTR args;
        if (enable)
            args = &ptPass[0];
        else
            args = &nmPass[0];

        STARTUPINFOA startupInfo = { sizeof(startupInfo) };
        PROCESS_INFORMATION processInfo = { 0 };
        if (CreateProcessA(NULL, args, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo))
        {
            DWORD exitCode = WaitToKillCompiler(processInfo);
            if (exitCode != 0)
                LogFile("Compiler process failed with error code: " + std::to_string(exitCode));
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }
        else
        {
            LogFile("Failed to inject tool with error code: " + std::to_string(GetLastError()));
            enabled = false;
            appStatus = "Status: Inactive";
        }
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
        timescaleInt = 1;
    }

    bool CreateButton(const std::string& name, const ImVec2& size, bool* value, bool toggle, const ImVec4& color, bool inGame)
    {
        if (toggle)
            SetToggleButtonColor(*value);
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(color.x, color.y, color.z, color.w));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(color.x, color.y, color.z, color.w));
        }
        if (ImGui::Button(name.c_str(), size))
        {
            if (!inGame || inGame && appStatus == "Status: Active")
            {
                if (toggle)
                    *value = !*value;
            }
            ImGui::PopStyleColor(2);
            return 1;
        }
        if (name == ICON_FA_ARROW_LEFT && ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            ImGui::PopStyleColor(2);
            return 1;
        }
        ImGui::PopStyleColor(2);
        return 0;
    }

    bool CreateListBox(const std::string& name, const std::vector<std::string>& items, int& currentItem, const ImVec2& size)
    {
        int prevItem = currentItem;
        if (ImGui::ListBoxHeader(name.c_str(), size))
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

    bool CreateGumImages(const std::vector<int>& gumArr, const ImVec2& imgSize, int numOnLine, const std::string& type, const std::function<void(int input)>& funcOnPress, int& outIndex)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(25, 100, 128, 100));
        float leftPos = ImGui::GetCursorPosX();
        if (numOnLine < 1)
            numOnLine = 1;
        else if (numOnLine > (int)gumArr.size())
            numOnLine = (int)gumArr.size();

        for (int i = 0; i < gumArr.size(); i++)
        {
            if (gumArr[i] < 0)
            {
                ImGui::PopStyleColor(2);
                return 0;
            }
            auto it = std::find(bgbImgList.begin(), bgbImgList.end(), bgbImgList[gumArr[i]]);
            int index = static_cast<int>(std::distance(bgbImgList.begin(), it));
            if (ImGui::ImageButton(bgbImgList[gumArr[i]]->GetDescriptorSet(), imgSize))
            {
                if (type == "Selection")
                    funcOnPress(i);
                else
                    funcOnPress(index);
                ImGui::PopStyleColor(2);
                return 1;
            }
            if (ImGui::IsItemHovered())
                outIndex = index;
            if ((i + 1) % numOnLine != 0)
                ImGui::SameLine();
            else
                ImGui::SetCursorPosX(leftPos);
        }

        ImGui::PopStyleColor(2);
        return 0;
    }

    bool DoesPathExist(const std::string_view& s)
    {
        return std::filesystem::exists(s);
    }

    bool CheckVersions(const std::string& newVersion, const std::string& oldVersion)
    {
        std::vector<int> newVersionParts;
        std::vector<int> oldVersionParts;

        std::istringstream newVersionStream(newVersion);
        std::istringstream oldVersionStream(oldVersion);
        std::string part;

        std::getline(newVersionStream, part, 'v');
        std::getline(oldVersionStream, part, 'v');

        while (std::getline(newVersionStream, part, '.'))
            newVersionParts.emplace_back(std::stoi(part));
        while (std::getline(oldVersionStream, part, '.'))
            oldVersionParts.emplace_back(std::stoi(part));

        if (newVersionParts.size() != 3 || oldVersionParts.size() != 3)
            return false;
        if (newVersionParts[0] > oldVersionParts[0])
            return true;
        else if (newVersionParts[0] == oldVersionParts[0])
        {
            if (newVersionParts[1] > oldVersionParts[1])
                return true;
            else if (newVersionParts[1] == oldVersionParts[1])
            {
                if (newVersionParts[2] > oldVersionParts[2])
                    return true;
            }
        }
        return false;
    }

    bool DownloadAndExtractZip(const std::unordered_set<std::string_view>& wantedFiles)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        CURL* curl = curl_easy_init();
        CURLcode res;
        FILE* file;
        std::string filename = "BO3 Practice Tool.zip";
        std::string ptexe;

        errno_t err = fopen_s(&file, filename.c_str(), "wb");
        if (err != 0)
        {
            char errorMsg[256];
            strerror_s(errorMsg, sizeof(errorMsg), err);
            LogFile("Opening file " + filename + " failed with error code: " + errorMsg);
            return 0;
        }
        curl_easy_setopt(curl, CURLOPT_URL, downloadURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFile);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        if (res != CURLE_OK)
        {
            LogFile("curl download failed with error code: " + res);
            return 0;
        }

        fclose(file);

        std::filesystem::path output_directory("./");
        mz_zip_archive zip_archive;
        mz_zip_zero_struct(&zip_archive);
        if (!mz_zip_reader_init_file(&zip_archive, filename.c_str(), 0))
        {
            LogFile("Failed to open zip file: " + filename);
            return 0;
        }

        int num_files = mz_zip_reader_get_num_files(&zip_archive);
        LogFile("Extracting " + num_files + std::string(" files from ") + filename + " to " + output_directory.string());

        for (int i = 0; i < num_files; i++)
        {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
            {
                LogFile("Failed to get file stat for index " + i);
                continue;
            }

            std::filesystem::path output_file_path = output_directory / file_stat.m_filename;

            std::string currentFile(file_stat.m_filename);
            bool wantedFileFound = false;
            for (const std::string_view& wantedFile : wantedFiles)
            {
                if (currentFile.find(wantedFile) != currentFile.npos)
                {
                    wantedFileFound = true;
                    break;
                }
            }

            if (!wantedFileFound)
                continue;

            LogFile("Extracting " + std::string(file_stat.m_filename) + " to " + output_file_path.string());

            std::filesystem::create_directories(output_file_path.parent_path());

            if (mz_zip_reader_is_file_a_directory(&zip_archive, i))
                std::filesystem::create_directory(output_file_path);
            else
            {
                if (currentFile == "BO3 Practice Tool.exe" && std::filesystem::exists(output_file_path))
                {
                    std::filesystem::rename(output_file_path, output_directory / "BO3 Practice Tool.old.exe");
                    ptexe = output_file_path.string();
                }
                else if (std::filesystem::exists(output_file_path))
                    std::filesystem::remove(output_file_path);
                if (!mz_zip_reader_extract_to_file(&zip_archive, i, output_file_path.generic_string().c_str(), 0))
                {
                    LogFile("Failed to extract file " + std::string(file_stat.m_filename));
                    return 0;
                }
            }
        }
        mz_zip_reader_end(&zip_archive);
        std::filesystem::remove(filename);

        if (!ptexe.empty())
        {
            STARTUPINFOA startupInfo = { sizeof(startupInfo) };
            PROCESS_INFORMATION processInfo = { 0 };
            LPSTR args = &ptexe[0];
            if (!CreateProcessA(NULL, args, NULL, NULL, TRUE, NULL, NULL, NULL, &startupInfo, &processInfo))
                LogFile("Failed to start new practice tool exe");
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
            done = 1;
        }
        return 1;
    }

    std::vector<int> GumSearch(const std::vector<int>& inGumArr, const std::string& searchText)
    {
        if (searchText.empty())
            return inGumArr;

        std::vector<int> outGumArr = { };
        outGumArr.reserve(inGumArr.size());

        std::string searchTextLower = searchText;
        std::transform(searchTextLower.begin(), searchTextLower.end(), searchTextLower.begin(), [](char c) { return std::tolower(c); });

        std::copy_if(inGumArr.begin(), inGumArr.end(), std::back_inserter(outGumArr), [&searchTextLower](int i)
            {
                std::string inGum = bgbImgList[i]->GetFilename();
                std::transform(inGum.begin(), inGum.end(), inGum.begin(), [](char c) { return std::tolower(c); });
                return inGum.find(searchTextLower) != inGum.npos;
            });

        return outGumArr;
    }

    std::vector<int> GetWeaponIndex(const std::string& currentMap, const std::string& weaponSelectName)
    {
        std::vector<std::string> weaponTypes = { "_ar", "_smg", "_lmg", "_shotgun", "_sniper", "_pistol", "_launcher", "_melee", "_special", "_equipment", "_hero" };
        int list = 0;
        for (const std::string& weaponType : weaponTypes)
        {
            int index = 0;
            for (const std::string& weapon : weaponList[currentMap + weaponType])
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
#pragma endregion

#pragma region Weapons
#pragma region WeaponDefs
#define SVG "SVG-100"
#define DRAKON "Drakon"
#define LOCUS "Locus"
#define BOOTLEGGER "Bootlegger"
#define VMP "VMP"
#define KUDA "Kuda"
#define VESPER "Vesper"
#define WEEVIL "Weevil"
#define PHARO "Pharo"
#define HG40 "HG 40"
#define RAZORBACK "Razorback"
#define PPSH "PPSh-41"
#define THOMPSON "M1927"
#define AK74 "AK-74u"
#define MP40 "MP40"
#define XM "XM-53"
#define L4 "L4 Siege"
#define BRECCI "205 Brecci"
#define KRM "KRM-262"
#define ARGUS "Argus"
#define HAYMAKER "Haymaker 12"
#define BANSHII "Banshii"
#define LCAR "L-CAR 9"
#define RK5 "RK5"
#define BLOODHOUND "Bloodhound"
#define MR6 "MR6"
#define MARSHAL "Marshal 16 Dual-Wield"
#define RIFT "Rift E9"
#define M1911 "1911"
#define MAUSER "Mauser C96"
#define GORGON "Gorgon"
#define BRM "BRM"
#define DREDGE "48 Dredge"
#define DINGO "Dingo"
#define RPK "RPK"
#define MG08 "MG-08/15"
#define KN "KN-44"
#define SHIEVA "Shieva"
#define M8 "M8A7"
#define MANOWAR "Man-O-War"
#define HVK "HVK-30"
#define ICR "ICR-1"
#define GARAND "MX Garand"
#define FFAR "FFAR"
#define PEACEKEEPER "Peacekeeper MK2"
#define STG "StG-44"
#define M14 "M14"
#define M16 "M16"
#define GALIL "Galil"
#define SHADOWCLAW "NX ShadowClaw Dual Wield"
#define APOTHSERVANT "Apothicon Servant"
#define WAFFE "Wunderwaffe DG-2"
#define RAYGUN "Ray Gun"
#define RAYGUNMK2 "Ray Gun Mark II"
#define RAYGUNMK3 "GKZ-45 Mk3"
#define TGUN "Thundergun"
#define BABYGUN "31-79 JGb215"
#define BOW "Default Bow"
#define LIGHTNINGBOW "Lightning Bow"
#define FIREBOW "Fire Bow"
#define VOIDBOW "Void Bow"
#define WOLFBOW "Wolf Bow"
#define KT4 "KT-4"
#define WAVEGUN "Zap Gun Dual Wield"
#define ICESTAFF "Staff of Ice"
#define LIGHTNINGSTAFF "Staff of Lightning"
#define FIRESTAFF "Staff of Fire"
#define WINDSTAFF "Staff of Wind"
#define BOWIEKNIFE "Bowie Knife"
#define PLUNGER "Plunger"
#define WRENCH "Wrench"
#define SWORD "Fury's Song"
#define AXE "Slash N' Burn"
#define DAGGER "Malice"
#define KATANA "Path of Sorrows"
#define MACE "Skull Splitter"
#define NUNCHUCKS "Nunchucks"
#define IMPROVISE "Buzz Cut"
#define BONEGLASS "Nightbreaker"
#define SICKLE "Sickle"
#define GRENADES "Frag Grenades"
#define ARNIE "Li'l Arnie"
#define MONKEY "Cymbal Monkey"
#define DOLLS "Matryoshka Doll"
#define GERSH "Gersh Device"
#define QED "Quantum Entanglement Device"
#define GSTRIKE "G Strikes"
#define TRIPMINE "Trip Mine"
#define DSTRIKE "Dragon Strike"
#define SHIELD "Shield"
#define GASMASK "P.E.S."
#define HACKER "Hacker"
#define GLAIVE "Keeper Sword"
#define RAGS "Ragnarok DG-4"
#define SKULL "Skull of Nan Sapwe"
#define GAUNTLET "Gauntlet of Siegfried"
#define ANNIHILATOR "Annihilator"
#pragma endregion
    void InitWeaponsList()
    {
        std::vector<std::string> zodSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> zodSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, BOOTLEGGER }; std::vector<std::string> zodLauncher = { XM }; std::vector<std::string> zodShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> zodPistol = { BLOODHOUND, RK5, LCAR }; std::vector<std::string> zodLMG = { DINGO, DREDGE, BRM, GORGON }; std::vector<std::string> zodAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN }; std::vector<std::string> zodSpecial = { APOTHSERVANT, RAYGUN }; std::vector<std::string> zodMelee = { BOWIEKNIFE }; std::vector<std::string> zodEquipment = { ARNIE, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> zodHero = { GLAIVE };
        std::vector<std::string> factorySniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> factorySMG = { PHARO, WEEVIL, VESPER, KUDA, VMP }; std::vector<std::string> factoryLauncher = { XM }; std::vector<std::string> factoryShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> factoryPistol = { MR6, RK5, LCAR }; std::vector<std::string> factoryLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> factoryAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN }; std::vector<std::string> factorySpecial = { WAFFE, RAYGUN }; std::vector<std::string> factoryMelee = { BOWIEKNIFE }; std::vector<std::string> factoryEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> factoryHero = { ANNIHILATOR };
        std::vector<std::string> castleSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> castleSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP }; std::vector<std::string> castleLauncher = { XM }; std::vector<std::string> castleShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> castlePistol = { MR6, RK5, LCAR }; std::vector<std::string> castleLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> castleAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN }; std::vector<std::string> castleSpecial = { BOW, LIGHTNINGBOW, FIREBOW, VOIDBOW, WOLFBOW, RAYGUN }; std::vector<std::string> castleMelee = { BOWIEKNIFE, PLUNGER }; std::vector<std::string> castleEquipment = { MONKEY, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> castleHero = { RAGS };
        std::vector<std::string> islandSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> islandSMG = { PHARO, WEEVIL, VESPER, RAZORBACK, HG40, KUDA, VMP }; std::vector<std::string> islandLauncher = { XM }; std::vector<std::string> islandShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> islandPistol = { MR6, RK5, LCAR, MARSHAL }; std::vector<std::string> islandLMG = { DINGO, DREDGE, BRM, GORGON }; std::vector<std::string> islandAR = { ICR, HVK, MANOWAR, GARAND, M8, SHIEVA, KN }; std::vector<std::string> islandSpecial = { KT4, RAYGUN }; std::vector<std::string> islandMelee = { BOWIEKNIFE }; std::vector<std::string> islandEquipment = { MONKEY, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> islandHero = { SKULL };
        std::vector<std::string> stalingradSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> stalingradSMG = { PHARO, WEEVIL, VESPER, HG40, PPSH, KUDA, VMP }; std::vector<std::string> stalingradLauncher = { L4, XM }; std::vector<std::string> stalingradShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> stalingradPistol = { MR6, RK5, LCAR }; std::vector<std::string> stalingradLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> stalingradAR = { ICR, HVK, MANOWAR, FFAR, GARAND, M8, SHIEVA, KN }; std::vector<std::string> stalingradSpecial = { RAYGUNMK3, RAYGUN, SHADOWCLAW }; std::vector<std::string> stalingradMelee = { BOWIEKNIFE, DAGGER, AXE, SWORD, WRENCH }; std::vector<std::string> stalingradEquipment = { DSTRIKE, MONKEY, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> stalingradHero = { GAUNTLET };
        std::vector<std::string> genesisSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> genesisSMG = { PHARO, WEEVIL, VESPER, KUDA, THOMPSON, VMP }; std::vector<std::string> genesisLauncher = { XM }; std::vector<std::string> genesisShotgun = { BANSHII, HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> genesisPistol = { MR6, RIFT, RK5, LCAR }; std::vector<std::string> genesisLMG = { DINGO, DREDGE, BRM, GORGON }; std::vector<std::string> genesisAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, PEACEKEEPER }; std::vector<std::string> genesisSpecial = { TGUN, APOTHSERVANT, RAYGUN }; std::vector<std::string> genesisMelee = { BOWIEKNIFE, BONEGLASS, IMPROVISE, NUNCHUCKS, MACE, KATANA }; std::vector<std::string> genesisEquipment = { ARNIE, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> genesisHero = { RAGS };
        std::vector<std::string> prototypeSniper = { LOCUS, SVG }; std::vector<std::string> prototypeSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, MP40, BOOTLEGGER, AK74 }; std::vector<std::string> prototypeLauncher = { XM }; std::vector<std::string> prototypeShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> prototypePistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> prototypeLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> prototypeAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, GARAND, STG }; std::vector<std::string> prototypeSpecial = { RAYGUNMK2, RAYGUN }; std::vector<std::string> prototypeMelee = { BOWIEKNIFE }; std::vector<std::string> prototypeEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> prototypeHero = { ANNIHILATOR };
        std::vector<std::string> asylumSniper = { LOCUS, SVG }; std::vector<std::string> asylumSMG = { PHARO, WEEVIL, VESPER, PPSH, KUDA, BOOTLEGGER, VMP, MP40, AK74 }; std::vector<std::string> asylumLauncher = { XM }; std::vector<std::string> asylumShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> asylumPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> asylumLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> asylumAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, GARAND, STG }; std::vector<std::string> asylumSpecial = { WAFFE, RAYGUNMK2, RAYGUN }; std::vector<std::string> asylumMelee = { BOWIEKNIFE }; std::vector<std::string> asylumEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> asylumHero = { ANNIHILATOR };
        std::vector<std::string> sumpfSniper = { LOCUS, SVG }; std::vector<std::string> sumpfSMG = { PHARO, WEEVIL, VESPER, PPSH, KUDA, BOOTLEGGER, VMP, MP40, AK74 }; std::vector<std::string> sumpfLauncher = { XM }; std::vector<std::string> sumpfShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> sumpfPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> sumpfLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> sumpfAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, GARAND, STG }; std::vector<std::string> sumpfSpecial = { WAFFE, RAYGUNMK2, RAYGUN }; std::vector<std::string> sumpfMelee = { BOWIEKNIFE }; std::vector<std::string> sumpfEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> sumpfHero = { ANNIHILATOR };
        std::vector<std::string> theaterSniper = { DRAKON, SVG }; std::vector<std::string> theaterSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, MP40, AK74 }; std::vector<std::string> theaterLauncher = { XM }; std::vector<std::string> theaterShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> theaterPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> theaterLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> theaterAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> theaterSpecial = { TGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> theaterMelee = { BOWIEKNIFE }; std::vector<std::string> theaterEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> theaterHero = { ANNIHILATOR };
        std::vector<std::string> cosmodromeSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> cosmodromeSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, AK74 }; std::vector<std::string> cosmodromeLauncher = { L4, XM }; std::vector<std::string> cosmodromeShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> cosmodromePistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> cosmodromeLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> cosmodromeAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> cosmodromeSpecial = { TGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> cosmodromeMelee = { SICKLE }; std::vector<std::string> cosmodromeEquipment = { GERSH, DOLLS, TRIPMINE, GRENADES }; std::vector<std::string> cosmodromeHero = { ANNIHILATOR };
        std::vector<std::string> templeSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> templeSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, AK74 }; std::vector<std::string> templeLauncher = { L4, XM }; std::vector<std::string> templeShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> templePistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> templeLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> templeAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> templeSpecial = { BABYGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> templeMelee = { BOWIEKNIFE }; std::vector<std::string> templeEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> templeHero = { ANNIHILATOR };
        std::vector<std::string> moonSniper = { LOCUS, DRAKON }; std::vector<std::string> moonSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, AK74 }; std::vector<std::string> moonLauncher = { L4, XM }; std::vector<std::string> moonShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> moonPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> moonLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> moonAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> moonSpecial = { WAVEGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> moonMelee = { BOWIEKNIFE }; std::vector<std::string> moonEquipment = { GASMASK, HACKER, GERSH, QED, TRIPMINE, GRENADES }; std::vector<std::string> moonHero = { ANNIHILATOR };
        std::vector<std::string> tombSniper = { LOCUS, DRAKON }; std::vector<std::string> tombSMG = { PHARO, WEEVIL, VESPER, MP40, KUDA, THOMPSON, VMP, AK74 }; std::vector<std::string> tombLauncher = { L4, XM }; std::vector<std::string> tombShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> tombPistol = { RK5, MAUSER, LCAR }; std::vector<std::string> tombLMG = { DINGO, DREDGE, BRM, MG08, GORGON, RPK }; std::vector<std::string> tombAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, STG, M14 }; std::vector<std::string> tombSpecial = { ICESTAFF, LIGHTNINGSTAFF, FIRESTAFF, WINDSTAFF, RAYGUNMK2, RAYGUN }; std::vector<std::string> tombMelee = { BOWIEKNIFE }; std::vector<std::string> tombEquipment = { MONKEY, GSTRIKE, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> tombHero = { ANNIHILATOR };
        weaponList.insert({ "zm_zod_sniper", zodSniper }); weaponList.insert({ "zm_zod_smg", zodSMG }); weaponList.insert({ "zm_zod_launcher", zodLauncher }); weaponList.insert({ "zm_zod_shotgun", zodShotgun }); weaponList.insert({ "zm_zod_pistol", zodPistol }); weaponList.insert({ "zm_zod_lmg", zodLMG }); weaponList.insert({ "zm_zod_ar", zodAR }); weaponList.insert({ "zm_zod_special", zodSpecial }); weaponList.insert({ "zm_zod_melee", zodMelee }); weaponList.insert({ "zm_zod_equipment", zodEquipment }); weaponList.insert({ "zm_zod_hero", zodHero });
        weaponList.insert({ "zm_factory_sniper", factorySniper }); weaponList.insert({ "zm_factory_smg", factorySMG }); weaponList.insert({ "zm_factory_launcher", factoryLauncher }); weaponList.insert({ "zm_factory_shotgun", factoryShotgun }); weaponList.insert({ "zm_factory_pistol", factoryPistol }); weaponList.insert({ "zm_factory_lmg", factoryLMG }); weaponList.insert({ "zm_factory_ar", factoryAR }); weaponList.insert({ "zm_factory_special", factorySpecial }); weaponList.insert({ "zm_factory_melee", factoryMelee }); weaponList.insert({ "zm_factory_equipment", factoryEquipment }); weaponList.insert({ "zm_factory_hero", factoryHero });
        weaponList.insert({ "zm_castle_sniper", castleSniper }); weaponList.insert({ "zm_castle_smg", castleSMG }); weaponList.insert({ "zm_castle_launcher", castleLauncher }); weaponList.insert({ "zm_castle_shotgun", castleShotgun }); weaponList.insert({ "zm_castle_pistol", castlePistol }); weaponList.insert({ "zm_castle_lmg", castleLMG }); weaponList.insert({ "zm_castle_ar", castleAR }); weaponList.insert({ "zm_castle_special", castleSpecial }); weaponList.insert({ "zm_castle_melee", castleMelee }); weaponList.insert({ "zm_castle_equipment", castleEquipment }); weaponList.insert({ "zm_castle_hero", castleHero });
        weaponList.insert({ "zm_island_sniper", islandSniper }); weaponList.insert({ "zm_island_smg", islandSMG }); weaponList.insert({ "zm_island_launcher", islandLauncher }); weaponList.insert({ "zm_island_shotgun", islandShotgun }); weaponList.insert({ "zm_island_pistol", islandPistol }); weaponList.insert({ "zm_island_lmg", islandLMG }); weaponList.insert({ "zm_island_ar", islandAR }); weaponList.insert({ "zm_island_special", islandSpecial }); weaponList.insert({ "zm_island_melee", islandMelee }); weaponList.insert({ "zm_island_equipment", islandEquipment }); weaponList.insert({ "zm_island_hero", islandHero });
        weaponList.insert({ "zm_stalingrad_sniper", stalingradSniper }); weaponList.insert({ "zm_stalingrad_smg", stalingradSMG }); weaponList.insert({ "zm_stalingrad_launcher", stalingradLauncher }); weaponList.insert({ "zm_stalingrad_shotgun", stalingradShotgun }); weaponList.insert({ "zm_stalingrad_pistol", stalingradPistol }); weaponList.insert({ "zm_stalingrad_lmg", stalingradLMG }); weaponList.insert({ "zm_stalingrad_ar", stalingradAR }); weaponList.insert({ "zm_stalingrad_special", stalingradSpecial }); weaponList.insert({ "zm_stalingrad_melee", stalingradMelee }); weaponList.insert({ "zm_stalingrad_equipment", stalingradEquipment }); weaponList.insert({ "zm_stalingrad_hero", stalingradHero });
        weaponList.insert({ "zm_genesis_sniper", genesisSniper }); weaponList.insert({ "zm_genesis_smg", genesisSMG }); weaponList.insert({ "zm_genesis_launcher", genesisLauncher }); weaponList.insert({ "zm_genesis_shotgun", genesisShotgun }); weaponList.insert({ "zm_genesis_pistol", genesisPistol }); weaponList.insert({ "zm_genesis_lmg", genesisLMG }); weaponList.insert({ "zm_genesis_ar", genesisAR }); weaponList.insert({ "zm_genesis_special", genesisSpecial }); weaponList.insert({ "zm_genesis_melee", genesisMelee }); weaponList.insert({ "zm_genesis_equipment", genesisEquipment }); weaponList.insert({ "zm_genesis_hero", genesisHero });
        weaponList.insert({ "zm_prototype_sniper", prototypeSniper }); weaponList.insert({ "zm_prototype_smg", prototypeSMG }); weaponList.insert({ "zm_prototype_launcher", prototypeLauncher }); weaponList.insert({ "zm_prototype_shotgun", prototypeShotgun }); weaponList.insert({ "zm_prototype_pistol", prototypePistol }); weaponList.insert({ "zm_prototype_lmg", prototypeLMG }); weaponList.insert({ "zm_prototype_ar", prototypeAR }); weaponList.insert({ "zm_prototype_special", prototypeSpecial }); weaponList.insert({ "zm_prototype_melee", prototypeMelee }); weaponList.insert({ "zm_prototype_equipment", prototypeEquipment }); weaponList.insert({ "zm_prototype_hero", prototypeHero });
        weaponList.insert({ "zm_asylum_sniper", asylumSniper }); weaponList.insert({ "zm_asylum_smg", asylumSMG }); weaponList.insert({ "zm_asylum_launcher", asylumLauncher }); weaponList.insert({ "zm_asylum_shotgun", asylumShotgun }); weaponList.insert({ "zm_asylum_pistol", asylumPistol }); weaponList.insert({ "zm_asylum_lmg", asylumLMG }); weaponList.insert({ "zm_asylum_ar", asylumAR }); weaponList.insert({ "zm_asylum_special", asylumSpecial }); weaponList.insert({ "zm_asylum_melee", asylumMelee }); weaponList.insert({ "zm_asylum_equipment", asylumEquipment }); weaponList.insert({ "zm_asylum_hero", asylumHero });
        weaponList.insert({ "zm_sumpf_sniper", sumpfSniper }); weaponList.insert({ "zm_sumpf_smg", sumpfSMG }); weaponList.insert({ "zm_sumpf_launcher", sumpfLauncher }); weaponList.insert({ "zm_sumpf_shotgun", sumpfShotgun }); weaponList.insert({ "zm_sumpf_pistol", sumpfPistol }); weaponList.insert({ "zm_sumpf_lmg", sumpfLMG }); weaponList.insert({ "zm_sumpf_ar", sumpfAR }); weaponList.insert({ "zm_sumpf_special", sumpfSpecial }); weaponList.insert({ "zm_sumpf_melee", sumpfMelee }); weaponList.insert({ "zm_sumpf_equipment", sumpfEquipment }); weaponList.insert({ "zm_sumpf_hero", sumpfHero });
        weaponList.insert({ "zm_theater_sniper", theaterSniper }); weaponList.insert({ "zm_theater_smg", theaterSMG }); weaponList.insert({ "zm_theater_launcher", theaterLauncher }); weaponList.insert({ "zm_theater_shotgun", theaterShotgun }); weaponList.insert({ "zm_theater_pistol", theaterPistol }); weaponList.insert({ "zm_theater_lmg", theaterLMG }); weaponList.insert({ "zm_theater_ar", theaterAR }); weaponList.insert({ "zm_theater_special", theaterSpecial }); weaponList.insert({ "zm_theater_melee", theaterMelee }); weaponList.insert({ "zm_theater_equipment", theaterEquipment }); weaponList.insert({ "zm_theater_hero", theaterHero });
        weaponList.insert({ "zm_cosmodrome_sniper", cosmodromeSniper }); weaponList.insert({ "zm_cosmodrome_smg", cosmodromeSMG }); weaponList.insert({ "zm_cosmodrome_launcher", cosmodromeLauncher }); weaponList.insert({ "zm_cosmodrome_shotgun", cosmodromeShotgun }); weaponList.insert({ "zm_cosmodrome_pistol", cosmodromePistol }); weaponList.insert({ "zm_cosmodrome_lmg", cosmodromeLMG }); weaponList.insert({ "zm_cosmodrome_ar", cosmodromeAR }); weaponList.insert({ "zm_cosmodrome_special", cosmodromeSpecial }); weaponList.insert({ "zm_cosmodrome_melee", cosmodromeMelee }); weaponList.insert({ "zm_cosmodrome_equipment", cosmodromeEquipment }); weaponList.insert({ "zm_cosmodrome_hero", cosmodromeHero });
        weaponList.insert({ "zm_temple_sniper", templeSniper }); weaponList.insert({ "zm_temple_smg", templeSMG }); weaponList.insert({ "zm_temple_launcher", templeLauncher }); weaponList.insert({ "zm_temple_shotgun", templeShotgun }); weaponList.insert({ "zm_temple_pistol", templePistol }); weaponList.insert({ "zm_temple_lmg", templeLMG }); weaponList.insert({ "zm_temple_ar", templeAR }); weaponList.insert({ "zm_temple_special", templeSpecial }); weaponList.insert({ "zm_temple_melee", templeMelee }); weaponList.insert({ "zm_temple_equipment", templeEquipment }); weaponList.insert({ "zm_temple_hero", templeHero });
        weaponList.insert({ "zm_moon_sniper", moonSniper }); weaponList.insert({ "zm_moon_smg", moonSMG }); weaponList.insert({ "zm_moon_launcher", moonLauncher }); weaponList.insert({ "zm_moon_shotgun", moonShotgun }); weaponList.insert({ "zm_moon_pistol", moonPistol }); weaponList.insert({ "zm_moon_lmg", moonLMG }); weaponList.insert({ "zm_moon_ar", moonAR }); weaponList.insert({ "zm_moon_special", moonSpecial }); weaponList.insert({ "zm_moon_melee", moonMelee }); weaponList.insert({ "zm_moon_equipment", moonEquipment }); weaponList.insert({ "zm_moon_hero", moonHero });
        weaponList.insert({ "zm_tomb_sniper", tombSniper }); weaponList.insert({ "zm_tomb_smg", tombSMG }); weaponList.insert({ "zm_tomb_launcher", tombLauncher }); weaponList.insert({ "zm_tomb_shotgun", tombShotgun }); weaponList.insert({ "zm_tomb_pistol", tombPistol }); weaponList.insert({ "zm_tomb_lmg", tombLMG }); weaponList.insert({ "zm_tomb_ar", tombAR }); weaponList.insert({ "zm_tomb_special", tombSpecial }); weaponList.insert({ "zm_tomb_melee", tombMelee }); weaponList.insert({ "zm_tomb_equipment", tombEquipment }); weaponList.insert({ "zm_tomb_hero", tombHero });
    }
#pragma endregion

#pragma region BGB
#define PRESET_FILE "/Settings/Gum Profiles/GumPresets.csv"

    void InitBGBDescriptions()
    {
        gumDescriptions.reserve(63);
        gumDescriptions.emplace_back("Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 full rounds)\nWalk faster while aiming. Raise and lower your weapon to aim more quickly.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nInstantly teleport to a random location. A concussive blast knocks away any nearby zombies, keeping you safe.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 full rounds)\nSwitch weapons and recover from performing melee attacks faster. Reload and use items more quickly.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts until next respawn)\nRespawn with the guns you had when you bled out.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 10 minutes)\nCharge your special weapon faster.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 20 minutes)\nLonger bleedout time.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 full rounds)\nZero explosive damage.");
        gumDescriptions.emplace_back("Activated (4x Activations)\nOverrides the colors you see.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 full rounds)\nCan fire while sprinting.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts until bleedout)\nRespawn near the end of the current round instead of at the start of the next round.");
        gumDescriptions.emplace_back("Activated (2x Activations, 10 seconds each)\nYou are ignored by zombies for 10 seconds.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 1 full round)\nImproves your chances of activating an Alternate Ammo Type.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 25 minutes)\nZombies killed fall straight up.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nAll zombies will chase you for 10 seconds.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 5 full rounds)\nZombies you kill with grenades and large projectiles vomit uncontrollably.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 minutes)\nAmmo is taken from your stockpile instead of your weapon's magazine.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 2.5 minutes)\nMelee attacks deal zombies 5x as much damage.");
        gumDescriptions.emplace_back("Auto-activates when killing a zombie (25x Activations)\nSilly sounds play when zombies are killed.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 full rounds)\nKeep all perks after being revived.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 5 full rounds)\nRepairing a board immediately repairs all boards at that window.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts for 5 minutes)\nRepairing a board will kill all nearby zombies.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nRe-Pack your current Pack-a-Punched gun(if supported).");
        gumDescriptions.emplace_back("Activates Immediately (Lasts two hits)\nThe next time you take melee damage, nearby zombies burst into fire.");
        gumDescriptions.emplace_back("Activated (1x Activation)\nSpawns a max ammo power up.");
        gumDescriptions.emplace_back("Auto-activates next time you take a gun from the magic box\nThe next gun taken from the magic box comes Pack-a-Punched.");
        gumDescriptions.emplace_back("Activated (5x Activations)\nAll nearby zombies become crawlers.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nSpawns a nuke power up.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts for 5 minutes)\nGives a random gun every 10 seconds.");
        gumDescriptions.emplace_back("Activated (2x Activations, 60 seconds each)\nTurns the weapon in your hands into Pack-a-Punched weapon.");
        gumDescriptions.emplace_back("Activated (4x Activations)\nSpawns a personal 1250 point power up.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nSpawn a Death Machine power up.");
        gumDescriptions.emplace_back("Activated (1x Activation, 2 minutes)\nZombies seen by players will not move.");
        gumDescriptions.emplace_back("Activates Immediately (2x Activations)\nTransforms into a random Mega GobbleGum not in your Pack.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts for the remainder of the round)\nAny bullet which hits a zombie will damage its head.");
        gumDescriptions.emplace_back("Activated (3x Activations, 30 seconds each)\nAll zombies will ignore all players.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nSpawn a random power up.");
        gumDescriptions.emplace_back("Activated (3x Activations)\nSpawns a fire sale power up.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nSpawns an Instakill power up.");
        gumDescriptions.emplace_back("Activated (1x Activation)\nAll zombies freeze in place for 20 seconds.If they are shot, they will be annihilated when the time is up.");
        gumDescriptions.emplace_back("Activated (3x Activations)\nSpawns a Carpenter power up.");
        gumDescriptions.emplace_back("Activated (3x Activations)\nGib the heads of all zombies you can see, killing them.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 3 full rounds)\nRevive, or be revived, simply by being near other players. Revived players keep all of their perks.");
        gumDescriptions.emplace_back("Activated (1x Activation)\nSpawns a free perk power up.");
        gumDescriptions.emplace_back("Activates Immediately\nGives you all perks in the map.");
        gumDescriptions.emplace_back("Activated (1x Activation)\nRevives all teammates. Teammates keep all of their perks.");
        gumDescriptions.emplace_back("Auto-activates when melee attacking zombies (5x Activations)\nMelee attacks trigger an electrostatic discharge, electrocuting nearby zombies.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts for 4 full rounds)\nMore power ups can drop each round.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 10 minutes)\nPoints you earn are also received by nearby players, and vice versa.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nSpawns one of each of the nine core power ups.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nRe-spins the weapons in a magic box after it has been activated.");
        gumDescriptions.emplace_back("Activated (1x Activation)\nEnds the current round. All players gain 1600 points.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 10 minutes)\nAny gun wall-buy can be used to buy ammo for any gun.");
        gumDescriptions.emplace_back("Auto-activates by getting a kill in last stand (3x Activations)\nAuto revive yourself. Keep all of your perks.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 1 minute)\nAll purchases are free.");
        gumDescriptions.emplace_back("Auto-activates when sliding (6x Activations)\nCreate 2 lethal explosions by sliding.");
        gumDescriptions.emplace_back("Auto-activates when you buy a perk (5x Activations)\nCan buy an extra perk. Gives you a free perk after you buy one.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 1 full round)\nPower ups last longer.");
        gumDescriptions.emplace_back("Auto-activates when a teddy bear appears in the magic box.\nMagic box re-spins automatically. Magic box will not move for several uses.");
        gumDescriptions.emplace_back("Activates Immediately (Lasts 4 minutes)\nSlow down all zombies to shambling speed.");
        gumDescriptions.emplace_back("Auto-activates when you have maximum perks.\nCan buy an extra perk.");
        gumDescriptions.emplace_back("Auto-activates on your next wall-buy gun purchase\nThe next gun bought off a wall comes Pack-a-Punched.");
        gumDescriptions.emplace_back("Activated (2x Activations)\nSpawns a double points power up.");
    }

    void InitClassicGumsList()
    {
        classicList.emplace_back("Alchemical Antithesis");
        classicList.emplace_back("Always Done Swiftly");
        classicList.emplace_back("Anywhere But Here!");
        classicList.emplace_back("Armamental Accomplishment");
        classicList.emplace_back("Arms Grace");
        classicList.emplace_back("Arsenal Accelerator");
        classicList.emplace_back("Coagulant");
        classicList.emplace_back("Danger Closest");
        classicList.emplace_back("Eye Candy");
        classicList.emplace_back("Firing On All Cylinders");
        classicList.emplace_back("Impatient");
        classicList.emplace_back("In Plain Sight");
        classicList.emplace_back("Lucky Crit");
        classicList.emplace_back("Newtonian Negation");
        classicList.emplace_back("Now You See Me");
        classicList.emplace_back("Projectile Vomiting");
        classicList.emplace_back("Stock Option");
        classicList.emplace_back("Sword Flay");
        classicList.emplace_back("Tone Death");
    }

    void InitMegaGumsList()
    {
        megaList.emplace_back("Aftertaste");
        megaList.emplace_back("Board Games");
        megaList.emplace_back("Board To Death");
        megaList.emplace_back("Bullet Boost");
        megaList.emplace_back("Burned Out");
        megaList.emplace_back("Cache Back");
        megaList.emplace_back("Crate Power");
        megaList.emplace_back("Crawl Space");
        megaList.emplace_back("Dead Of Nuclear Winter");
        megaList.emplace_back("Disorderly Combat");
        megaList.emplace_back("Ephemeral Enhancement");
        megaList.emplace_back("Extra Credit");
        megaList.emplace_back("Fatal Contraption");
        megaList.emplace_back("Fear In Headlights");
        megaList.emplace_back("Flavor Hexed");
        megaList.emplace_back("Head Drama");
        megaList.emplace_back("Idle Eyes");
        megaList.emplace_back("I'm Feelin' Lucky");
        megaList.emplace_back("Immolation Liquidation");
        megaList.emplace_back("Kill Joy");
        megaList.emplace_back("Killing Time");
        megaList.emplace_back("Licensed Contractor");
        megaList.emplace_back("Mind Blown");
        megaList.emplace_back("Near Death Experience");
        megaList.emplace_back("On The House");
        megaList.emplace_back("Perkaholic");
        megaList.emplace_back("Phoenix Up");
        megaList.emplace_back("Pop Shocks");
        megaList.emplace_back("Power Vacuum");
        megaList.emplace_back("Profit Sharing");
        megaList.emplace_back("Reign Drops");
        megaList.emplace_back("Respin Cycle");
        megaList.emplace_back("Round Robbin'");
        megaList.emplace_back("Secret Shopper");
        megaList.emplace_back("Self Medication");
        megaList.emplace_back("Shopping Free");
        megaList.emplace_back("Slaughter Slide");
        megaList.emplace_back("Soda Fountain");
        megaList.emplace_back("Temporal Gift");
        megaList.emplace_back("Unbearable");
        megaList.emplace_back("Undead Man Walking");
        megaList.emplace_back("Unquenchable");
        megaList.emplace_back("Wall Power");
        megaList.emplace_back("Who's Keeping Score");
    }

    void LoadGumProfiles()
    {
        gumPresets.clear();

        std::string line;
        std::ifstream file(selfDirectory + PRESET_FILE);

        while (getline(file, line))
        {
            BGBPreset loadedPreset;
            std::stringstream sstream(line);
            std::string substr;
            getline(sstream, substr, ',');
            loadedPreset.presetName = substr;
            for (int i = 1; i < 6; i++)
            {
                getline(sstream, substr, ',');
                loadedPreset.presetGums[i - 1] = atoi(substr.c_str());
            }
            gumPresets.emplace_back(loadedPreset);
        }
        file.close();

        if (gumPresets.size())
            gumContextIndex = gumPresets[currentGumPreset].presetGums[0];
        else
            gumPresets.emplace_back(inactiveGumPreset);
    }

    void DeleteGumPreset(const std::string& presetName)
    {
        std::string line;
        std::string outData;
        std::ifstream inFile(selfDirectory + PRESET_FILE);

        while (getline(inFile, line))
        {
            std::stringstream sstream(line);
            std::string name;
            getline(sstream, name, ',');
            if (name != presetName)
                outData.append(line + "\n");
        }
        inFile.close();

        std::ofstream outFile(selfDirectory + PRESET_FILE);
        outFile << outData;
        outFile.close();
        if (currentGumPreset > 0) currentGumPreset--;
        LoadGumProfiles();
    }

    void CreateNewGumPreset(const std::string& presetName)
    {
        std::string line;
        std::ifstream checkFile(selfDirectory + PRESET_FILE);
        while (getline(checkFile, line));

        std::ofstream file(selfDirectory + PRESET_FILE, std::ios::app);
        if (line != "")
            file << "\n";
        checkFile.close();
        file << presetName << ",0,1,2,3,4\n";
        file.close();
        LoadGumProfiles();
        currentGumPreset = (int)gumPresets.size() - 1;
    }

    void WriteGumPreset(const std::vector<int>& gumPreset)
    {
        std::string line;
        std::string outData;
        std::ifstream inFile(selfDirectory + PRESET_FILE);

        while (getline(inFile, line))
        {
            std::stringstream sstream(line);
            std::string name;
            getline(sstream, name, ',');
            std::string preset(GetCurrentPresetName());
            if (name != preset)
            {
                outData.append(line + "\n");
                continue;
            }
            outData.append(preset + ",");
            for (const int& gum : gumPreset)
            {
                outData.append(std::to_string(gum) + ",");
            }
            outData.replace(outData.size() - 1, 1, "\n");
        }
        inFile.close();

        std::ofstream outFile(selfDirectory + PRESET_FILE);
        outFile << outData;
        outFile.close();

        LoadGumProfiles();
    }

    void WritePresetToGame(BGBPreset& gumPreset, const std::string& file)
    {
        std::string outData;
        for (const int& gum : gumPreset.presetGums)
        {
            outData.append(std::to_string(gum) + ",");
        }
        outData.replace(outData.size() - 1, 1, "");
        std::ofstream outFile(file);
        outFile << outData;
        outFile.close();
    }

    bool CheckPresetExists(const std::string& inPreset)
    {
        for (const BGBPreset& preset : gumPresets)
        {
            if (preset.presetName == inPreset)
                return true;
        }
        return false;
    }

    std::string GetCurrentPresetName()
    {
        return gumPresets[currentGumPreset].presetName;
    }
#pragma endregion

#pragma region Perks
#pragma region PerkDefs
#define JUG "Juggernog"
#define SPEED "Speed Cola"
#define DTAP "Double Tap"
#define QR "Quick Revive"
#define STAM "Stamin-Up"
#define MULEKICK "Mule Kick"
#define DEADSHOT "Deadshot Daiquiri"
#define WIDOWS "Widow's Wine"
#define CHERRY "Electric Cherry"
#pragma endregion
    void InitPerksList()
    {
        std::vector<std::string> zodPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, WIDOWS };
        std::vector<std::string> factoryPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT };
        std::vector<std::string> castlePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> islandPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> stalingradPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> genesisPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> prototypePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> asylumPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> sumpfPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> theaterPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> cosmodromePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> templePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> moonPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> tombPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        perksList.insert({ "zm_zod",  zodPerks });
        perksList.insert({ "zm_factory",  factoryPerks });
        perksList.insert({ "zm_castle",  castlePerks });
        perksList.insert({ "zm_island",  islandPerks });
        perksList.insert({ "zm_stalingrad",  stalingradPerks });
        perksList.insert({ "zm_genesis",  genesisPerks });
        perksList.insert({ "zm_prototype",  prototypePerks });
        perksList.insert({ "zm_asylum",  asylumPerks });
        perksList.insert({ "zm_sumpf",  sumpfPerks });
        perksList.insert({ "zm_theater",  theaterPerks });
        perksList.insert({ "zm_cosmodrome",  cosmodromePerks });
        perksList.insert({ "zm_temple",  templePerks });
        perksList.insert({ "zm_moon",  moonPerks });
        perksList.insert({ "zm_tomb",  tombPerks });
    }
#pragma endregion

#pragma region PowerupOptions
#pragma region PowerupDefs
#define POINTS_PLAYER "Bonus Points Player"
#define POINTS_TEAM "Bonus Points Team"
#define CARPENTER "Carpenter"
#define DOUBLE_POINTS "Double Points"
#define FIRE_SALE "Fire Sale"
#define EMPTY_PERK "Perk Slot"
#define FREE_PERK "Free Perk"
#define MAX "Max Ammo"
#define INSTA "Insta Kill"
#define MINIGUN "Death Machine"
#define NUKE "Nuke"
#define SHIELDCHARGE "Shield Charge"
#define WW_GRENADE "Widows Grenade"
#define TRAM "Tram Token"
#define RUNE_LOR "Void Symbol Tears"
#define RUNE_MAR "Void Symbol Ribbon"
#define RUNE_OTH "Void Symbol Claw"
#define RUNE_UJA "Void Symbol Triangles"
#define RUNE_ULLA "Void Symbol Rib"
#define RUNE_ZOR "Void Symbol Tear"
#define SEED "Seed"
#define CYLINDER_RED "Tank Cylinder"
#define CYLINDER_YELLOW "Supply Cylinder"
#define CYLINDER_BLUE "DC Cylinder"
#define GENESIS_WEAPON "Random Weapon"
#define ZOMBIE_BLOOD "Zombie Blood"
#pragma endregion
    void InitPowerupList()
    {
        std::vector<std::string> zodPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, POINTS_TEAM, WW_GRENADE, SHIELDCHARGE };
        std::vector<std::string> factoryPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER };
        std::vector<std::string> castlePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, POINTS_TEAM, WW_GRENADE, SHIELDCHARGE, TRAM, RUNE_LOR, RUNE_ULLA, RUNE_UJA, RUNE_OTH, RUNE_ZOR, RUNE_MAR };
        std::vector<std::string> islandPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, POINTS_TEAM, WW_GRENADE, SHIELDCHARGE, EMPTY_PERK, SEED };
        std::vector<std::string> stalingradPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE, SHIELDCHARGE, CYLINDER_RED, CYLINDER_YELLOW, CYLINDER_BLUE };
        std::vector<std::string> genesisPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE, SHIELDCHARGE, GENESIS_WEAPON };
        std::vector<std::string> prototypePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> asylumPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> sumpfPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> theaterPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> cosmodromePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> templePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> moonPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> tombPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE, SHIELDCHARGE, ZOMBIE_BLOOD };
        powerupList.insert({ "zm_zod",  zodPowerups });
        powerupList.insert({ "zm_factory",  factoryPowerups });
        powerupList.insert({ "zm_castle",  castlePowerups });
        powerupList.insert({ "zm_island",  islandPowerups });
        powerupList.insert({ "zm_stalingrad",  stalingradPowerups });
        powerupList.insert({ "zm_genesis",  genesisPowerups });
        powerupList.insert({ "zm_prototype",  prototypePowerups });
        powerupList.insert({ "zm_asylum",  asylumPowerups });
        powerupList.insert({ "zm_sumpf",  sumpfPowerups });
        powerupList.insert({ "zm_theater",  theaterPowerups });
        powerupList.insert({ "zm_cosmodrome",  cosmodromePowerups });
        powerupList.insert({ "zm_temple",  templePowerups });
        powerupList.insert({ "zm_moon",  moonPowerups });
        powerupList.insert({ "zm_tomb",  tombPowerups });
    }
#pragma endregion

#pragma region ZombieCalc
    int GetZombieCountForRound(int round, int playerCount)
    {
        int maxZombies = 24;
        float multiplier = round / 5.f;
        if (multiplier < 1)
            multiplier = 1;
        if (round >= 10)
            multiplier *= (round * 0.15f);
        if (playerCount == 1)
            maxZombies += static_cast<int>(3 * multiplier);
        else
            maxZombies += static_cast<int>(((playerCount - 1) * 6) * multiplier);
        return MaxZombieFunc(maxZombies, round);
    }

    int MaxZombieFunc(int maxZombies, int round)
    {
        if (round < 2)
            return static_cast<int>(maxZombies * 0.25);
        if (round < 3)
            return static_cast<int>(maxZombies * 0.3);
        if (round < 4)
            return static_cast<int>(maxZombies * 0.5);
        if (round < 5)
            return static_cast<int>(maxZombies * 0.7);
        if (round < 6)
            return static_cast<int>(maxZombies * 0.9);
        return static_cast<int>(maxZombies);
    }

    int GetZombiesUpToRound(int round, int playerCount)
    {
        if (round == 1)
            return 0;
        return GetZombieCountForRound(round - 1, playerCount) + GetZombiesUpToRound(round - 1, playerCount);
    }

    int GetZombieHealthForRound(int round)
    {
        if (round > 162) return INT_MAX;
        int zombieHealth = 150;
        if (round < 10) zombieHealth += 100 * (round - 1);
        else zombieHealth += 100 * (9 - 1);
        if (round < 10)
            return zombieHealth;
        for (int i = 10; i <= round; i++)
        {
            zombieHealth += static_cast<int>(zombieHealth * 0.1);
        }
        return zombieHealth;
    }

    int GetZombieSpawnRateForRound(int round, int playerCount)
    {
        if (round == 1)
            return 2100;
        float startDelay = 2.0f;
        if (playerCount == 2)
            startDelay = 1.5f;
        else if (playerCount == 3)
            startDelay = 0.89f;
        else if (playerCount == 4)
            startDelay = 0.67f;
        float calcSpawnRate = startDelay * static_cast<float>(pow(0.95f, round - 1)) + 0.1f;
        if (calcSpawnRate < 0.2f)
            calcSpawnRate = 0.2f;
        int calcRoundedTime = static_cast<int>(calcSpawnRate * 1000);
        return RoundTimeNearest50(calcRoundedTime);
    }

    float RawSpawnRateForRound(int round, int playerCount)
    {
        if (round == 1)
            return 2.1f;
        float startDelay = 2.0f;
        if (playerCount == 2)
            startDelay = 1.5f;
        else if (playerCount == 3)
            startDelay = 0.89f;
        else if (playerCount == 4)
            startDelay = 0.67f;
        return startDelay * static_cast<float>(pow(0.95f, round - 1));
    }

    int RoundTimeNearest50(int time)
    {
        int newTime = time % 50;
        if (newTime < 25)
            newTime = time - newTime;
        else
            newTime = time + (50 - newTime);
        return newTime;
    }

    int GetSpecialEnemySpawnRate(int playerCount, const std::string& specialEnemy)
    {
        switch (hashstr(specialEnemy.c_str()))
        {
        case hashstr("Meatballs"):
        case hashstr("Manglers"):
        case hashstr("Valks"):
            switch (playerCount)
            {
            case 1:
                return 2350;
            case 2:
                return 1850;
            case 3:
                return 1350;
            case 4:
                return 850;
            }
            break;
        case hashstr("Dogs"):
            switch (numDogRounds)
            {
            case 1:
                return 2100;
            case 2:
                return 1600;
            case 3:
                return 1100;
            default:
                return 600;
            }
            break;
        case hashstr("Spiders"):
            switch (playerCount)
            {
            case 1:
                return 2350;
            case 2:
                return 2100;
            case 3:
                return 1850;
            case 4:
                return 1600;
            }
            break;
        case hashstr("Furys"):
            return 2750;
        case hashstr("Keepers"):
            return 1400;
        }
        return 0;
    }

    std::string CalcRoundTime(int round, int playerCount, int corpseDelay, bool specialEnemies)
    {
        int roundIntermission = 12600;
        if (round == 1)
            roundIntermission = 8150;
        int zombieCount = GetZombieCountForRound(round, playerCount);
        int specialEnemiesSpawnTime = 0;
        if (specialEnemies)
        {
            zombieCount -= specialEnemyCount_1 + specialEnemyCount_2 + specialEnemyCount_3;
            if (specialEnemyCount_1)
                specialEnemiesSpawnTime += GetSpecialEnemySpawnRate(playerCount, specialEnemy_1) * specialEnemyCount_1;
            if (specialEnemyCount_2)
                specialEnemiesSpawnTime += GetSpecialEnemySpawnRate(playerCount, specialEnemy_2) * specialEnemyCount_2;
            if (specialEnemyCount_3)
                specialEnemiesSpawnTime += GetSpecialEnemySpawnRate(playerCount, specialEnemy_3) * specialEnemyCount_3;
        }
        int zombieSpawnsTime = GetZombieSpawnRateForRound(round, playerCount) * (zombieCount - 1) + specialEnemiesSpawnTime + roundIntermission;
        if (corpseDelay) zombieSpawnsTime += 100 * corpseDelay;
        return ParseTimeFromMilli(zombieSpawnsTime);
    }

    std::string SpecialRoundTime(int round, int playerCount, int corpseDelay, bool soe)
    {
        int roundIntermission = 12600;
        if (round == 1)
            roundIntermission = 8150;
        int zombieCount = GetZombieCountForRound(round, playerCount);
        int zombieSpawnsTime;
        if (soe)
            zombieSpawnsTime = 2100 * (zombieCount - 1);
        else
        {
            if (moonRoundSkip)
                zombieCount = 25;
            zombieSpawnsTime = GetZombieSpawnRateForRound(round + (moonEarthTravels * 2 - moonRoundSkips) + 1, 1) * (zombieCount - 1);
        }
        zombieSpawnsTime += +roundIntermission;
        if (corpseDelay) zombieSpawnsTime += 100 * corpseDelay;
        return ParseTimeFromMilli(zombieSpawnsTime);
    }

    std::string CustomRoundTime(int round, int playerCount, int zombieCount)
    {
        int zombieSpawnsTime = GetZombieSpawnRateForRound(round, playerCount) * (zombieCount - 1);
        return ParseTimeFromMilli(zombieSpawnsTime);
    }

    void CalcLockdownTime(int round, int playerCount)
    {
        float rawSpawnRate_1 = RawSpawnRateForRound(round, playerCount);
        if (rawSpawnRate_1 < 0.1f)
            rawSpawnRate_1 = 0.1f;
        float rawSpawnRate_2 = rawSpawnRate_1 - playerCount * 0.1f;
        if (rawSpawnRate_2 < 0.1f)
            rawSpawnRate_2 = 0.1f;
        float rawSpawnRate_3 = rawSpawnRate_2 - playerCount * 0.1f;
        if (rawSpawnRate_3 < 0.1f)
            rawSpawnRate_3 = 0.1f;
        float rawSpawnRate_4 = rawSpawnRate_3 - playerCount * 0.1f;
        if (rawSpawnRate_4 < 0.1f)
            rawSpawnRate_4 = 0.1f;

        int spawnRate_1 = RoundTimeNearest50(static_cast<int>(rawSpawnRate_1 * 1000)) + 50;
        int spawnRate_2 = RoundTimeNearest50(static_cast<int>(rawSpawnRate_2 * 1000)) + 50;
        int spawnRate_3 = RoundTimeNearest50(static_cast<int>(rawSpawnRate_3 * 1000)) + 50;
        int spawnRate_4 = RoundTimeNearest50(static_cast<int>(rawSpawnRate_4 * 1000)) + 50;

        int waveCount_1 = 14 + (8 * playerCount);
        int waveCount_2 = 16 + (9 * playerCount);
        int waveCount_3 = 18 + (10 * playerCount);
        int waveCount_4 = 18 + (10 * playerCount * 2);

        int waveIntermission_1 = 6200;
        int waveIntermission_2 = 5500;
        int waveIntermission_3 = 5500;
        int waveIntermission_4 = 3300;

        waveTime_1 = ParseTimeFromMilli(spawnRate_1 * waveCount_1 + waveIntermission_1 + 500);
        waveTime_2 = ParseTimeFromMilli(spawnRate_2 * waveCount_2 + waveIntermission_2 + 500);
        waveTime_3 = ParseTimeFromMilli(spawnRate_3 * waveCount_3 + waveIntermission_3 + 500);
        waveTime_4 = ParseTimeFromMilli(spawnRate_4 * waveCount_4 + waveIntermission_4 + 500);
    }
#pragma endregion

#pragma region GKValveSolver
    void InitValveSolutions()
    {
        std::unordered_map<std::string, std::vector<std::string>> departmentSolutions;
        std::unordered_map<std::string, std::vector<std::string>> dragonSolutions;
        std::unordered_map<std::string, std::vector<std::string>> armorySolutions;
        std::unordered_map<std::string, std::vector<std::string>> supplySolutions;
        std::unordered_map<std::string, std::vector<std::string>> infirmarySolutions;
        std::unordered_map<std::string, std::vector<std::string>> tankSolutions;

        departmentSolutions.insert({ "Dragon Command", { "P", "1", "2", "2", "1", "1" } });
        departmentSolutions.insert({ "Armory", { "P", "2", "1", "3", "3", "1" } });
        departmentSolutions.insert({ "Supply", { "P", "3", "3", "1", "2", "3" } });
        departmentSolutions.insert({ "Infirmary", { "P", "2", "1", "1", "2", "3" } });
        departmentSolutions.insert({ "Tank", { "P", "1", "3", "2", "3", "1" } });

        dragonSolutions.insert({ "Department", { "1", "P", "1", "3", "3", "1" } });
        dragonSolutions.insert({ "Armory", { "3", "P", "1", "3", "1", "1" } });
        dragonSolutions.insert({ "Supply", { "3", "P", "2", "2", "1", "1" } });
        dragonSolutions.insert({ "Infirmary", { "1", "P", "2", "1", "1", "2" } });
        dragonSolutions.insert({ "Tank", { "1", "P", "1", "1", "1", "1" } });

        armorySolutions.insert({ "Department", { "2", "1", "P", "3", "3", "3" } });
        armorySolutions.insert({ "Dragon Command", { "1", "1", "P", "3", "1", "1" } });
        armorySolutions.insert({ "Supply", { "2", "2", "P", "1", "2", "3" } });
        armorySolutions.insert({ "Infirmary", { "3", "1", "P", "3", "1", "3" } });
        armorySolutions.insert({ "Tank", { "3", "1", "P", "2", "1", "1" } });

        supplySolutions.insert({ "Department", { "1", "1", "2", "P", "3", "1" } });
        supplySolutions.insert({ "Dragon Command", { "2", "2", "1", "P", "2", "3" } });
        supplySolutions.insert({ "Armory", { "3", "1", "2", "P", "1", "1" } });
        supplySolutions.insert({ "Infirmary", { "3", "1", "3", "P", "2", "3" } });
        supplySolutions.insert({ "Tank", { "1", "2", "1", "P", "3", "1" } });

        infirmarySolutions.insert({ "Department", { "1", "3", "2", "1", "P", "2" } });
        infirmarySolutions.insert({ "Dragon Command", { "2", "1", "3", "3", "P", "3" } });
        infirmarySolutions.insert({ "Armory", { "2", "2", "2", "1", "P", "2" } });
        infirmarySolutions.insert({ "Supply", { "1", "2", "2", "1", "P", "1" } });
        infirmarySolutions.insert({ "Tank", { "3", "3", "3", "2", "P", "2" } });

        tankSolutions.insert({ "Department", { "1", "3", "1", "1", "2", "P" } });
        tankSolutions.insert({ "Dragon Command", { "2", "1", "3", "2", "2", "P" } });
        tankSolutions.insert({ "Armory", { "2", "2", "1", "1", "2", "P" } });
        tankSolutions.insert({ "Supply", { "1", "3", "2", "1", "1", "P" } });
        tankSolutions.insert({ "Infirmary", { "3", "1", "2", "2", "1", "P" } });

        valveSolutions_1.insert({ "Department", departmentSolutions });
        valveSolutions_1.insert({ "Dragon Command", dragonSolutions });
        valveSolutions_1.insert({ "Armory", armorySolutions });
        valveSolutions_1.insert({ "Supply", supplySolutions });
        valveSolutions_1.insert({ "Infirmary", infirmarySolutions });
        valveSolutions_1.insert({ "Tank", tankSolutions });

        departmentSolutions = { };
        dragonSolutions = { };
        armorySolutions = { };
        supplySolutions = { };
        infirmarySolutions = { };
        tankSolutions = { };

        departmentSolutions.insert({ "Dragon Command", { "P", "3", "3", "2", "2", "2" } });
        departmentSolutions.insert({ "Armory", { "P", "3", "2", "1", "1", "2" } });
        departmentSolutions.insert({ "Supply", { "P", "2", "2", "2", "3", "1" } });
        departmentSolutions.insert({ "Infirmary", { "P", "1", "3", "3", "3", "3" } });
        departmentSolutions.insert({ "Tank", { "P", "3", "1", "1", "1", "3" } });

        dragonSolutions.insert({ "Department", { "2", "P", "1", "1", "2", "3" } });
        dragonSolutions.insert({ "Armory", { "2", "P", "3", "1", "2", "2" } });
        dragonSolutions.insert({ "Supply", { "2", "P", "3", "3", "3", "3" } });
        dragonSolutions.insert({ "Infirmary", { "3", "P", "3", "2", "2", "2" } });
        dragonSolutions.insert({ "Tank", { "2", "P", "3", "2", "3", "2" } });

        armorySolutions.insert({ "Department", { "3", "3", "P", "2", "2", "2" } });
        armorySolutions.insert({ "Dragon Command", { "2", "2", "P", "2", "2", "2" } });
        armorySolutions.insert({ "Supply", { "1", "2", "P", "3", "3", "1" } });
        armorySolutions.insert({ "Infirmary", { "1", "2", "P", "1", "2", "2" } });
        armorySolutions.insert({ "Tank", { "1", "3", "P", "1", "1", "2" } });

        supplySolutions.insert({ "Department", { "3", "3", "1", "P", "2", "3" } });
        supplySolutions.insert({ "Dragon Command", { "1", "3", "2", "P", "1", "2" } });
        supplySolutions.insert({ "Armory", { "3", "3", "3", "P", "2", "2" } });
        supplySolutions.insert({ "Infirmary", { "1", "2", "2", "P", "3", "2" } });
        supplySolutions.insert({ "Tank", { "2", "1", "3", "P", "3", "3" } });

        infirmarySolutions.insert({ "Department", { "3", "1", "2", "2", "P", "1" } });
        infirmarySolutions.insert({ "Dragon Command", { "1", "2", "1", "3", "P", "1" } });
        infirmarySolutions.insert({ "Armory", { "3", "1", "3", "3", "P", "1" } });
        infirmarySolutions.insert({ "Supply", { "3", "3", "3", "3", "P", "3" } });
        infirmarySolutions.insert({ "Tank", { "2", "2", "1", "1", "P", "3" } });

        tankSolutions.insert({ "Department", { "2", "1", "2", "2", "3", "P" } });
        tankSolutions.insert({ "Dragon Command", { "1", "3", "1", "3", "1", "P" } });
        tankSolutions.insert({ "Armory", { "2", "1", "3", "3", "3", "P" } });
        tankSolutions.insert({ "Supply", { "3", "3", "3", "2", "2", "P" } });
        tankSolutions.insert({ "Infirmary", { "1", "2", "1", "3", "3", "P" } });

        valveSolutions_2.insert({ "Department", departmentSolutions });
        valveSolutions_2.insert({ "Dragon Command", dragonSolutions });
        valveSolutions_2.insert({ "Armory", armorySolutions });
        valveSolutions_2.insert({ "Supply", supplySolutions });
        valveSolutions_2.insert({ "Infirmary", infirmarySolutions });
        valveSolutions_2.insert({ "Tank", tankSolutions });
    }

    void CalcValveProbabilities()
    {
        valveDirectionCounts_1 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        valveDirectionCounts_2 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        for (std::string& solutions : valveLocations)
        {
            if (passwordChosen && passwordLocation != solutions)
                continue;
            int checked = -1;
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_1[solutions])
            {
                checked++;
                if (std::find(excludedValves.begin(), excludedValves.end(), solution.second) != excludedValves.end())
                    continue;
                if (greenChosen && solution.first != greenLocation)
                    continue;
                int num = locationToInt[solution.first];
                if (checkedGreenArray[locationToInt[solution.first]])
                    continue;
                int index = -1;
                for (std::string& direction : solution.second)
                {
                    index++;
                    if (direction == "P")
                        continue;
                    int number = atoi(direction.c_str()) - 1;
                    valveDirectionCounts_1[index][number]++;
                }
            }
        }
        for (std::string& solutions : valveLocations)
        {
            if (passwordLocation != "" && passwordLocation != solutions)
                continue;
            int checked = -1;
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_2[solutions])
            {
                checked++;
                if (std::find(excludedValves.begin(), excludedValves.end(), solution.second) != excludedValves.end())
                    continue;
                if (greenChosen && solution.first != greenLocation)
                    continue;
                int num = locationToInt[solution.first];
                if (checkedGreenArray[locationToInt[solution.first]])
                    continue;
                int index = -1;
                for (std::string& direction : solution.second)
                {
                    index++;
                    if (direction == "P")
                        continue;
                    int number = atoi(direction.c_str()) - 1;
                    valveDirectionCounts_2[index][number]++;
                }
            }
        }
        int odds = 0;
        for (std::vector<int>& counts : valveDirectionCounts_1)
        {
            int index = -1;
            for (int& count : counts)
            {
                index++;
                int divideBy = valveDirectionCounts_1[odds][0] + valveDirectionCounts_1[odds][1] + valveDirectionCounts_1[odds][2];
                if (!divideBy)
                    valveDirectionOdds_1[odds][index] = "P";
                else
                {
                    float percentage = count * 100.0f / divideBy;
                    char buf[8];
                    sprintf_s(buf, "%.2f", percentage);
                    std::string addPercentage = buf;
                    addPercentage += "%%";
                    valveDirectionOdds_1[odds][index] = addPercentage;
                }
            }
            odds++;
        }
        odds = 0;
        for (std::vector<int>& counts : valveDirectionCounts_2)
        {
            int index = -1;
            for (int& count : counts)
            {
                index++;
                int divideBy = valveDirectionCounts_2[odds][0] + valveDirectionCounts_2[odds][1] + valveDirectionCounts_2[odds][2];
                if (!divideBy)
                    valveDirectionOdds_2[odds][index] = "P";
                else
                {
                    float percentage = count * 100.0f / divideBy;
                    char buf[8];
                    sprintf_s(buf, "%.2f", percentage);
                    std::string addPercentage = buf;
                    addPercentage += "%%";
                    valveDirectionOdds_2[odds][index] = addPercentage;
                }
            }
            odds++;
        }
        int a = 0;
    }

    void CalcRemainingGreen()
    {
        int notGreen = 0;
        std::string turnToGreen = "";
        int compareAgainst = (passwordChosen) ? 1 : 0;
        noGreenChoice = false;
        for (int i = 0; i < 6; i++)
        {
            if (checkedGreenArray[i])
            {
                if (passwordChosen && passwordLocation == valveLocations[i])
                    continue;
                notGreen++;
            }
            else
                turnToGreen = valveLocations[i];
            if (i - notGreen > compareAgainst)
                return;
        }
        if (notGreen >= 6)
            return;
        noGreenChoice = true;
        greenLocation = turnToGreen;
        valveGreen[locationToInt[greenLocation]] = true;
        greenChosen = true;
    }

    void CalcExcludedValves()
    {
        std::vector<std::vector<std::string>> results;
        std::vector<std::string> result;
        excludedValves = { };
        possibleValves_1 = { };
        possibleValves_2 = { };
        for (int valve = 0; valve < 6; valve++)
        {
            for (int direction = 0; direction < 3; direction++)
            {
                if (passwordLocation == valveLocations[valve])
                {
                    result.emplace_back("P");
                    break;
                }
                if (valveDirections[valve][direction])
                {
                    result.emplace_back(std::to_string(direction + 1));
                    break;
                }
                if (direction == 2)
                    result.emplace_back("0");
            }
        }
        if (result.size() != 6)
        {
            valveComboSet = false;
            return;
        }
        if (passwordChosen)
            valveComboSet = true;
        for (std::string& solutions : valveLocations)
        {
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_1[solutions])
            {
                if (result == solution.second)
                {
                    excludedValves.emplace_back(result);
                }
                else if (passwordLocation == solutions)
                {
                    possibleValves_1.insert({ solution.first, solution.second });
                }
            }
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_2[solutions])
            {
                if (result == solution.second)
                {
                    excludedValves.emplace_back(result);
                }
                else if (passwordLocation == solutions)
                {
                    possibleValves_2.insert({ solution.first, solution.second });
                }
            }
        }
    }
#pragma endregion

#pragma region IceCodeGuide
    void InitIceCodePairs()
    {
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["dot-digit"], iceCodeImgList["dot-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["i-digit"], iceCodeImgList["i-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["l-digit"], iceCodeImgList["l-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["f-digit"], iceCodeImgList["f-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["i-dot-digit"], iceCodeImgList["i-dot-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["i-i-digit"], iceCodeImgList["i-i-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["i-l-digit"], iceCodeImgList["i-l-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["i-f-digit"], iceCodeImgList["i-f-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["l-dot-digit"], iceCodeImgList["l-dot-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["l-i-digit"], iceCodeImgList["l-i-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["l-l-digit"], iceCodeImgList["l-l-symbol"] }));
        iceCodePairs.emplace_back(IceCodePair({ iceCodeImgList["l-f-digit"], iceCodeImgList["l-f-symbol"] }));

        randomIceCodePairs = iceCodePairs;

        RandomizeCodes();
    }

    void RandomizeCodes()
    {
        Walnut::Random::Init();
        std::shuffle(iceCodePairs.begin(), iceCodePairs.end(), std::default_random_engine(Walnut::Random::UInt()));
        Walnut::Random::Init();
        std::shuffle(randomIceCodePairs.begin(), randomIceCodePairs.end(), std::default_random_engine(Walnut::Random::UInt()));
        Walnut::Random::Init();
        std::shuffle(&randomList[0], &randomList[11], std::default_random_engine(Walnut::Random::UInt()));
    }

    void ProgressGame(bool success, int numCode)
    {
        if (!gameStarted)
        {
            gameStarted = true;
            gameTimer.Reset();
        }
        if (success)
        {
            gameProgress++;
            for (bool& checked : gameChecked)
                checked = false;
            if (gameProgress >= 12)
            {
                gameTime = "Time: " + ParseTimeFromMilli(gameTimer.Elapsed());
                std::stringstream ss;
                ss.precision(4);
                float percentage = (timesGuessed - timesMissed) / (float)timesGuessed * 100;
                ss << percentage << "%%";
                accuracy = "Accuracy: " + ss.str();
                gameProgress = 0;
                gameStarted = false;
                timesMissed = 0;
                timesGuessed = 0;
                RandomizeCodes();
                return;
            }
            Walnut::Random::Init();
            std::shuffle(randomIceCodePairs.begin(), randomIceCodePairs.end(), std::default_random_engine(Walnut::Random::UInt()));
        }
        else if (!gameChecked[numCode])
        {
            gameChecked[numCode] = true;
            timesMissed++;
        }
        timesGuessed++;
        std::stringstream ss;
        ss.precision(4);
        float percentage = (timesGuessed - timesMissed) / (float)timesGuessed * 100;
        ss << "Accuracy: " << percentage << "%%";
        accuracy = "Accuracy: " + ss.str();
    }
#pragma endregion

#pragma region Autosplits
#define PRESET_DIRECTORY "/Settings/Autosplits"
    void LoadSplitPresets()
    {
        splitPresets.clear();
        for (const auto& file : std::filesystem::directory_iterator(std::filesystem::path(selfDirectory) / "Settings\\Autosplits"))
        {
            if (std::filesystem::is_regular_file(file) && file.path().extension().string() == ".json")
            {
                SplitPreset preset = ParseSplitJson(file.path().string());
                preset.presetName = file.path().stem().string();
                splitPresets.emplace_back(preset);
            }
        }

        if (!splitPresets.size())
            splitPresets.emplace_back(inactiveSplitPreset);
    }

    void WriteAutosplitPreset(const SplitPreset& preset)
    {
        if (preset.presetName != inactiveSplitPreset.presetName && (!writeSplits || appStatus == "Status: Inactive"))
        {
            WriteAutosplitPreset(inactiveSplitPreset);
            return;
        }

        std::string filename = selfDirectory + PRESET_DIRECTORY + "/" + preset.presetName + ".json";

        Walnut::JSONBuilder builder;

        rapidjson::Value& settings = builder.AddObject(builder.GetDocument(), "Settings");
        settings.AddMember("In Game Timer", preset.igt, builder.GetAllocator());
        settings.AddMember("In Game Round Timer", preset.igrt, builder.GetAllocator());
        settings.AddMember("Amount of Splits", preset.numSplits, builder.GetAllocator());
        settings.AddMember("Map Index", preset.map, builder.GetAllocator());
        settings.AddMember("Split Type", preset.splitType, builder.GetAllocator());

        rapidjson::Value& splitData = builder.AddObject(builder.GetDocument(), "Split Data");
        for (std::pair<std::string, int> pair : preset.splits)
        {
            rapidjson::Value key(rapidjson::StringRef(pair.first.c_str()), builder.GetAllocator());
            rapidjson::Value value(pair.second);
            splitData.AddMember(key, value, builder.GetAllocator());
        }

        if (preset.presetName != inactiveSplitPreset.presetName)
            builder.SaveToFile(filename);
        builder.SaveToFile(bo3Directory + "\\Practice Tool\\Settings\\Active Autosplit Preset.json");
    }

    void CreateNewAutosplitPreset(const std::string& presetName)
    {
        std::string filename = selfDirectory + PRESET_DIRECTORY + "/" + presetName + ".json";
       
        Walnut::JSONBuilder builder;

        rapidjson::Value& settings = builder.AddObject(builder.GetDocument(), "Settings");
        settings.AddMember("In Game Timer", false, builder.GetAllocator());
        settings.AddMember("In Game Round Timer", true, builder.GetAllocator());
        settings.AddMember("Amount of Splits", 0, builder.GetAllocator());
        settings.AddMember("Map Index", 0, builder.GetAllocator());
        settings.AddMember("Split Type", 0, builder.GetAllocator());

        rapidjson::Value& splitData = builder.AddObject(builder.GetDocument(), "Split Data");

        builder.SaveToFile(filename);
        LoadSplitPresets();
    }

    void DeleteAutosplitPreset(const std::string& preset)
    {
        std::string file = PRESET_DIRECTORY;
        file += "/" + preset + ".json";
        if (std::filesystem::exists(selfDirectory + file))
            std::filesystem::remove(selfDirectory + file);
        if (currentSplitPreset > 0) currentSplitPreset--;
        LoadSplitPresets();
    }

    void WriteSplitXML(const std::string& preset, const std::vector<std::pair<std::string, int>>& splits)
    {
        Walnut::XMLBuilder builder("1.0", "Run");

        rapidxml::xml_node<>* runNode = builder.GetRootNode();
        builder.AddAttribute(runNode, "version", "1.7.0");

        rapidxml::xml_node<>* gameIconNode = builder.AddNode(builder.GetRootNode(), "GameIcon");

        builder.AddNode(builder.GetRootNode(), "GameName", "Black Ops 3");
        builder.AddNode(builder.GetRootNode(), "CategoryName", preset);
        builder.AddNode(builder.GetRootNode(), "LayoutPath");

        rapidxml::xml_node<>* metadataNode = builder.AddNode(builder.GetRootNode(), "Metadata");
        rapidxml::xml_node<>* metadataRunNode = builder.AddNode(metadataNode, "Run");
        builder.AddAttribute(metadataRunNode, "id", "");

        rapidxml::xml_node<>* platformNode = builder.AddNode(metadataNode, "Platform");
        builder.AddAttribute(platformNode, "usesEmulator", "False");
        builder.AddNode(metadataNode, "Region");
        builder.AddNode(metadataNode, "Variables");
        builder.AddNode(builder.GetRootNode(), "Offset", "00:00:00");
        builder.AddNode(builder.GetRootNode(), "AttemptCount", "0");
        builder.AddNode(builder.GetRootNode(), "AttemptHistory");

        rapidxml::xml_node<>* segmentsNode = builder.AddNode(builder.GetRootNode(), "Segments");

        for (auto split : splits)
        {
            rapidxml::xml_node<>* segmentNode = builder.AddNode(segmentsNode, "Segment");

            builder.AddNode(segmentNode, "Name", split.first.c_str());
            builder.AddNode(segmentNode, "Icon");

            rapidxml::xml_node<>* splitTimesNode = builder.AddNode(segmentNode, "SplitTimes");
            rapidxml::xml_node<>* splitTimeNode = builder.AddNode(splitTimesNode, "SplitTime");
            builder.AddAttribute(splitTimeNode, "name", "Personal Best");
            builder.AddNode(segmentNode, "BestSegmentTime");
            builder.AddNode(segmentNode, "SegmentHistory");
        }

        builder.AddNode(builder.GetRootNode(), "AutoSplitterSettings");

        builder.SaveToFile(selfDirectory + PRESET_DIRECTORY + "/" + preset + ".lss");
    }

    void WriteLayoutXML(const std::string& preset, int numSplits)
    {
        Walnut::XMLBuilder builder("1.0", "Layout");

        rapidxml::xml_node<>* root = builder.GetRootNode();
        builder.AddAttribute(root, "version", "1.6.1");

        builder.AddNode(root, "Mode", "Vertical");
        builder.AddNode(root, "X", "0");
        builder.AddNode(root, "Y", "0");
        builder.AddNode(root, "VerticalWidth", "300");
        builder.AddNode(root, "VerticalHeight", "400");
        builder.AddNode(root, "HorizontalWidth", "-1");
        builder.AddNode(root, "HorizontalHeight", "-1");

        rapidxml::xml_node<>* settingsNode = builder.AddNode(root, "Settings");
        builder.AddNode(settingsNode, "TextColor", "FFFFFFFF");
        builder.AddNode(settingsNode, "BackgroundColor", "FF0F0F0F");
        builder.AddNode(settingsNode, "BackgroundColor2", "00000000");
        builder.AddNode(settingsNode, "ThinSeparatorsColor", "03FFFFFF");
        builder.AddNode(settingsNode, "SeparatorsColor", "24FFFFFF");
        builder.AddNode(settingsNode, "PersonalBestColor", "FF16A6FF");
        builder.AddNode(settingsNode, "AheadGainingTimeColor", "FF00CC36");
        builder.AddNode(settingsNode, "AheadLosingTimeColor", "FF52CC73");
        builder.AddNode(settingsNode, "BehindGainingTimeColor", "FFCC5C52");
        builder.AddNode(settingsNode, "BehindLosingTimeColor", "FFCC1200");
        builder.AddNode(settingsNode, "BestSegmentColor", "FFD8AF1F");
        builder.AddNode(settingsNode, "UseRainbowColor", "False");
        builder.AddNode(settingsNode, "NotRunningColor", "FFACACAC");
        builder.AddNode(settingsNode, "PausedColor", "FF7A7A7A");
        builder.AddNode(settingsNode, "TextOutlineColor", "00000000");
        builder.AddNode(settingsNode, "ShadowsColor", "00000000");
        rapidxml::xml_node<>* timesFontNode = builder.AddNode(settingsNode, "TimesFont");
        rapidxml::xml_node<>* timesCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        timesCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAAhTZWdvZSBVSQAAQEEF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAABAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAwAAAAs="));
        timesFontNode->append_node(timesCDATA);
        rapidxml::xml_node<>* timerFontNode = builder.AddNode(settingsNode, "TimerFont");
        rapidxml::xml_node<>* timerCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        timerCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAA5DZW50dXJ5IEdvdGhpYwAAL0IF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAABAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAgAAAAs="));
        timerFontNode->append_node(timerCDATA);
        rapidxml::xml_node<>* textFontNode = builder.AddNode(settingsNode, "TextFont");
        rapidxml::xml_node<>* textCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        textCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAAhTZWdvZSBVSQAAQEEF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAAAAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAwAAAAs="));
        textFontNode->append_node(textCDATA);
        builder.AddNode(settingsNode, "AlwaysOnTop", "True");
        builder.AddNode(settingsNode, "ShowBestSegments", "True");
        builder.AddNode(settingsNode, "AntiAliasing", "True");
        builder.AddNode(settingsNode, "DropShadows", "True");
        builder.AddNode(settingsNode, "BackgroundType", "SolidColor");
        builder.AddNode(settingsNode, "BackgroundImage");
        builder.AddNode(settingsNode, "ImageOpacity", "1");
        builder.AddNode(settingsNode, "ImageBlur", "0");
        builder.AddNode(settingsNode, "Opacity", "1");
        builder.AddNode(settingsNode, "MousePassThroughWhileRunning", "False");

        rapidxml::xml_node<>* componentsNode = builder.AddNode(root, "Components");

        rapidxml::xml_node<>* titleComponent = builder.AddNode(componentsNode, "Component");
        builder.AddNode(titleComponent, "Path", "LiveSplit.Title.dll");
        rapidxml::xml_node<>* titleSettings = builder.AddNode(titleComponent, "Settings");
        builder.AddNode(titleSettings, "Version", "1.7.3");
        builder.AddNode(titleSettings, "ShowGameName", "True");
        builder.AddNode(titleSettings, "ShowCategoryName", "True");
        builder.AddNode(titleSettings, "ShowAttemptCount", "True");
        builder.AddNode(titleSettings, "ShowFinishedRunsCount", "False");
        builder.AddNode(titleSettings, "OverrideTitleFont", "False");
        builder.AddNode(titleSettings, "OverrideTitleColor", "False");
        rapidxml::xml_node<>* titleFontNode = builder.AddNode(titleSettings, "TitleFont");
        rapidxml::xml_node<>* titleCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        titleCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAAhTZWdvZSBVSQAAUEEF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAAAAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAgAAAAs="));
        titleFontNode->append_node(titleCDATA);
        builder.AddNode(titleSettings, "SingleLine", "False");
        builder.AddNode(titleSettings, "TitleColor", "FFFFFFFF");
        builder.AddNode(titleSettings, "BackgroundColor", "FF2A2A2A");
        builder.AddNode(titleSettings, "BackgroundColor2", "FF131313");
        builder.AddNode(titleSettings, "BackgroundGradient", "Vertical");
        builder.AddNode(titleSettings, "DisplayGameIcon", "True");
        builder.AddNode(titleSettings, "ShowRegion", "False");
        builder.AddNode(titleSettings, "ShowPlatform", "False");
        builder.AddNode(titleSettings, "ShowVariables", "True");
        builder.AddNode(titleSettings, "TextAlignment", "0");

        rapidxml::xml_node<>* splitsComponent = builder.AddNode(componentsNode, "Component");
        builder.AddNode(splitsComponent, "Path", "LiveSplit.Splits.dll");
        rapidxml::xml_node<>* splitsSettings = builder.AddNode(splitsComponent, "Settings");
        builder.AddNode(splitsSettings, "Version", "1.6");
        builder.AddNode(splitsSettings, "CurrentSplitTopColor", "FF3373F4");
        builder.AddNode(splitsSettings, "CurrentSplitBottomColor", "FF153574");
        builder.AddNode(splitsSettings, "VisualSplitCount", std::to_string(min(numSplits, 6)));
        builder.AddNode(splitsSettings, "SplitPreviewCount", "1");
        builder.AddNode(splitsSettings, "DisplayIcons", "True");
        builder.AddNode(splitsSettings, "ShowThinSeparators", "True");
        builder.AddNode(splitsSettings, "AlwaysShowLastSplit", "True");
        builder.AddNode(splitsSettings, "SplitWidth", "20");
        builder.AddNode(splitsSettings, "SplitTimesAccuracy", "Hundredths");
        builder.AddNode(splitsSettings, "AutomaticAbbreviations", "False");
        builder.AddNode(splitsSettings, "BeforeNamesColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "CurrentNamesColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "AfterNamesColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "OverrideTextColor", "False");
        builder.AddNode(splitsSettings, "BeforeTimesColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "CurrentTimesColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "AfterTimesColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "OverrideTimesColor", "False");
        builder.AddNode(splitsSettings, "ShowBlankSplits", "True");
        builder.AddNode(splitsSettings, "LockLastSplit", "True");
        builder.AddNode(splitsSettings, "IconSize", "24");
        builder.AddNode(splitsSettings, "IconShadows", "True");
        builder.AddNode(splitsSettings, "SplitHeight", "3.6");
        builder.AddNode(splitsSettings, "CurrentSplitGradient", "Vertical");
        builder.AddNode(splitsSettings, "BackgroundColor", "00FFFFFF");
        builder.AddNode(splitsSettings, "BackgroundColor2", "01FFFFFF");
        builder.AddNode(splitsSettings, "BackgroundGradient", "Alternating");
        builder.AddNode(splitsSettings, "SeparatorLastSplit", "True");
        builder.AddNode(splitsSettings, "DeltasAccuracy", "Tenths");
        builder.AddNode(splitsSettings, "DropDecimals", "True");
        builder.AddNode(splitsSettings, "OverrideDeltasColor", "False");
        builder.AddNode(splitsSettings, "DeltasColor", "FFFFFFFF");
        builder.AddNode(splitsSettings, "Display2Rows", "False");
        builder.AddNode(splitsSettings, "ShowColumnLabels", "False");
        builder.AddNode(splitsSettings, "LabelsColor", "FFFFFFFF");

        rapidxml::xml_node<>* splitsColumnsNode = builder.AddNode(splitsSettings, "Columns");

        rapidxml::xml_node<>* splitsColumnsSettingsDeltaNode = builder.AddNode(splitsColumnsNode, "Settings");
        builder.AddNode(splitsColumnsSettingsDeltaNode, "Version", "1.5");
        builder.AddNode(splitsColumnsSettingsDeltaNode, "Name", "+/-");
        builder.AddNode(splitsColumnsSettingsDeltaNode, "Type", "Delta");
        builder.AddNode(splitsColumnsSettingsDeltaNode, "Comparison", "Current Comparison");
        builder.AddNode(splitsColumnsSettingsDeltaNode, "TimingMethod", "Current Timing Method");

        rapidxml::xml_node<>* splitsColumnsSettingsTimeNode = builder.AddNode(splitsColumnsNode, "Settings");
        builder.AddNode(splitsColumnsSettingsTimeNode, "Version", "1.5");
        builder.AddNode(splitsColumnsSettingsTimeNode, "Name", "Time");
        builder.AddNode(splitsColumnsSettingsTimeNode, "Type", "SplitTime");
        builder.AddNode(splitsColumnsSettingsTimeNode, "Comparison", "Current Comparison");
        builder.AddNode(splitsColumnsSettingsTimeNode, "TimingMethod", "Current Timing Method");

        rapidxml::xml_node<>* prevSegComponent = builder.AddNode(componentsNode, "Component");
        builder.AddNode(prevSegComponent, "Path", "LiveSplit.PreviousSegment.dll");
        rapidxml::xml_node<>* prevSegSettings = builder.AddNode(prevSegComponent, "Settings");
        builder.AddNode(prevSegSettings, "Version", "1.6");
        builder.AddNode(prevSegSettings, "TextColor", "FFFFFFFF");
        builder.AddNode(prevSegSettings, "OverrideTextColor", "False");
        builder.AddNode(prevSegSettings, "BackgroundColor", "00FFFFFF");
        builder.AddNode(prevSegSettings, "BackgroundColor2", "00FFFFFF");
        builder.AddNode(prevSegSettings, "BackgroundGradient", "Plain");
        builder.AddNode(prevSegSettings, "DeltaAccuracy", "Tenths");
        builder.AddNode(prevSegSettings, "DropDecimals", "True");
        builder.AddNode(prevSegSettings, "Comparison", "Personal Best");
        builder.AddNode(prevSegSettings, "Display2Rows", "False");
        builder.AddNode(prevSegSettings, "ShowPossibleTimeSave", "False");
        builder.AddNode(prevSegSettings, "TimeSaveAccuracy", "Tenths");

        rapidxml::xml_node<>* runPredComponent = builder.AddNode(componentsNode, "Component");
        builder.AddNode(runPredComponent, "Path", "LiveSplit.RunPrediction.dll");
        rapidxml::xml_node<>* runPredSettings = builder.AddNode(runPredComponent, "Settings");
        builder.AddNode(runPredSettings, "Version", "1.4");
        builder.AddNode(runPredSettings, "TextColor", "FFFFFFFF");
        builder.AddNode(runPredSettings, "OverrideTextColor", "False");
        builder.AddNode(runPredSettings, "TimeColor", "FFFFFFFF");
        builder.AddNode(runPredSettings, "OverrideTimeColor", "False");
        builder.AddNode(runPredSettings, "Accuracy", "Seconds");
        builder.AddNode(runPredSettings, "BackgroundColor", "00FFFFFF");
        builder.AddNode(runPredSettings, "BackgroundColor2", "00FFFFFF");
        builder.AddNode(runPredSettings, "BackgroundGradient", "Plain");
        builder.AddNode(runPredSettings, "Comparison", "Best Segments");
        builder.AddNode(runPredSettings, "Display2Rows", "False");

        rapidxml::xml_node<>* sobComponent = builder.AddNode(componentsNode, "Component");
        builder.AddNode(sobComponent, "Path", "LiveSplit.SumOfBest.dll");
        rapidxml::xml_node<>* sobSettings = builder.AddNode(sobComponent, "Settings");
        builder.AddNode(sobSettings, "Version", "1.4");
        builder.AddNode(sobSettings, "TextColor", "FFFFFFFF");
        builder.AddNode(sobSettings, "OverrideTextColor", "False");
        builder.AddNode(sobSettings, "TimeColor", "FFFFFFFF");
        builder.AddNode(sobSettings, "OverrideTimeColor", "False");
        builder.AddNode(sobSettings, "Accuracy", "Seconds");
        builder.AddNode(sobSettings, "BackgroundColor", "00FFFFFF");
        builder.AddNode(sobSettings, "BackgroundColor2", "00FFFFFF");
        builder.AddNode(sobSettings, "BackgroundGradient", "Plain");
        builder.AddNode(sobSettings, "Display2Rows", "False");

        rapidxml::xml_node<>* detailedTimerComponent = builder.AddNode(componentsNode, "Component");
        builder.AddNode(detailedTimerComponent, "Path", "LiveSplit.DetailedTimer.dll");
        rapidxml::xml_node<>* detailedTimerSettings = builder.AddNode(detailedTimerComponent, "Settings");
        builder.AddNode(detailedTimerSettings, "Version", "1.5");
        builder.AddNode(detailedTimerSettings, "Height", "75");
        builder.AddNode(detailedTimerSettings, "Width", "200");
        builder.AddNode(detailedTimerSettings, "SegmentTimerSizeRatio", "40");
        builder.AddNode(detailedTimerSettings, "TimerShowGradient", "True");
        builder.AddNode(detailedTimerSettings, "OverrideTimerColors", "False");
        builder.AddNode(detailedTimerSettings, "SegmentTimerShowGradient", "True");
        builder.AddNode(detailedTimerSettings, "TimerFormat", "1.23");
        builder.AddNode(detailedTimerSettings, "SegmentTimerFormat", "1.23");
        builder.AddNode(detailedTimerSettings, "SegmentTimesAccuracy", "Hundredths");
        builder.AddNode(detailedTimerSettings, "TimerColor", "FFAAAAAA");
        builder.AddNode(detailedTimerSettings, "SegmentTimerColor", "FFAAAAAA");
        builder.AddNode(detailedTimerSettings, "SegmentLabelsColor", "FFFFFFFF");
        builder.AddNode(detailedTimerSettings, "SegmentTimesColor", "FFFFFFFF");
        rapidxml::xml_node<>* segmentLabelsNode = builder.AddNode(detailedTimerSettings, "SegmentLabelsFont");
        rapidxml::xml_node<>* segmentLabelsCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        segmentLabelsCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAAhTZWdvZSBVSQAAIEEF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAAAAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAwAAAAs="));
        segmentLabelsNode->append_node(segmentLabelsCDATA);
        rapidxml::xml_node<>* segmentTimesNode = builder.AddNode(detailedTimerSettings, "SegmentTimesFont");
        rapidxml::xml_node<>* segmentTimesCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        segmentTimesCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAAhTZWdvZSBVSQAAIEEF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAABAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAwAAAAs="));
        segmentTimesNode->append_node(segmentTimesCDATA);
        rapidxml::xml_node<>* splitNameNode = builder.AddNode(detailedTimerSettings, "SplitNameFont");
        rapidxml::xml_node<>* splitNameCDATA = builder.GetDocument()->allocate_node(rapidxml::node_cdata);
        splitNameCDATA->value(builder.GetDocument()->allocate_string("AAEAAAD/////AQAAAAAAAAAMAgAAAFFTeXN0ZW0uRHJhd2luZywgVmVyc2lvbj00LjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPWIwM2Y1ZjdmMTFkNTBhM2EFAQAAABNTeXN0ZW0uRHJhd2luZy5Gb250BAAAAAROYW1lBFNpemUFU3R5bGUEVW5pdAEABAQLGFN5c3RlbS5EcmF3aW5nLkZvbnRTdHlsZQIAAAAbU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AgAAAAIAAAAGAwAAAAhTZWdvZSBVSQAAMEEF/P///xhTeXN0ZW0uRHJhd2luZy5Gb250U3R5bGUBAAAAB3ZhbHVlX18ACAIAAAAAAAAABfv///8bU3lzdGVtLkRyYXdpbmcuR3JhcGhpY3NVbml0AQAAAAd2YWx1ZV9fAAgCAAAAAwAAAAs="));
        splitNameNode->append_node(splitNameCDATA);
        builder.AddNode(detailedTimerSettings, "DisplayIcon", "False");
        builder.AddNode(detailedTimerSettings, "IconSize", "40");
        builder.AddNode(detailedTimerSettings, "ShowSplitName", "False");
        builder.AddNode(detailedTimerSettings, "SplitNameColor", "FFFFFFFF");
        builder.AddNode(detailedTimerSettings, "BackgroundColor", "00FFFFFF");
        builder.AddNode(detailedTimerSettings, "BackgroundColor2", "00FFFFFF");
        builder.AddNode(detailedTimerSettings, "BackgroundGradient", "Plain");
        builder.AddNode(detailedTimerSettings, "Comparison", "Current Comparison");
        builder.AddNode(detailedTimerSettings, "Comparison2", "Best Segments");
        builder.AddNode(detailedTimerSettings, "HideComparison", "False");
        builder.AddNode(detailedTimerSettings, "TimingMethod", "Current Timing Method");
        builder.AddNode(detailedTimerSettings, "DecimalsSize", "35");
        builder.AddNode(detailedTimerSettings, "SegmentTimerDecimalsSize", "35");

        builder.SaveToFile(selfDirectory + PRESET_DIRECTORY + "/" + preset + ".lsl");
    }

    SplitPreset ParseSplitJson(std::string_view filename)
    {
        SplitPreset returnPreset;
        Walnut::JSONBuilder builder(filename);

        if (builder.GetDocument().HasMember("Settings") && builder.GetDocument()["Settings"].IsObject())
        {
            const rapidjson::Value& settings = builder.GetDocument()["Settings"];

            for (rapidjson::Value::ConstMemberIterator it = settings.MemberBegin(); it != settings.MemberEnd(); it++)
            {
                const std::string& key = it->name.GetString();
                const rapidjson::Value& value = it->value;

                switch (hashstr(key.c_str()))
                {
                case hashstr("In Game Timer"):
                    if (value.IsBool())
                        returnPreset.igt = value.GetBool();
                    break;
                case hashstr("In Game Round Timer"):
                    if (value.IsBool())
                        returnPreset.igrt = value.GetBool();
                    break;
                case hashstr("Amount of Splits"):
                    if (value.IsInt())
                        returnPreset.numSplits = value.GetInt();
                    break;
                case hashstr("Map Index"):
                    if (value.IsInt())
                        returnPreset.map = value.GetInt();
                    break;
                case hashstr("Split Type"):
                    if (value.IsInt())
                        returnPreset.splitType = value.GetInt();
                    break;
                default:
                    break;
                }
            }
        }
        if (builder.GetDocument().HasMember("Split Data") && builder.GetDocument()["Split Data"].IsObject())
        {
            const rapidjson::Value& splitData = builder.GetDocument()["Split Data"];

            for (rapidjson::Value::ConstMemberIterator it = splitData.MemberBegin(); it != splitData.MemberEnd(); it++)
            {
                if (it->name.IsString() && it->value.IsInt())
                    returnPreset.splits.push_back({ it->name.GetString(), it->value.GetInt() });
                else
                    LogFile("Autosplits json error: Incorrect typings found at key " + std::string(it->name.GetString()));
            }
        }

        return returnPreset;
    }
#pragma endregion
}