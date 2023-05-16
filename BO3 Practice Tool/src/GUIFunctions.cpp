#include "GUIFunctions.h"
#include "Walnut/Application.h"

#include "imgui.h"
#include "imgui_internal.h"
#define CURL_STATICLIB
#include <curl/curl.h>
#include <miniz/miniz.h>
#include <algorithm>

#include "PlayerOptions.h"
#include "ZombieOptions.h"
#include "PowerupOptions.h"
#include "Craftables.h"
#include "Resources.h"

using namespace BGB;
using namespace Weapons;
using namespace Perks;
using namespace PlayerOptions;
using namespace ZombieOptions;
using namespace PowerupOptions;
using namespace Craftables;
using namespace ZombieCalc;
using namespace GKValveSolver;
using namespace SOECodeGuide;
using namespace IceCodePractice;
using namespace Autosplits;
using namespace JSON;

static size_t WriteToFile(char* ptr, size_t size, size_t nmemb, void* f)
{
    FILE* file = (FILE*)f;
    return fwrite(ptr, size, nmemb, file);
}

DWORD WaitToKillCompiler(PROCESS_INFORMATION lpExecInfo);

namespace GUIFunctions
{
    void InitVariables()
    {
        InitImages();
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
            bgbImgList.push_back(img);
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/Gum Images/Megas"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            bgbImgList.push_back(img);
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/Soe Code"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            codeImgList.push_back(img);
            soeCodeCombo.push_back(img->GetFilename());
        }
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/GK Valve Solver"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            Walnut::Image* img = new Walnut::Image(entry.path().string());
            valveSolverImgList.push_back(img);
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
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Active Autosplit Preset.txt"))
            std::ofstream(practiceToolDirectory + "\\Settings\\Active Autosplit Preset.txt");
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Practice Presets.txt"))
            std::ofstream(practiceToolDirectory + "\\Settings\\Practice Presets.txt");
        if (!DoesPathExist(selfDirectory + "\\bindings.json"))
            JSON::WriteEmptyJson(selfDirectory + "\\bindings.json");
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
        if (CreateProcessA(NULL, args, NULL, NULL, TRUE, NULL, NULL, NULL, &startupInfo, &processInfo))
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
        /*if (name.find(ICON_FA_ARROW_LEFT) != name.npos && KeyBinds::KeyPressed(VK_ESCAPE, false))
        {
            ImGui::PopStyleColor(2);
            return 1;
        }*/

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

    bool CreateGumImages(const std::vector<int>& gumArr, const ImVec2& imgSize, int numOnLine, const std::string& type, const std::function<void(int input)>& funcOnPress)
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
                gumContextIndex = index;
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
            newVersionParts.push_back(std::stoi(part));
        while (std::getline(oldVersionStream, part, '.'))
            oldVersionParts.push_back(std::stoi(part));

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
}

std::mutex logMutex;
void LogFile(const std::string& text, bool initialBoot)
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream logFile;
    if (initialBoot)
        logFile.open(selfDirectory + "/log.txt");
    else
        logFile.open(selfDirectory + "/log.txt", std::ios::app);
    logFile << text << "\n";
    logFile.flush();
    logFile.close();
}

std::mutex nlogMutex;
void NLog(const std::string& text)
{
    std::lock_guard<std::mutex> lock(nlogMutex);

    HWND notepad, edit;
    notepad = FindWindowA(NULL, "Untitled - Notepad");
    if (!notepad)
    {
        notepad = FindWindowA(NULL, "*Untitled - Notepad");
    }
    edit = FindWindowExA(notepad, NULL, "EDIT", NULL);
    SendMessage(edit, EM_REPLACESEL, TRUE, (LPARAM)text.c_str());
    SendMessage(edit, EM_REPLACESEL, TRUE, (LPARAM)"\n");
}

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