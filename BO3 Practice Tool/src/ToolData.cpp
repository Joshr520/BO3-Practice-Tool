#include "ToolData.h"
#include "GlobalData.h"
#include "Fonts/Icons.h"
#include "Walnut/Random.h"
#include "Walnut/FileFormats/xml.h"
#include "Walnut/FileFormats/json.h"
#include "Walnut/Logger.h"
#include "Keybinds.h"
#include "GUIState.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <miniz/miniz.h>

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <thread>
#include <set>
#include <regex>

using namespace BO3PT;

DWORD WaitToKillCompiler(PROCESS_INFORMATION processInfo)
{
    DWORD exitCode = 0;
    DWORD result = WaitForSingleObject(processInfo.hProcess, 3000);
    if (result == WAIT_TIMEOUT)
    {
        Walnut::Logger::Log(Walnut::MessageType::Info, "Killing compiler for timeout");
        TerminateProcess(processInfo.hProcess, 0);
    }
    else if (result == WAIT_OBJECT_0)
    {
        if (!GetExitCodeProcess(processInfo.hProcess, &exitCode))
        {
            Walnut::Logger::Log(Walnut::MessageType::Error, "GetExitCodeProcess failed with error code: " + GetLastError());
        }
    }
    else
    {
        Walnut::Logger::Log(Walnut::MessageType::Error, "WaitForSingleObject failed with error code: " + GetLastError());
    }
    return exitCode;
}

static size_t WriteToFile(char* ptr, size_t size, size_t nmemb, void* f)
{
    FILE* file = (FILE*)f;
    return fwrite(ptr, size, nmemb, file);
}

static bool bgbLoaded = false;
static bool codesLoaded = false;
static bool buildKitsLoaded = false;

namespace BO3PT
{
#pragma region GUIFunctions
    void InitVariables()
    {
        InitHotKeyBinds();
        LoadGumProfiles();
        LoadAutosplitPresets();
        LoadWeaponProfiles();
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
        CalcValveProbabilities();
        // Currently only 1 image is loaded for valves, so I'm not bothering to load and unload it for ram benefits
        for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "/Resource Images/GK Valve Solver"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == name.npos)
                continue;
            valveSolverImgList.emplace_back(std::make_unique<Walnut::Image>(entry.path().string()));
        }
    }

    void LoadImages(RenderWindow window)
    {
        switch (window) {
        case BGBLoadout:
        case GumTracker: {
            if (codesLoaded) {
                soeCodeImgList.clear();
                soeCodeCombo.clear();
                iceCodeImgList.clear();
                iceCodePairs.clear();
                randomIceCodePairs.clear();
                codesLoaded = false;
            }
            if (buildKitsLoaded) {
                buildKitImgList.clear();
                camosImgList.clear();
                weaponIconsImgList.clear();
                opticsImgList.clear();
                attachmentsImgList.clear();
                buildKitsLoaded = false;
            }
            if (bgbLoaded) {
                break;
            }

            std::thread classicsThread([&]() {
                for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Gum Images\\Classics")) {
                    if (entry.path().extension().string() != ".png") {
                        continue;
                    }
                    bgbImgList.insert({ entry.path().stem().string(), std::make_unique<Walnut::Image>(entry.path().string())});
                }
                });

            std::thread megasThread([&]() {
                for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Gum Images\\Megas")) {
                    if (entry.path().extension().string() != ".png") {
                        continue;
                    }
                    bgbImgList.insert({ entry.path().stem().string(), std::make_unique<Walnut::Image>(entry.path().string())});
                }
                });

            classicsThread.join();
            megasThread.join();
            bgbLoaded = true;
            break;
        }
        case WeaponLoadout: {
            if (bgbLoaded) {
                bgbImgList.clear();
                bgbLoaded = false;
            }
            if (codesLoaded) {
                soeCodeImgList.clear();
                soeCodeCombo.clear();
                iceCodeImgList.clear();
                iceCodePairs.clear();
                randomIceCodePairs.clear();
                codesLoaded = false;
            }
            if (buildKitsLoaded) {
                break;
            }

            std::thread buildKitThread([&]() {
                for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Weapons\\Build Kits")) {
                    if (entry.path().extension().string() != ".png") {
                        continue;
                    }
                    buildKitImgList.emplace_back(std::make_unique<Walnut::Image>(entry.path().string()));
                }
                });

            std::thread camosThread([&]() {
                for (const auto& directory : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Weapons\\Camos")) {
                    if (std::filesystem::is_directory(directory.path())) {
                        WeaponCamoGroup camoGroup;
                        camoGroup.m_Name = directory.path().filename().string();
                        for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
                            if (file.path().extension().string() != ".png") {
                                continue;
                            }
                            camoGroup.m_Camos.emplace_back(std::make_unique<Walnut::Image>(file.path().string()));
                        }
                        camosImgList.emplace_back(std::move(camoGroup));
                    }
                }
                });

            std::thread weaponIconsThread([&]() {
                for (const auto& directory : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Weapons\\Icons")) {
                    if (std::filesystem::is_directory(directory.path())) {
                        for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
                            if (file.path().extension().string() != ".png") {
                                continue;
                            }
                            weaponIconsImgList.insert({ file.path().stem().string(), std::make_unique<Walnut::Image>(file.path().string()) });
                        }
                    }
                }
                });

            std::thread opticsThread([&]() {
                for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Weapons\\Attachments\\Optics")) {
                    if (entry.path().extension().string() != ".png") {
                        continue;
                    }
                    opticsImgList.insert({ entry.path().stem().string(), std::make_unique<Walnut::Image>(entry.path().string()) });
                }
                });

            std::thread attachmentsThread([&]() {
                for (const auto& directory : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Weapons\\Attachments")) {
                    if (std::filesystem::is_directory(directory.path())) {
                        for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
                            if (file.path().extension().string() != ".png") {
                                continue;
                            }
                            attachmentsImgList.insert({ file.path().stem().string(), std::make_unique<Walnut::Image>(file.path().string()) });
                        }
                    }
                }
                });

            buildKitThread.join();
            camosThread.join();
            weaponIconsThread.join();
            opticsThread.join();
            attachmentsThread.join();
            buildKitsLoaded = true;
            break;
        }
        case CodeGuides: {
            if (bgbLoaded) {
                bgbImgList.clear();
                bgbLoaded = false;
            }
            if (buildKitsLoaded) {
                buildKitImgList.clear();
                camosImgList.clear();
                weaponIconsImgList.clear();
                opticsImgList.clear();
                attachmentsImgList.clear();
                buildKitsLoaded = false;
            }
            if (codesLoaded) {
                break;
            }

            std::thread soeCodeThread([&]() {
                for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Soe Code")) {
                    if (entry.path().extension().string() != ".png") {
                        continue;
                    }
                    std::shared_ptr<Walnut::Image> img = std::make_shared<Walnut::Image>(entry.path().string());
                    soeCodeImgList.emplace_back(img);
                    soeCodeCombo.emplace_back(img->GetFilename());
                }
                });

            std::thread iceCodeThread([&]() {
                for (const auto& entry : std::filesystem::directory_iterator(selfDirectory + "\\Resource Images\\Ice Code")) {
                    if (entry.path().extension().string() != ".png") {
                        continue;
                    }
                    std::shared_ptr<Walnut::Image> img = std::make_shared<Walnut::Image>(entry.path().string());
                    iceCodeImgList.insert({ img->GetFilename(), img });
                }
                });

            soeCodeThread.join();
            iceCodeThread.join();
            LoadIceCodePairs();
            codesLoaded = true;
            break;
        }
        default:
            if (bgbLoaded) {
                bgbImgList.clear();
                bgbLoaded = false;
            }
            if (codesLoaded) {
                soeCodeImgList.clear();
                soeCodeCombo.clear();
                iceCodeImgList.clear();
                iceCodePairs.clear();
                randomIceCodePairs.clear();
                codesLoaded = false;
            }
            if (buildKitsLoaded) {
                buildKitImgList.clear();
                camosImgList.clear();
                weaponIconsImgList.clear();
                opticsImgList.clear();
                attachmentsImgList.clear();
                buildKitsLoaded = false;
            }
            break;
        }
    }

    void HelpMarker(const std::string& text)
    {
        ImGui::TextDisabled(ICON_FA_CIRCLE_QUESTION);
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextWrapped(text.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void VerifyFileStructure()
    {
        std::string practiceToolDirectory = bo3Directory + "\\Practice Tool";
        std::string gscDirectory = practiceToolDirectory + "\\GSC";
        std::string settingsFolder = practiceToolDirectory + "\\Settings";
        if (!DoesPathExist(practiceToolDirectory)) {
            std::filesystem::create_directory(practiceToolDirectory);
        }
        if (!DoesPathExist(gscDirectory)) {
            std::filesystem::create_directory(gscDirectory);
        }
        if (!DoesPathExist(settingsFolder)) {
            std::filesystem::create_directory(settingsFolder);
        }
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Active Gum Preset.txt")) {
            std::ofstream(practiceToolDirectory + "\\Settings\\Active Gum Preset.txt");
        }
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Active Autosplit Preset.json")) {
            std::ofstream(practiceToolDirectory + "\\Settings\\Active Autosplit Preset.json");
        }
        if (!DoesPathExist(practiceToolDirectory + "\\Settings\\Practice Presets.txt")) {
            std::ofstream(practiceToolDirectory + "\\Settings\\Practice Presets.txt");
        }
        if (!DoesPathExist(selfDirectory + "\\bindings.json")) {
            Walnut::JSONBuilder::WriteEmpty(selfDirectory + "\\bindings.json");
        }
        if (!DoesPathExist(selfDirectory + "\\Settings")) {
            std::filesystem::create_directory(selfDirectory + "\\Settings");
        }
        if (!DoesPathExist(selfDirectory + "\\Settings\\Autosplits")) {
            std::filesystem::create_directory(selfDirectory + "\\Settings\\Autosplits");
        }
        if (!DoesPathExist(selfDirectory + "\\Settings\\Gum Profiles")) {
            std::filesystem::create_directory(selfDirectory + "\\Settings\\Gum Profiles");
        }
        if (!DoesPathExist(selfDirectory + "\\Settings\\Weapon Loadouts")) {
            std::filesystem::create_directory(selfDirectory + "\\Settings\\Weapon Loadouts");
        }
        if (DoesPathExist(selfDirectory + "\\GSC")) {
            std::string startDirectory = selfDirectory + "\\GSC";
            for (const auto& entry : std::filesystem::directory_iterator(startDirectory))
            {
                if (entry.is_directory() || entry.path().filename().string() == "." || entry.path().filename().string() == "..")
                    continue;
                std::string name = entry.path().filename().string();
                std::string oldFile = entry.path().string();
                std::string newFile = gscDirectory + "\\" + name;
                if (!DoesPathExist(oldFile)) {
                    continue;
                }
                try {
                    std::filesystem::rename(oldFile, newFile);
                }
                catch (const std::filesystem::filesystem_error& e) {
                    Walnut::Logger::Log(Walnut::MessageType::Error, "Failed to move file " + oldFile + " to " + newFile + " with error message: " + std::string(e.what()));
                    std::filesystem::remove(oldFile);
                }
            }
            if (!std::filesystem::remove(startDirectory)) {
                Walnut::Logger::Log(Walnut::MessageType::Error, "Couldn't remove GSC directory with error code: " + std::error_code(errno, std::system_category()).message());
            }
        }
    }

    void WritePracticePatches(bool active)
    {
        std::string data;
        
        if (active) {
            for (const MapPracticePatch& list : practicePatches.m_PracticePatches) {
                data.append(std::to_string(list.m_Index - 1) + ",");
            }
        }
        else {
            for (const MapPracticePatch& list : practicePatches.m_PracticePatches) {
                data.append("-1,");
            }
        }
        data.replace(data.size() - 1, 1, "");
        std::ofstream file(bo3Directory + "\\Practice Tool\\Settings\\Practice Presets.txt");
        file << data;
        file.close();
    }

    void NotifyGame(const std::vector<int>& passList)
    {
        if (!GUIState::IsStateSet(Active)) {
            return;
        }
        std::string outData;
        for (int i = 0; i < passList.size(); i++) {
            outData.append(std::to_string(passList[i]) + " ");
        }

        HANDLE pipe = CreateFileA("\\\\.\\pipe\\t7Compiler", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (!pipe) {
            return;
        }

        WriteFile(pipe, outData.c_str(), static_cast<DWORD>(outData.size()), nullptr, NULL);
        CloseHandle(pipe);
    }

    void InjectTool(bool enable)
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
                GUIState::UnsetState(Active);
                ImGuiHelper::PopupWrapper::PrepPopup(ImGuiHelper::InjectFailed);
                Walnut::Logger::Log(Walnut::MessageType::Error, "Required files to inject missing");
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
                Walnut::Logger::Log(Walnut::MessageType::Error, "Compiler process failed with error code: " + std::to_string(exitCode));
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }
        else
        {
            Walnut::Logger::Log(Walnut::MessageType::Error, "Failed to inject tool with error code: " + GetLastError());
            GUIState::UnsetState(Active);
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

    bool DoesPathExist(std::string_view s)
    {
        return std::filesystem::exists(s);
    }

    bool CheckVersions(const std::string& newVersion, std::string_view currentVersion)
    {
        std::regex regex("\\d+\\.\\d+\\.\\d+");
        std::vector<int> currentNumbers;
        std::vector<int> newNumbers;

        std::smatch match;
        if (std::regex_search(newVersion, match, regex)) {
            std::string numericVersion = match.str();

            std::stringstream currentStream(currentVersion.data());
            std::stringstream newStream(numericVersion);

            std::string segment;
            while (std::getline(currentStream, segment, '.')) {
                currentNumbers.emplace_back(std::stoi(segment));
            }

            while (std::getline(newStream, segment, '.')) {
                newNumbers.emplace_back(std::stoi(segment));
            }

            for (int i = 0; i < static_cast<int>(currentNumbers.size()) && i < static_cast<int>(newNumbers.size()); ++i) {
                if (newNumbers[i] > currentNumbers[i]) {
                    return true;
                }
                else if (newNumbers[i] < currentNumbers[i]) {
                    return false;
                }
            }
        }

        return newNumbers.size() > currentNumbers.size();
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
            Walnut::Logger::Log(Walnut::MessageType::Error, "Opening file " + filename + " failed with error code: " + errorMsg);
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
            Walnut::Logger::Log(Walnut::MessageType::Error, "curl download failed with error code: " + res);
            return 0;
        }

        fclose(file);

        std::filesystem::path output_directory("./");
        mz_zip_archive zip_archive;
        mz_zip_zero_struct(&zip_archive);
        if (!mz_zip_reader_init_file(&zip_archive, filename.c_str(), 0))
        {
            Walnut::Logger::Log(Walnut::MessageType::Error, "Failed to open zip file: " + filename);
            return 0;
        }

        int num_files = mz_zip_reader_get_num_files(&zip_archive);
        Walnut::Logger::Log(Walnut::MessageType::Info, "Extracting " + num_files + std::string(" files from ") + filename + " to " + output_directory.string());

        for (int i = 0; i < num_files; i++)
        {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
            {
                Walnut::Logger::Log(Walnut::MessageType::Error, "Failed to get file stat for index " + i);
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

            Walnut::Logger::Log(Walnut::MessageType::Info, "Extracting " + std::string(file_stat.m_filename) + " to " + output_file_path.string());

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
                    Walnut::Logger::Log(Walnut::MessageType::Error, "Failed to extract file " + std::string(file_stat.m_filename));
                    return 0;
                }
            }
        }
        Walnut::Logger::Log(Walnut::MessageType::Success, "Extraction completed successfully");

        mz_zip_reader_end(&zip_archive);
        std::filesystem::remove(filename);

        if (!ptexe.empty())
        {
            STARTUPINFOA startupInfo = { sizeof(startupInfo) };
            PROCESS_INFORMATION processInfo = { 0 };
            LPSTR args = &ptexe[0];
            if (!CreateProcessA(NULL, args, NULL, NULL, TRUE, NULL, NULL, NULL, &startupInfo, &processInfo))
                Walnut::Logger::Log(Walnut::MessageType::Error, "Failed to start new practice tool exe");
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
            done = 1;
        }
        return 1;
    }

    std::vector<BGB> GumSearch(int type, std::string_view searchText)
    {
        if (searchText.empty()) {
            if (type) {
                return bgbs.m_Megas;
            }
            return bgbs.m_Classics;
        }

        const std::vector<BGB>* bgbCompare;
        type ? bgbCompare = &bgbs.m_Megas : bgbCompare = &bgbs.m_Classics;

        std::vector<BGB> bgbsOut = { };

        std::string searchTextLower = searchText.data();
        std::transform(searchTextLower.begin(), searchTextLower.end(), searchTextLower.begin(), [](char c) { return std::tolower(c); });

        std::copy_if(bgbCompare->begin(), bgbCompare->end(), std::back_inserter(bgbsOut), [&searchTextLower, &bgbCompare](const BGB& bgb)
            {
                std::string inGum = bgbImgList[bgb.m_Name]->GetFilename();
                std::transform(inGum.begin(), inGum.end(), inGum.begin(), [](char c) { return std::tolower(c); });
                return inGum.find(searchTextLower) != inGum.npos;
            });

        return bgbsOut;
    }
#pragma endregion

#pragma region BGB
#define PRESET_FILE "/Settings/Gum Profiles/GumPresets.csv"

    void LoadGumProfiles()
    {
        gumPresets.clear();

        std::string line;
        std::ifstream file(selfDirectory + PRESET_FILE);

        while (getline(file, line)) {
            BGBPreset loadedPreset;
            std::stringstream sstream(line);
            std::string substr;
            getline(sstream, substr, ',');
            loadedPreset.m_Name = substr;
            for (int i = 0; i < 5; i++) {
                getline(sstream, substr, ',');
                int index = atoi(substr.c_str());
                index > 18 ? loadedPreset.m_BGBs.emplace_back(bgbs.m_Megas[index - 19]) : loadedPreset.m_BGBs.emplace_back(bgbs.m_Classics[index]);
            }
            gumPresets.emplace_back(loadedPreset);
        }
        file.close();

        if (gumPresets.size()) {
            bgbContext = gumPresets[currentGumPreset].m_BGBs[0];
        }
        //else
            //gumPresets.emplace_back(inactiveGumPreset);
    }

    void DeleteGumPreset(std::string_view presetName)
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

    void CreateNewGumPreset(std::string_view presetName)
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
        currentGumPreset = static_cast<int>(gumPresets.size()) - 1;
    }

    void WriteGumPreset(const BGBPreset& gumPreset)
    {
        std::string line;
        std::string outData;
        std::ifstream inFile(selfDirectory + PRESET_FILE);

        while (getline(inFile, line)) {
            std::stringstream sstream(line);
            std::string name;
            getline(sstream, name, ',');
            std::string preset(gumPresets[currentGumPreset].m_Name);
            if (name != preset) {
                outData.append(line + "\n");
                continue;
            }
            outData.append(preset + ",");
            for (const BGB& gum : gumPreset.m_BGBs) {
                outData.append(gum.m_Index + ",");
            }
            outData.replace(outData.size() - 1, 1, "\n");
        }
        inFile.close();

        std::ofstream outFile(selfDirectory + PRESET_FILE);
        outFile << outData;
        outFile.close();

        LoadGumProfiles();
    }

    void WritePresetToGame(const BGBPreset& gumPreset, std::string_view file)
    {
        if (gumPreset.m_Name.empty()) {
            std::ofstream(file.data()).write("-1,-1,-1,-1,-1", 13);
            return;
        }

        std::string outData;
        for (const BGB& bgb : gumPreset.m_BGBs) {
            outData.append(bgb.m_Index + ",");
        }
        outData.replace(outData.size() - 1, 1, "");
        std::ofstream outFile(file.data());
        outFile << outData;
        outFile.close();
    }

    void SwapBGBPreset(BGB bgbOld, BGB bgbNew)
    {
        for (BGB& bgb : gumPresets[currentGumPreset].m_BGBs) {
            if (bgb.m_Index == bgbNew.m_Index) {
                bgb = bgbOld;
            }
            else if (bgb.m_Index == bgbOld.m_Index) {
                bgb = bgbNew;
            }
        }

        WriteGumPreset(gumPresets[currentGumPreset]);
    }

    void SwapBGBTrack(BGB bgbOld, BGB bgbNew)
    {
        for (BGB& bgb : gumTrackBGBs) {
            if (bgb.m_Index == bgbNew.m_Index) {
                bgb = bgbOld;
            }
            else if (bgb.m_Index == bgbOld.m_Index) {
                bgb = bgbNew;
            }
        }
    }

    bool CheckPresetExists(std::string_view inPreset)
    {
        for (const BGBPreset& preset : gumPresets) {
            if (preset.m_Name == inPreset) {
                return true;
            }
        }
        return false;
    }
#pragma endregion

#pragma region WeaponLoadouts

    void LoadWeaponProfiles()
    {
        weaponPresets.clear();
        for (const auto& file : std::filesystem::directory_iterator(std::filesystem::path(selfDirectory) / "Settings\\Weapon Loadouts"))
        {
            if (std::filesystem::is_regular_file(file) && file.path().extension().string() == ".json")
            {
                MenuWeaponPreset preset = ParseWeaponLoadout(file.path().string());
                preset.m_Name = file.path().stem().string();
                weaponPresets.emplace_back(preset);
            }
        }
    }

    void CreateNewWeaponPreset(std::string_view presetName)
    {
        std::string filename = selfDirectory + "\\Settings\\Weapon Loadouts\\" + presetName.data() + ".json";

        Walnut::JSONBuilder builder;

        for (int i = 0; i < static_cast<int>(menuWeaponLists.m_WeaponTypes.size()); i++) {
            rapidjson::Value& weaponGroup = builder.AddObject(builder.GetDocument(), menuWeaponLists.m_WeaponTypes[i]);
            for (const MenuWeapon& weapon : menuWeaponLists.m_Weapons[i]) {
                rapidjson::Value& weaponObject = builder.AddObject(weaponGroup, weapon.m_Name);
                weaponObject.AddMember("Optic", -1, builder.GetAllocator());
                builder.AddArray(weaponObject, "Attachments");
                builder.AddArray(weaponObject, "Camo");
            }
        }

        builder.SaveToFile(filename);
        LoadWeaponProfiles();
    }

    void DeleteWeaponPreset(std::string_view preset)
    {
        if (std::filesystem::exists(selfDirectory + "\\Settings\\Weapon Loadouts\\" + preset.data() + ".json")) {
            std::filesystem::remove(selfDirectory + "\\Settings\\Weapon Loadouts\\" + preset.data() + ".json");
        }
        if (currentWeaponPreset > 0) {
            currentWeaponPreset--;
        }
        LoadWeaponProfiles();
    }

    MenuWeaponPreset ParseWeaponLoadout(std::string_view filename)
    {
        MenuWeaponPreset returnPreset;
        Walnut::JSONBuilder builder(filename);

        for (const std::string& weaponType : menuWeaponLists.m_WeaponTypes) {
            std::vector<MenuWeaponPresetItem> presetEntry;
            if (builder.GetDocument().HasMember(weaponType.c_str()) && builder.GetDocument()[weaponType.c_str()].IsObject()) {
                const rapidjson::Value& typeObject = builder.GetDocument()[weaponType.c_str()];

                for (rapidjson::Value::ConstMemberIterator it = typeObject.MemberBegin(); it != typeObject.MemberEnd(); it++) {
                    const std::string& name = it->name.GetString();
                    const rapidjson::Value& value = it->value;
                    MenuWeaponPresetItem weaponEntry;

                    if (value.HasMember("Optic") && value["Optic"].IsInt()) {
                        weaponEntry.m_EquippedOptic = value["Optic"].GetInt();
                    }

                    if (value.HasMember("Attachments") && value["Attachments"].IsArray()) {
                        const rapidjson::Value& attachments = value["Attachments"];

                        for (rapidjson::SizeType i = 0; i < attachments.Size(); i++) {
                            if (attachments[i].IsInt()) {
                                weaponEntry.m_EquippedAttachments.emplace_back(attachments[i].GetInt());
                            }
                        }
                    }

                    if (value.HasMember("Camo") && value["Camo"].IsArray()) {
                        const rapidjson::Value& attachments = value["Camo"];

                        for (int i = 0; i < min(static_cast<int>(attachments.Size()), 2); i++) {
                            if (attachments[i].IsInt()) {
                                weaponEntry.m_Camo[i] = (attachments[i].GetInt());
                            }
                        }
                    }

                    presetEntry.emplace_back(weaponEntry);
                }
                returnPreset.m_PresetItems.insert({ weaponType.c_str(), presetEntry });
            }
        }

        return returnPreset;
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
    void LoadIceCodePairs()
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
    void LoadAutosplitPresets()
    {
        autosplitPresets.clear();
        for (const auto& file : std::filesystem::directory_iterator(std::filesystem::path(selfDirectory) / "Settings\\Autosplits"))
        {
            if (std::filesystem::is_regular_file(file) && file.path().extension().string() == ".json")
            {
                AutosplitPreset preset = ParseSplitJson(file.path().string());
                preset.m_Name = file.path().stem().string();
                autosplitPresets.emplace_back(preset);
            }
        }
    }

    void WriteAutosplitPreset(const AutosplitPreset& preset)
    {
        std::string filename = selfDirectory + "\\Settings\\Autosplits\\" + preset.m_Name + ".json";

        Walnut::JSONBuilder builder;

        rapidjson::Value& settings = builder.AddObject(builder.GetDocument(), "Settings");
        settings.AddMember("In Game Timer", preset.m_IGT, builder.GetAllocator());
        settings.AddMember("In Game Round Timer", preset.m_IGRT, builder.GetAllocator());
        settings.AddMember("Amount of Splits", preset.m_NumSplits, builder.GetAllocator());
        settings.AddMember("Map Index", preset.m_Map, builder.GetAllocator());
        settings.AddMember("Split Type", preset.m_SplitType, builder.GetAllocator());

        rapidjson::Value& splitData = builder.AddObject(builder.GetDocument(), "Split Data");
        for (std::pair<std::string, int> pair : preset.m_Splits)
        {
            rapidjson::Value key(rapidjson::StringRef(pair.first.c_str()), builder.GetAllocator());
            rapidjson::Value value(pair.second);
            splitData.AddMember(key, value, builder.GetAllocator());
        }

        if (!preset.m_Name.empty()) {
            builder.SaveToFile(filename);
        }
        if (!writeSplits) {
            Walnut::JSONBuilder::WriteEmpty(std::string_view(bo3Directory + "\\Practice Tool\\Settings\\Active Autosplit Preset.json"));
        }
        else {
            builder.SaveToFile(bo3Directory + "\\Practice Tool\\Settings\\Active Autosplit Preset.json");
        }
            
    }

    void CreateNewAutosplitPreset(std::string_view presetName)
    {
        std::string filename = selfDirectory + "\\Settings\\Autosplits\\" + presetName.data() + ".json";
       
        Walnut::JSONBuilder builder;

        rapidjson::Value& settings = builder.AddObject(builder.GetDocument(), "Settings");
        settings.AddMember("In Game Timer", false, builder.GetAllocator());
        settings.AddMember("In Game Round Timer", false, builder.GetAllocator());
        settings.AddMember("Amount of Splits", 0, builder.GetAllocator());
        settings.AddMember("Map Index", 0, builder.GetAllocator());
        settings.AddMember("Split Type", 0, builder.GetAllocator());

        rapidjson::Value& splitData = builder.AddObject(builder.GetDocument(), "Split Data");

        builder.SaveToFile(filename);
        LoadAutosplitPresets();
    }

    void DeleteAutosplitPreset(std::string_view preset)
    {
        if (std::filesystem::exists(selfDirectory + "\\Settings\\Autosplits\\" + preset.data() + ".json")) {
            std::filesystem::remove(selfDirectory + "\\Settings\\Autosplits\\" + preset.data() + ".json");
        }
        if (currentAutosplitPreset > 0) {
            currentAutosplitPreset--;
        }
        LoadAutosplitPresets();
    }

    void WriteSplitXML(std::string_view preset, const std::vector<std::pair<std::string, int>>& splits)
    {
        if (!DoesPathExist(selfDirectory + "/Exported Splits"))
            std::filesystem::create_directory(selfDirectory + "/Exported Splits");

        Walnut::XMLBuilder builder("1.0", "Run");

        rapidxml::xml_node<>* runNode = builder.GetRootNode();
        builder.AddAttribute(runNode, "version", "1.7.0");

        rapidxml::xml_node<>* gameIconNode = builder.AddNode(builder.GetRootNode(), "GameIcon");

        builder.AddNode(builder.GetRootNode(), "GameName", "Black Ops 3");
        builder.AddNode(builder.GetRootNode(), "CategoryName", preset.data());
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

        for (const std::pair<std::string, int>& split : splits)
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

        rapidxml::xml_node<>* segmentNode = builder.AddNode(segmentsNode, "Segment");

        builder.AddNode(segmentNode, "Name", "End");
        builder.AddNode(segmentNode, "Icon");

        rapidxml::xml_node<>* splitTimesNode = builder.AddNode(segmentNode, "SplitTimes");
        rapidxml::xml_node<>* splitTimeNode = builder.AddNode(splitTimesNode, "SplitTime");
        builder.AddAttribute(splitTimeNode, "name", "Personal Best");
        builder.AddNode(segmentNode, "BestSegmentTime");
        builder.AddNode(segmentNode, "SegmentHistory");

        builder.AddNode(builder.GetRootNode(), "AutoSplitterSettings");

        builder.SaveToFile(selfDirectory + "/Exported Splits/" + preset.data() + ".lss");
    }

    void WriteLayoutXML(std::string_view preset, int numSplits)
    {
        if (!DoesPathExist(selfDirectory + "/Exported Splits"))
            std::filesystem::create_directory(selfDirectory + "/Exported Splits");

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
        builder.AddNode(splitsComponent, "Path", "LiveSplit.m_Splits.dll");
        rapidxml::xml_node<>* splitsSettings = builder.AddNode(splitsComponent, "Settings");
        builder.AddNode(splitsSettings, "Version", "1.6");
        builder.AddNode(splitsSettings, "CurrentSplitTopColor", "FF3373F4");
        builder.AddNode(splitsSettings, "CurrentSplitBottomColor", "FF153574");
        builder.AddNode(splitsSettings, "VisualSplitCount", std::to_string(min(numSplits, 6)));
        builder.AddNode(splitsSettings, "SplitPreviewCount", "0");
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

        builder.SaveToFile(selfDirectory + "/Exported Splits/" + preset.data() + ".lsl");
    }

    AutosplitPreset ParseSplitJson(std::string_view filename)
    {
        AutosplitPreset returnPreset;
        Walnut::JSONBuilder builder(filename);

        if (builder.GetDocument().HasMember("Settings") && builder.GetDocument()["Settings"].IsObject()) {
            const rapidjson::Value& settings = builder.GetDocument()["Settings"];

            for (rapidjson::Value::ConstMemberIterator it = settings.MemberBegin(); it != settings.MemberEnd(); it++) {
                const std::string& key = it->name.GetString();
                const rapidjson::Value& value = it->value;

                switch (hashstr(key.c_str())) {
                case hashstr("In Game Timer"):
                    if (value.IsBool())
                        returnPreset.m_IGT = value.GetBool();
                    break;
                case hashstr("In Game Round Timer"):
                    if (value.IsBool())
                        returnPreset.m_IGRT = value.GetBool();
                    break;
                case hashstr("Amount of Splits"):
                    if (value.IsInt())
                        returnPreset.m_NumSplits = value.GetInt();
                    break;
                case hashstr("Map Index"):
                    if (value.IsInt())
                        returnPreset.m_Map = value.GetInt();
                    break;
                case hashstr("Split Type"):
                    if (value.IsInt())
                        returnPreset.m_SplitType = value.GetInt();
                    break;
                default:
                    break;
                }
            }
        }
        if (builder.GetDocument().HasMember("Split Data") && builder.GetDocument()["Split Data"].IsObject()) {
            const rapidjson::Value& splitData = builder.GetDocument()["Split Data"];

            for (rapidjson::Value::ConstMemberIterator it = splitData.MemberBegin(); it != splitData.MemberEnd(); it++) {
                if (it->name.IsString() && it->value.IsInt()) {
                    returnPreset.m_Splits.push_back({ it->name.GetString(), it->value.GetInt() });
                }
                else {
                    Walnut::Logger::Log(Walnut::MessageType::Error, "Autosplits json error: Incorrect typings found at key " + std::string(it->name.GetString()));
                }
            }
        }

        return returnPreset;
    }
#pragma endregion
}