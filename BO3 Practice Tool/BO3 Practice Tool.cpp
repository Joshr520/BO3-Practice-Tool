#include <Windows.h>
#include <Psapi.h>
#include <Shobjidl.h>
#include <filesystem>

#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include "Walnut/FileFormats/json.h"
#include "Walnut/Logger.h"

#include "ToolData.h"
#include "GlobalData.h"
#include "GUIState.h"
#include "Memhelp.h"
#include "Keybinds.h"
#include "ImGuiHelper.h"
#include "ListDefs.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <miniz/miniz.h>

using namespace BO3PT;
using WLog = Walnut::Logger;
using WMT = Walnut::MessageType;
using WJson = Walnut::JSONBuilder;
using MSSelection = ImGuiHelper::MultiSpanSelection;
using TextFont = ImGuiHelper::TextFont;
using Popup = ImGuiHelper::PopupWrapper;
using PopupStates = ImGuiHelper::PopupStates_;
using Button = ImGuiHelper::ButtonWrapper;
using Selection = ImGuiHelper::Selection;
using ListBox = ImGuiHelper::ListBoxWrapper;
using HelpMarker = ImGuiHelper::HelpMarker;

void SetupData();
void SearchForGame();
bool CheckUpdate();
bool PerformUpdate();
std::string SelectFolder();

void Sidebar();
void BGBLoadoutFunc();
void WeaponLoadoutFunc();
void AutosplitsFunc();
void PracticePatchesFunc();
void SettingsFunc();
void PlayerOptionsFunc();
void ZombieOptionsFunc();
void RoundOptionsFunc();
void PowerupOptionsFunc();
void EggStepOptionsFunc();
void CraftableOptionsFunc();
void BlockerOptionsFunc();
void MapOptionsFunc();
void GumTrackerFunc();
void ZombieCalculatorFunc();
void CodeGuidesFunc();
void GKValveSolverFunc();

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ImGUI data
static ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
static ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking;
static ImGuiWindowFlags dockFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;
static ImGuiViewport* viewport;

static MSSelection sidebarSelections({ {{"Gobblegum Loadouts", "Weapon Loadouts", "Autosplits", "Practice Patches", "Settings"}}, {{"Player Options", "Zombie Options", "Round Options", "Powerup Options", "Egg Step Options", "Craftable Options", "Blocker Options", "Map Options"}},
        {{"Gum Tracker", "Zombie Calculator", "Code Guides", "GK Valve Solver"}} });
static MSSelection weaponSelections;
static std::vector<MSSelection> autosplitSelections;

static std::string internalVersion = "0.5.0";
static Walnut::Image* discordIcon;
static Walnut::Image* weaponWarning;

// Gum data
static int chooseGumPreset = 0;
static std::vector<BGB> bgbDisplayList = { };
static int gumSelectMenu = 0;

// Player options data
static std::string weaponSelectName = "none";
static int perkSelectIndex = 0;
static int bgbClassicSelectIndex = 0;
static int bgbMegaSelectIndex = 0;

// Round options data
static int roundInput = 1;
static int zombieCount = 0;

// Autosplits data
static bool addSplitView = false;
static int currentAddSplitIndex = 0;
static int addSplitRound = -1;
static int mapError = 0;

extern Walnut::Application* Walnut::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	while (g_ApplicationRunning) {
		char buf[256];
		GetCurrentDirectoryA(256, buf);
		selfDirectory = buf;

        WLog::InitLogger(selfDirectory + "/log.txt");
		WLog::Log(WMT::Success, "Practice Tool Startup");

		Walnut::Application* app = Walnut::CreateApplication(__argc, __argv);
		app->Run();
		delete app;
	}

    WLog::Log(WMT::Success, "Practice Tool Finish");
    WLog::Log(WMT::Info, "Resetting presets");
    WriteBGBPresetToGame({});
    WriteAutosplitPresetToGame({});
    WritePracticePatches();

    return 0;
}

class RenderLayor : public Walnut::Layer
{
public:
	virtual void OnAttach() override
	{
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(1.25f);
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.WindowBorderSize = 0.0f;
        style.Colors[2].w = 0.6f;

        SetupData();

        discordIcon = new Walnut::Image("Resource Images\\Discord.png");
        weaponWarning = new Walnut::Image("Resource Images\\Weapons\\Warning.png");
	}

    virtual void OnUIRender() override
    {
        // Setup main window
        viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::PushFont(titleFont);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("BO3 Practice Tool", NULL, flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        // Setup dockspace
        ImGuiID dockID = ImGui::GetID("Dockspace");
        if (!GUIState::IsStateSet(Docked)) {
            GUIState::SetState(Docked);

            ImGui::DockBuilderRemoveNode(dockID);
            ImGui::DockBuilderAddNode(dockID);

            // Make the dock node's size and position to match the viewport
            ImGui::DockBuilderSetNodeSize(dockID, ImGui::GetMainViewport()->WorkSize);
            ImGui::DockBuilderSetNodePos(dockID, ImGui::GetMainViewport()->WorkPos);

            ImGuiID dock_main_id = dockID;
            ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
            ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
            ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.35f, nullptr, &dock_right_id);

            ImGui::DockBuilderDockWindow("##Sidebar", dock_left_id);
            ImGui::DockBuilderDockWindow("##Body", dock_right_id);
            ImGui::DockBuilderDockWindow("##Gum Context Menu", dock_down_id);

            ImGui::DockBuilderFinish(dock_main_id);
        }
        ImGui::DockSpace(dockID, { NULL, NULL }, ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::PopFont();
        ImGui::Begin("##Sidebar", 0, dockFlags);
        Sidebar();
        ImGui::End();

        ImGui::Begin("##Body", 0, dockFlags);

        switch (Popup::GetPrepState()) {
        case PopupStates::Update: {
            ImGui::OpenPopup("Update Available");
            Popup::OpenPopup(PopupStates::Update);
            break;
        }
        case PopupStates::UpdateFailed: {
            ImGui::OpenPopup("Update Failed");
            Popup::OpenPopup(PopupStates::UpdateFailed);
            break;
        }
        case PopupStates::InjectFailed: {
            ImGui::OpenPopup("Injection Failed");
            Popup::OpenPopup(PopupStates::InjectFailed);
            break;
        }
        case PopupStates::JoinDiscord: {
            ImGui::OpenPopup("Join Discord");
            Popup::OpenPopup(PopupStates::JoinDiscord);
            break;
        }
        case PopupStates::DirectoryError: {
            ImGui::OpenPopup("Directory Error");
            Popup::OpenPopup(PopupStates::DirectoryError);
            break;
        }
        case PopupStates::GumPresetCreation: {
            ImGui::OpenPopup("New Gum Preset");
            Popup::OpenPopup(PopupStates::GumPresetCreation);
            break;
        }
        case PopupStates::GumPresetError: {
            ImGui::OpenPopup("Gum Preset Already Exists");
            Popup::OpenPopup(PopupStates::GumPresetError);
            break;
        }
        case PopupStates::WeaponLoadoutCreation: {
            ImGui::OpenPopup("New Weapon Loadout");
            Popup::OpenPopup(PopupStates::WeaponLoadoutCreation);
            break;
        }
        case PopupStates::WeaponLoadoutError: {
            ImGui::OpenPopup("Weapon Loadout Already Exists");
            Popup::OpenPopup(PopupStates::WeaponLoadoutError);
            break;
        }
        case PopupStates::AutosplitPresetCreation: {
            ImGui::OpenPopup("New Autosplits Preset");
            Popup::OpenPopup(PopupStates::AutosplitPresetCreation);
            break;
        }
        case PopupStates::AutosplitPresetError: {
            ImGui::OpenPopup("Autosplit Preset Already Exists");
            Popup::OpenPopup(PopupStates::AutosplitPresetError);
            break;
        }
        case PopupStates::AutosplitMapError: {
            ImGui::OpenPopup("Change Map Error");
            Popup::OpenPopup(PopupStates::AutosplitMapError);
            break;
        }
        default:
            break;
        }
        ImVec2 modalSize = ImVec2(ImGui::GetContentRegionMax().x / 3, ImGui::GetContentRegionMax().y / 2);
        ImVec2 modalPos = ImVec2(viewport->Pos.x + viewport->Size.x / 2 - modalSize.x / 2, viewport->Pos.y + viewport->Size.y / 4);
        switch (Popup::GetOpenState()) {
        case PopupStates::Update: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Update Available", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("A new update is available, would you like to update?");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x / 2.0f - 5;
                if (ImGui::Button("OK", ImVec2(width, 25))) {
                    if (!PerformUpdate()) {
                        Popup::PrepPopup(PopupStates::UpdateFailed);
                    }
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Exit", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::UpdateFailed: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Update Failed", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("There was an error updating the program, check log.txt for more info");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x;
                if (ImGui::Button("OK", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::InjectFailed: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Injection Failed", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("1 or more required files is missing, would you like to redownload them? (Add an exclusion in your antivirus to stop this from happening)");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x / 2.0f - 5;
                if (ImGui::Button("Download Files", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                    DownloadAndExtractZip({ "GSC" });
                    VerifyExternalFiles();
                }
                ImGui::SameLine();
                if (ImGui::Button("Exit", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::JoinDiscord: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Join Discord", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Need help or looking for Practice Tool news? Join the Discord and head to #help or #bo3-practice-tool to stay up to date.");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x / 2.0f - 5;
                if (ImGui::Button("Join", ImVec2(width, 25))) {
                    std::thread([]() {
                        ShellExecuteA(NULL, "open", "https://discord.gg/SnaSjkPEmV", NULL, NULL, SW_SHOWNORMAL);
                    }).detach();
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Close", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::DirectoryError: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Directory Error", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Incorrect Folder Chosen");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x;
                if (ImGui::Button("OK", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::GumPresetCreation: {
            if (ImGui::BeginPopup("New Gum Preset")) {
                ImGui::SetKeyboardFocusHere();
                char presetInput[32] = "";
                ImGui::SetNextItemWidth(200);
                if (ImGui::InputText("New Preset Name", presetInput, IM_ARRAYSIZE(presetInput), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (std::string(presetInput).empty() || BGBPresetExists(presetInput)) {
                        Popup::PrepPopup(PopupStates::GumPresetError);
                    }
                    else {
                        CreateBGBPreset(presetInput);
                    }
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::GumPresetError: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Gum Preset Already Exists", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Choose a name that doesn't already exist.");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x;
                if (ImGui::Button("Close", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::WeaponLoadoutCreation: {
            if (ImGui::BeginPopup("New Weapon Loadout")) {
                ImGui::SetKeyboardFocusHere();
                char presetInput[32] = "";
                ImGui::SetNextItemWidth(200);
                if (ImGui::InputText("New Preset Name", presetInput, IM_ARRAYSIZE(presetInput), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (std::string(presetInput).empty() || DoesPathExist(selfDirectory + "\\Settings\\Weapon Loadouts\\" + presetInput + ".json")) {
                        Popup::PrepPopup(PopupStates::WeaponLoadoutError);
                    }
                    else {
                        CreateWeaponPreset(presetInput);
                    }
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::WeaponLoadoutError: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Weapon Loadout Already Exists", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Choose a name that doesn't already exist.");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x;
                if (ImGui::Button("Close", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::AutosplitPresetCreation: {
            if (ImGui::BeginPopup("New Autosplits Preset")) {
                ImGui::SetKeyboardFocusHere();
                char presetInput[32] = "";
                ImGui::SetNextItemWidth(200);
                if (ImGui::InputText("New Preset Name", presetInput, IM_ARRAYSIZE(presetInput), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (std::string(presetInput).empty() || DoesPathExist(selfDirectory + "\\Settings\\Autosplits\\" + presetInput + ".json")) {
                        Popup::PrepPopup(PopupStates::AutosplitPresetError);
                    }
                    else {
                        CreateAutosplitPreset(presetInput);
                    }
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::AutosplitPresetError: {
            ImGui::SetNextWindowSize(modalSize);
            ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Autosplit Preset Already Exists", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Choose a name that doesn't already exist.");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x;
                if (ImGui::Button("Close", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        case PopupStates::AutosplitMapError: {
            ImGui::SetNextWindowSize(modalSize);
            if (ImGui::BeginPopupModal("Change Map Error", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextWrapped("Changing the map will invalidate the current selected extra splits. Press continue to delete the current splits or press close and create a new layout instead.");
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 30.0f);
                float width = ImGui::GetContentRegionAvail().x / 2.0f - 5;
                if (ImGui::Button("Continue", ImVec2(width, 25))) {
                    autosplitPresets[currentAutosplitPreset].m_Map = mapError;
                    autosplitPresets[currentAutosplitPreset].m_Splits = {  };
                    autosplitPresets[currentAutosplitPreset].m_NumSplits = 0;
                    WriteAutosplitPresetToGame(autosplitPresets[currentAutosplitPreset]);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Close", ImVec2(width, 25))) {
                    ImGui::CloseCurrentPopup();
                    Popup::ClosePopup();
                }
                ImGui::EndPopup();
            }
            break;
        }
        default:
            break;
        }

        if (!GUIState::IsStateSet(SteamFound)) {
            ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Select BO3 Folder").x / 2, ImGui::GetContentRegionAvail().y / 2 - ImGui::CalcTextSize("Select BO3 Folder").y / 2 - 50));
            ImGui::Text("Select BO3 Folder");
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Select BO3 Folder").x / 2);
            if (Button::RenderSingle("Browse", ImVec2(ImGui::CalcTextSize("Select BO3 Folder").x, 50))) {
                bo3Directory = SelectFolder();
                std::string exe = bo3Directory + std::string("\\BlackOps3.exe");

                if (bo3Directory.empty() || !DoesPathExist(exe)) {
                    Popup::PrepPopup(PopupStates::DirectoryError);
                }
                else {
                    GUIState::SetState(SteamFound);
                    WJson builder = WJson::FromFile(selfDirectory + "\\settings.json");
                    builder.ModifyString(builder.GetDocument(), "Steam Path", bo3Directory);
                    builder.SaveToFile(selfDirectory + "\\settings.json");
                    VerifyExternalFiles();
                    WLog::Log(WMT::Info, "Writing default presets");
                    WriteBGBPresetToGame({});
                    WriteAutosplitPresetToGame({});
                    WritePracticePatches();
                }
            }
        }
        else if (GUIState::IsStateSet(SetupDone)) {
            GUIState::Render();
        }

        ImGui::End();
        ImGui::End();

        WLog::DrawLogWindow();

        if (MemState::GetState() == Loaded) {
            int prevRound = MemState::GetRoundValue();
            std::string prevMap = MemState::GetMapNameValue();

            if (!MemState::ReadData()) {
                ResetToggles();
                writeBGBs = false;
                writeSplits = false;
                WriteBGBPresetToGame({});
                WriteAutosplitPresetToGame({});
                GUIState::UnsetState(Active);
                auto thread = std::thread(SearchForGame);
                thread.detach();
            }
            
            currentMap = MemState::GetMapNameValue();

            if ((currentMap.substr(0, 2) != "zm" && prevMap != "core_frontend") || (!MemState::GetRoundValue() && prevRound)) {
                ResetToggles();
            }

            if (currentMap != prevMap && currentMap.substr(0, 2) == "zm") {
                weaponSelections.Reset();

                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_AR);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_SMG);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_LMG);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Shotgun);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Sniper);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Pistol);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Launcher);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Melee);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Special);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Equipment);
                weaponSelections.AddSelection(weapons.m_Weapons[currentMap].m_Hero);
            }
        }
    }
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "BO3 Practice Tool";
    spec.Width = 1280;
    spec.Height = 720;

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<RenderLayor>();
	return app;
}

void SetupData()
{
    if (!DoesPathExist(selfDirectory + "\\settings.json")) {
        WJson::WriteEmpty(selfDirectory + "\\settings.json");
    }
    WJson builder = WJson::FromFile(selfDirectory + "\\settings.json");

    WLog::Log(WMT::Info, "Searching for BO3 directory");
    if (builder.GetDocument().HasMember("Steam Path") && builder.GetDocument()["Steam Path"].IsString()) {
        bo3Directory = builder.GetDocument()["Steam Path"].GetString();
        WLog::Log(WMT::Success, "BO3 directory found");
    }
    else {
        WLog::Log(WMT::Info, "BO3 directory not saved - prompting user for input");
    }
    if (DoesPathExist(bo3Directory)) {
        GUIState::SetState(SteamFound);
        VerifyExternalFiles();
    }
    if (MemState::SetPID(L"BlackOps3.exe") && MemState::SetAddresses(L"BlackOps3.exe")) {
        std::stringstream log;
        log << std::string(40, '-') + "Base Address: " << MemState::GetBaseAddress() << "\n";
        log << std::string(40, '-') + "Map Address: " << MemState::GetMapNameAddress() << "\n";
        log << std::string(40, '-') + "Round Address: " << MemState::GetRoundAddress();
        WLog::Log(WMT::Success, "Loading Process Addresses:\n" + log.str());
        if (!MemState::SetHandle()) {
            WLog::Log(WMT::Error, "Error opening process with error code " + GetLastError());
        }
    }
    else {
        std::thread(SearchForGame).detach();
    }

    if (CheckUpdate()) {
        Popup::PrepPopup(PopupStates::Update);
    }

    VerifyInternalFiles();
    InitVariables();
    LoadImages(sidebarSelections.GetIndex());

    if (GUIState::IsStateSet(SteamFound)) {
        WLog::Log(WMT::Info, "Writing default presets");
        WriteBGBPresetToGame({});
        WritePracticePatches();
        WriteAutosplitPresetToGame({});
    }

    GUIState::AddMember({ std::function<void()>(BGBLoadoutFunc) });
    GUIState::AddMember({ std::function<void()>(WeaponLoadoutFunc) });
    GUIState::AddMember({ std::function<void()>(AutosplitsFunc) });
    GUIState::AddMember({ std::function<void()>(PracticePatchesFunc) });
    GUIState::AddMember({ std::function<void()>(SettingsFunc) });
    GUIState::AddMember({ std::function<void()>(PlayerOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(ZombieOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(RoundOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(PowerupOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(EggStepOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(CraftableOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(BlockerOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(MapOptionsFunc) });
    GUIState::AddMember({ std::function<void()>(GumTrackerFunc) });
    GUIState::AddMember({ std::function<void()>(ZombieCalculatorFunc) });
    GUIState::AddMember({ std::function<void()>(CodeGuidesFunc) });
    GUIState::AddMember({ std::function<void()>(GKValveSolverFunc) });


    for (int i = 0; i < static_cast<int>(t7Maps.size()); i++) {
        autosplitSelections.emplace_back(MSSelection());
        for (const AutosplitGroup& splitGroup : autosplits.m_AutosplitLists[t7Maps[i]]) {
            autosplitSelections[i].AddSelection(splitGroup.m_Splits);
        }
    }

    WLog::Log(WMT::Success, "Setup Finished");
    GUIState::SetState(SetupDone);
}

void SearchForGame()
{
    HWND codHWND;
    DWORD tempID = 0;
    WLog::Log(WMT::Info, "Started async process - looking for BO3");
    for (;;) {
        codHWND = FindWindowA("CoDBlackOps", NULL);
        if (codHWND != NULL) {
            char windowFilename[256] = "";
            GetWindowThreadProcessId(codHWND, &tempID);
            HANDLE tempHandle = OpenProcess(PROCESS_ALL_ACCESS, false, tempID);
            GetModuleFileNameExA(tempHandle, NULL, windowFilename, 256);
            std::string name = windowFilename;
            if (name.find("BlackOps3.exe") != name.npos) {
                if (MemState::SetPID(L"BlackOps3.exe") && MemState::SetAddresses(L"BlackOps3.exe")) {
                    std::stringstream log;
                    log << std::string(20, ' ') + "Base Address: " << MemState::GetBaseAddress() << "\n";
                    log << std::string(20, ' ') + "Map Address: " << MemState::GetMapNameAddress() << "\n";
                    log << "Round Address: " << MemState::GetRoundAddress();
                    WLog::Log(WMT::Success, "Loading Process Addresses:\n" + log.str());
                    if (!MemState::SetHandle()) {
                        WLog::Log(WMT::Error, "Error opening process with error code " + GetLastError());
                    }
                    else {
                        WLog::Log(WMT::Success, "Ending async process - BO3 found");
                        return;
                    }
                }
            }
            CloseHandle(tempHandle);
        }
        Sleep(1000);
    }
}

bool CheckUpdate()
{
    if (DoesPathExist(selfDirectory + "/BO3 Practice Tool.old.exe")) {
        if (std::filesystem::remove(selfDirectory + "/BO3 Practice Tool.old.exe")) {
            WLog::Log(WMT::Info, "Deleting old exe");
        }
        else {
            WLog::Log(WMT::Error, "Couldn't remove old exe with error code: " + std::error_code(errno, std::system_category()).message());
        }
    }
    WLog::Log(WMT::Info, "Checking for updates");

    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string buffer;
    std::string tagName;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/joshr520/BO3-Practice-Tool/releases/latest");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "BO3-Practice-Tool");
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            WLog::Log(WMT::Error, "curl GET failed with error code: " + std::to_string(res));
            return false;
        }
        WJson builder = WJson::FromString(buffer);
        if (builder.GetDocument().HasParseError()) {
            WLog::Log(WMT::Error, "JSON returned from get invalid");
            return false;
        }
        const char* downloadGet = "";
        const char* tagGet = "";
        if (!builder.RetrieveValueFromKey("browser_download_url", downloadGet)) {
            return false;
        }
        if (!builder.RetrieveValueFromKey("tag_name", tagGet)) {
            return false;
        }

        downloadURL = downloadGet;
        tagName = tagGet;

        if (!CheckVersions(tagName, internalVersion)) {
            WLog::Log(WMT::Info, "New version not available");
            return false;
        }
        WLog::Log(WMT::Info, "New version detected");
    }

    return true;
}

bool PerformUpdate()
{
    std::string ptexe;

    if (DownloadAndExtractZip({ "BO3 Practice Tool", "GSC", "Resource Images" })) {
        return false;
    }
    return true;
}

std::string SelectFolder()
{
    std::string returnFolder = "";

    HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);

    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    IFileDialog* pFileDialog;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (SUCCEEDED(hr)) {
        FILEOPENDIALOGOPTIONS options;
        hr = pFileDialog->GetOptions(&options);
        if (SUCCEEDED(hr)) {
            options |= FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST;
            hr = pFileDialog->SetOptions(options);
        }

        if (SUCCEEDED(hr)) {
            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr)) {
                IShellItem* pItem;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR folderPath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &folderPath);
                    if (SUCCEEDED(hr)) {
                        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, folderPath, -1, nullptr, 0, nullptr, nullptr);

                        returnFolder.resize(bufferSize);
                        WideCharToMultiByte(CP_UTF8, 0, folderPath, -1, &returnFolder[0], bufferSize, nullptr, nullptr);
                        returnFolder.resize(returnFolder.length() - 1);

                        CoTaskMemFree(folderPath);
                    }
                    pItem->Release();
                }
            }
        }

        pFileDialog->Release();
    }

    CoUninitialize();

    return returnFolder;
}

void Sidebar()
{
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
    if (ImGui::ImageButton("Discord Icon", discordIcon->GetDescriptorSet(), ImVec2(45, 45))) {
        Popup::PrepPopup(PopupStates::JoinDiscord);
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();

    if (!GUIState::IsStateSet(SteamFound) || MemState::GetState() == Unloaded) {
        ImGui::BeginDisabled();
    }
    if (Button::RenderToggle("Toggle Status", ImVec2(ImGui::GetContentRegionAvail().x, 50), GUIState::IsStateSet(Active))) {
        if (currentMap == "") {
            GUIState::UnsetState(Active);
            WLog::Log(WMT::Error, "Game not loaded enough to toggle on");
        }
        else {
            if (GUIState::IsStateSet(Active)) {
                WriteBGBPresetToGame({});
                WriteAutosplitPresetToGame({});
                WritePracticePatches();
                ResetToggles();
                WLog::Log(WMT::Info, "Toggling tool off");
                GUIState::UnsetState(Active);
            }
            else {
                WriteBGBPresetToGame(bgbPresets[currentBGBPreset]);
                WriteAutosplitPresetToGame(autosplitPresets[currentAutosplitPreset]);
                WritePracticePatches();
                WLog::Log(WMT::Info, "Toggling tool on");
                GUIState::SetState(Active);

            }
            std::thread(InjectTool, GUIState::IsStateSet(Active)).detach();
        }
    }
    ImGui::Separator();
    if (!GUIState::IsStateSet(SteamFound) || MemState::GetState() == Unloaded) {
        ImGui::EndDisabled();
    }

    TextFont::Render("Frontend", sidebarFont);
    ImGui::Separator(2.5f);
    // Frontend
    sidebarSelections.Render(0);
    ImGui::Spacing();
    ImGui::Separator();
    if (!GUIState::IsStateSet(SteamFound) || MemState::GetState() == Unloaded) {
        ImGui::BeginDisabled();
    }
    TextFont::Render("In Game", sidebarFont);
    ImGui::Separator(2.5f);
    // In Game
    if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm") {
        if (sidebarSelections.GetArrIndex() == 1) {
            sidebarSelections.SetIndex(0);
        }
        ImGui::BeginDisabled();
    }
    sidebarSelections.Render(1);
    ImGui::Spacing();
    ImGui::Separator();
    if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm") {
        ImGui::EndDisabled();
    }
    if (!GUIState::IsStateSet(SteamFound) || MemState::GetState() == Unloaded) {
        ImGui::EndDisabled();
    }
    TextFont::Render("Resources", sidebarFont);
    ImGui::Separator(2.5f);
    // Resources
    sidebarSelections.Render(2);
    ImGui::Spacing();
    ImGui::Separator();

    if (sidebarSelections.GetChanged()) {
        LoadImages(sidebarSelections.GetIndex());
    }

    GUIState::SetIndex(sidebarSelections.GetIndex());
}

void BGBLoadoutFunc()
{
    if (!showBGBSelection) {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
        ImGui::BeginGroup();
        if (Button::RenderSingle(ICON_FA_FILE_CIRCLE_PLUS " New Preset", ImVec2(150, 25))) {
            Popup::PrepPopup(PopupStates::GumPresetCreation);
        }
        if (bgbPresets.size()) {
            if (Button::RenderSingle(ICON_FA_FILE_CIRCLE_MINUS " Delete Preset", ImVec2(150, 25))) {
                DeleteBGBPreset(bgbPresets[currentBGBPreset]);
            }
        }
        ImGui::PopStyleVar();
        if (bgbPresets.size()) {
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            // presets dropdown
            ImGui::SetNextItemWidth(250);
            if (ImGui::BeginCombo("Gum Presets", bgbPresets[currentBGBPreset].m_Name.c_str(), ImGuiComboFlags_HeightRegular)) {
                if (Selection::RenderBGBPreset(bgbPresets, currentBGBPreset) && writeBGBs && GUIState::IsStateSet(Active)) {
                    WriteBGBPresetToGame(bgbPresets[currentBGBPreset]);
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (!GUIState::IsStateSet(Active)) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Checkbox("Active", &writeBGBs)) {
                WriteBGBPresetToGame(bgbPresets[currentBGBPreset]);
            }
            if (!GUIState::IsStateSet(Active)) {
                ImGui::EndDisabled();
            }
            // preset gum image buttons
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
            ImGui::BeginGroup();
            for (const BGB& bgb : bgbPresets[currentBGBPreset].m_BGBs) {
                if (ImGui::ImageButton(bgb.m_Name.c_str(), bgbImgList[bgb.m_Name]->GetDescriptorSet(), ImVec2(128, 128))) {
                    bgbContext = bgb;
                    bgbToSwap = bgb;
                    showBGBSelection = true;
                    break;
                }
                if (ImGui::IsItemHovered()) {
                    bgbContext = bgb;
                }
                ImGui::SameLine();
            }
            ImGui::EndGroup();
            ImGui::PopStyleColor(2);
        }
        ImGui::EndGroup();
    }
    else {
        // swap gum selection menu
        if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
            showBGBSelection = false;
        }
        ImGui::SameLine();
        if (ImGui::BeginTabBar("Gum Type Choice")) {
            if (ImGui::BeginTabItem("Classics")) {
                static char searchText[64] = "";
                if (gumSelectMenu) {
                    strcpy_s(searchText, "");
                }
                gumSelectMenu = 0;
                if (!strcmp(searchText, "")) {
                    bgbDisplayList = bgbs.m_Classics;
                }
                if (ImGui::InputText("Gobblegum Search", searchText, IM_ARRAYSIZE(searchText))) {
                    bgbDisplayList = BGBSearch(0, searchText);
                }
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
                ImGui::BeginGroup();
                int numBGB = 0;
                for (const BGB& bgb : bgbDisplayList) {
                    if (ImGui::ImageButton(bgb.m_Name.c_str(), bgbImgList[bgb.m_Name]->GetDescriptorSet(), ImVec2(145, 145))) {
                        bgbContext = bgb;
                        strcpy_s(searchText, "");
                        showBGBSelection = false;
                        SwapBGBPreset(bgbToSwap, bgbContext);
                        break;
                    }
                    if (ImGui::IsItemHovered()) {
                        bgbContext = bgb;
                    }
                    numBGB++;
                    if (numBGB > 5) {
                        numBGB = 0;
                        ImGui::EndGroup();
                        ImGui::BeginGroup();
                    }
                    else {
                        ImGui::SameLine();
                    }
                }
                ImGui::EndGroup();
                ImGui::PopStyleColor(2);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Megas")) {
                static char searchText[64] = "";
                if (!gumSelectMenu) {
                    strcpy_s(searchText, "");
                }
                gumSelectMenu = 1;
                if (!strcmp(searchText, "")) {
                    bgbDisplayList = bgbs.m_Megas;
                }
                if (ImGui::InputText("Gobblegum Search", searchText, IM_ARRAYSIZE(searchText))) {
                    bgbDisplayList = BGBSearch(1, searchText);
                }
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
                ImGui::BeginGroup();
                int numBGB = 0;
                for (const BGB& bgb : bgbDisplayList) {
                    if (ImGui::ImageButton(bgb.m_Name.c_str(), bgbImgList[bgb.m_Name]->GetDescriptorSet(), ImVec2(145, 145))) {
                        bgbContext = bgb;
                        strcpy_s(searchText, "");
                        showBGBSelection = false;
                        SwapBGBPreset(bgbToSwap, bgbContext);
                        break;
                    }
                    if (ImGui::IsItemHovered()) {
                        bgbContext = bgb;
                    }
                    numBGB++;
                    if (numBGB > 5) {
                        numBGB = 0;
                        ImGui::EndGroup();
                        ImGui::BeginGroup();
                    }
                    else {
                        ImGui::SameLine();
                    }
                }
                ImGui::EndGroup();
                ImGui::PopStyleColor(2);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    if (bgbPresets.size()) {
        ImGui::Begin("##Gum Context Menu", 0, dockFlags);
        ImGui::Image(BO3PT::bgbImgList[BO3PT::bgbContext.m_Name]->GetDescriptorSet(), ImVec2(196, 196));
        ImGui::SameLine();
        ImVec2 contPos = ImGui::GetCursorPos();
        TextFont::Render(bgbImgList[bgbContext.m_Name]->GetFilename().c_str(), sidebarFont, 1, 0);
        ImGui::SetCursorPos(ImVec2(contPos.x, contPos.y + 30));
        TextFont::RenderWrapped(bgbContext.m_Description.c_str(), sidebarFont, 0, 1);
        ImGui::End();
    }
}

void WeaponLoadoutFunc()
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGuiStyle style = ImGui::GetStyle();

    if (currentWeaponPresetMenu < 0) {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
        ImGui::BeginGroup();
        if (Button::RenderSingle(ICON_FA_FILE_CIRCLE_PLUS " New Preset", ImVec2(150, 25))) {
            Popup::PrepPopup(PopupStates::WeaponLoadoutCreation);
        }
        if (weaponPresets.size()) {
            if (Button::RenderSingle(ICON_FA_FILE_CIRCLE_MINUS " Delete Preset", ImVec2(150, 25))) {
                DeleteWeaponPreset(weaponPresets[currentWeaponPreset]);
            }
        }
        ImGui::PopStyleVar();
        if (weaponPresets.size()) {
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetNextItemWidth(250);
            if (ImGui::BeginCombo("Weapon Loadouts", weaponPresets[currentWeaponPreset].m_Name.c_str(), ImGuiComboFlags_HeightRegular)) {
                if (Selection::RenderWeaponLoadout(weaponPresets, currentWeaponPreset) && writeWeaponPresets && GUIState::IsStateSet(Active)) {
                    WriteWeaponLoadoutToGame(weaponPresets[currentWeaponPreset]);
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (!GUIState::IsStateSet(Active)) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Checkbox("Active", &writeWeaponPresets)) {
                WriteWeaponLoadoutToGame(weaponPresets[currentWeaponPreset]);
            }
            if (!GUIState::IsStateSet(Active)) {
                ImGui::EndDisabled();
            }

            ImGui::BeginChild("Scroll Weapon Type Selection", ImGui::GetContentRegionAvail());

            const float scroll = ImGui::GetScrollY();
            const int numOnLine = static_cast<int>(ImGui::GetContentRegionAvail().x / (weaponKitSize.x + style.ItemSpacing.x));
            const ImVec2 startPos = ImGui::GetCursorScreenPos() + ImVec2(ImGui::GetContentRegionAvail().x / 2 - (numOnLine * (weaponKitSize.x + style.ItemSpacing.x) / 2), 25);
            const ImVec2 startSize = ImGui::GetContentRegionAvail();
            const ImVec2 clipStart = ImVec2(startPos.x, startPos.y + scroll);
            const ImVec2 clipEnd = ImVec2(startPos.x + startSize.x, startPos.y + startSize.y + scroll);
            drawList->PushClipRect(clipStart, clipEnd + startSize, true);
            ImGui::SetCursorScreenPos(startPos);

            for (int i = 0; i < static_cast<int>(buildKitImgList.size()); i++) {
                const ImVec2 pos = ImGui::GetCursorScreenPos();
                const ImVec2 posEnd = pos + weaponKitSize;
                const ImVec2 textPos = pos + ImVec2(5, weaponKitSize.y - 25);
                const ImRect imageRect(pos, posEnd);
                const std::string& weaponType = menuWeaponLists.m_WeaponTypes[i];

                drawList->AddRectFilled(pos, posEnd, COLOR_GREY);
                drawList->AddImage(buildKitImgList[i]->GetDescriptorSet(), pos, posEnd);
                drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize(weaponType.c_str()), IM_COL32(0, 0, 0, 170));
                drawList->AddText(textPos, COLOR_WHITE, weaponType.c_str());
                drawList->AddRect(pos, posEnd, COLOR_WHITE);

                if (imageRect.Contains(ImGui::GetMousePos())) {
                    drawList->AddRect(pos, posEnd, COLOR_ORANGE);
                    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_MouseLeft, false)) {
                        currentWeaponPresetMenu = i;
                        currentWeaponEdit = 0;
                    }
                }

                if (!((i + 1) % numOnLine)) {
                    ImGui::SetCursorScreenPos(ImVec2(startPos.x, posEnd.y + style.ItemSpacing.y));
                }
                else {
                    ImGui::SetCursorScreenPos(ImVec2(posEnd.x + style.ItemSpacing.x, pos.y));
                }
                if (i == buildKitImgList.size() - 1) {
                    ImGui::SetCursorScreenPos(ImVec2(pos.x, posEnd.y + style.ItemSpacing.y));
                }
            }

            drawList->PopClipRect();
            ImGui::EndChild();
        }
        else {
            ImGui::EndGroup();
        }
    }
    else if (camoSelection) {
        if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
            camoSelection = false;
        }

        if (camoSelection) {
            const std::string& selectedWeaponType = menuWeaponLists.m_WeaponTypes[currentWeaponPresetMenu];
            MenuWeaponPresetItem& presetWeapon = weaponPresets[currentWeaponPreset].m_PresetItems[selectedWeaponType][currentWeaponEdit];

            if (ImGui::BeginTabBar("Camo Category")) {
                for (int i = 0; i < static_cast<int>(camosOrder.m_Types.size()); i++) {
                    const std::string& type = camosOrder.m_Types[i];

                    if (ImGui::BeginTabItem(type.c_str())) {
                        ImGui::BeginChild("Scroll Camos", ImGui::GetContentRegionAvail());
                        ImGui::Dummy(ImVec2(25, 0));
                        ImGui::SameLine();

                        const float scroll = ImGui::GetScrollY();
                        const int numOnLine = static_cast<int>(ImGui::GetContentRegionAvail().x / (camoSelectSize.x + style.ItemSpacing.x));
                        const ImVec2 startPos = ImGui::GetCursorScreenPos();
                        const ImVec2 startSize = ImGui::GetContentRegionAvail();
                        const ImVec2 clipStart = ImVec2(startPos.x, startPos.y + scroll);
                        const ImVec2 clipEnd = ImVec2(startPos.x + startSize.x, startPos.y + startSize.y + scroll);
                        drawList->PushClipRect(clipStart, clipEnd + startSize, true);
                        for (int j = 0; j < static_cast<int>(camosOrder.m_Camos[i].size()); j++) {
                            const ImVec2 pos = ImGui::GetCursorScreenPos();
                            const ImVec2 posEnd = pos + camoSelectSize;
                            const ImVec2 textPos = pos + ImVec2(5, camoSelectSize.y - 25);
                            const ImRect imageRect(pos, posEnd);
                            const std::string& camo = camosOrder.m_Camos[i][j];

                            drawList->AddImage(camosImgList[type].m_Camos[camo]->GetDescriptorSet(), pos, posEnd);
                            drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize(camo.c_str()), IM_COL32(0, 0, 0, 170));
                            drawList->AddText(textPos, COLOR_WHITE, camo.c_str());
                            drawList->AddRect(pos, posEnd, COLOR_WHITE);

                            if (imageRect.Contains(ImGui::GetMousePos())) {
                                drawList->AddRect(pos, posEnd, COLOR_ORANGE);
                                if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_MouseLeft, false)) {
                                    presetWeapon.m_Camo = { type, camo };
                                    camoSelection = false;
                                    SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
                                }
                            }

                            if (!((j + 1) % numOnLine)) {
                                ImGui::SetCursorScreenPos(ImVec2(startPos.x, posEnd.y + style.ItemSpacing.y));
                            }
                            else {
                                ImGui::SetCursorScreenPos(ImVec2(posEnd.x + style.ItemSpacing.x, pos.y));
                            }
                            if (j == camosOrder.m_Camos[i].size() - 1) {
                                ImGui::SetCursorScreenPos(ImVec2(pos.x, posEnd.y + style.ItemSpacing.y));
                            }
                        }
                        drawList->PopClipRect();
                        ImGui::EndChild();
                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
        }
    }
    else if (currentWeaponPresetMenu >= 0) {
        if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
            currentWeaponPresetMenu = -1;
        }

        if (currentWeaponPresetMenu >= 0)
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            const MenuWeapon& selectedWeapon = menuWeaponLists.m_Weapons[currentWeaponPresetMenu][currentWeaponEdit];
            const std::string& selectedWeaponType = menuWeaponLists.m_WeaponTypes[currentWeaponPresetMenu];
            MenuWeaponPresetItem& presetWeapon = weaponPresets[currentWeaponPreset].m_PresetItems[selectedWeaponType][currentWeaponEdit];
            std::vector<int>& equippedAttachments = presetWeapon.m_EquippedAttachments;

            ImGui::SameLine();
            if (Button::RenderSingle("Clear", ImVec2(75, 25))) {
                presetWeapon.m_EquippedOptic = -1;
                presetWeapon.m_EquippedAttachments.clear();
                presetWeapon.m_Camo = { "", "" };
                SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
            }
            // Button prompts
            const ImVec2 buttonPos = ImGui::GetWindowPos() + ImVec2(25, ImGui::GetContentRegionAvail().y - gunSize.y / 2);
            const ImVec2 buttonPosEnd = buttonPos + buttonPromptSize;
            const ImVec2 buttonTextPos = buttonPos + buttonPromptSize / 2 - ImGui::CalcTextSize("R") / 2;
            const ImVec2 actionTextPos = ImVec2(buttonPosEnd.x - buttonPromptSize.x / 2 - ImGui::CalcTextSize("Remove").x / 2, buttonPosEnd.y + style.ItemSpacing.y);

            drawList->AddRectFilled(buttonPos, buttonPosEnd, COLOR_GREY);
            drawList->AddRect(buttonPos, buttonPosEnd, COLOR_WHITE);
            drawList->AddText(buttonTextPos, COLOR_WHITE, "R");
            drawList->AddRectFilled(actionTextPos, actionTextPos + ImGui::CalcTextSize("Remove"), IM_COL32(0, 0, 0, 170));
            drawList->AddText(actionTextPos, COLOR_WHITE, "Remove");
            // Optics selections
            for (int i = 0; i < static_cast<int>(selectedWeapon.m_Optics.size()); i++) {
                const std::string& optic = selectedWeapon.m_Optics[i];
                const ImVec2 pos = ImGui::GetWindowPos() + ImVec2(50 + i * (attachmentSize.x + style.ItemSpacing.x), 50);
                const ImVec2 posEnd = pos + attachmentSize;
                const ImVec2 textPos = pos + ImVec2(5, attachmentSize.y - 25);
                const ImRect imageRect(pos, posEnd);

                if (i == presetWeapon.m_EquippedOptic) {
                    drawList->AddRectFilled(pos, posEnd, COLOR_BLUE);
                }
                drawList->AddImage(opticsImgList[optic]->GetDescriptorSet(), pos, posEnd);
                drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize(optic.c_str()), IM_COL32(0, 0, 0, 170));
                drawList->AddText(textPos, COLOR_WHITE, optic.c_str());
                drawList->AddRect(pos, posEnd, COLOR_WHITE);
                if (imageRect.Contains(ImGui::GetMousePos())) {
                    drawList->AddRect(pos, posEnd, COLOR_ORANGE);
                    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_MouseLeft, false) || ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R, false)) {
                        if (i == presetWeapon.m_EquippedOptic) {
                            presetWeapon.m_EquippedOptic = -1;
                            SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
                        }
                        else {
                            presetWeapon.m_EquippedOptic = i;
                            SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
                        }
                    }
                }
            }
            // Attachments selections
            for (int i = 0; i < static_cast<int>(selectedWeapon.m_Attachments.size()); i++) {
                const std::string& attachment = selectedWeapon.m_Attachments[i];
                const std::string& attachmentIndex = selectedWeapon.m_Name + "_" + attachment;
                const ImVec2 pos = ImGui::GetWindowPos() + ImVec2(50 + i * (attachmentSize.x + style.ItemSpacing.x), 50 + attachmentSize.y + style.ItemSpacing.y * 3);
                const ImVec2 posEnd = pos + attachmentSize;
                const ImVec2 textPos = pos + ImVec2(5, attachmentSize.y - 25);
                const ImRect imageRect(pos, posEnd);
                bool drawNum = false;

                if (std::find(equippedAttachments.begin(), equippedAttachments.end(), i) != equippedAttachments.end()) {
                    drawList->AddRectFilled(pos, posEnd, COLOR_BLUE);
                    drawNum = true;
                }
                drawList->AddImage(attachmentsImgList[attachmentIndex]->GetDescriptorSet(), pos, posEnd);
                drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize(attachment.c_str()), IM_COL32(0, 0, 0, 170));
                drawList->AddText(textPos, COLOR_WHITE, attachment.c_str());
                drawList->AddRect(pos, posEnd, COLOR_WHITE);
                if (drawNum) {
                    auto it = std::find(equippedAttachments.begin(), equippedAttachments.end(), i);
                    drawList->AddText(pos + ImVec2(5, 5), COLOR_WHITE, std::to_string(std::distance(equippedAttachments.begin(), it) + 1).c_str());
                }
                if (imageRect.Contains(ImGui::GetMousePos())) {
                    drawList->AddRect(pos, posEnd, COLOR_ORANGE);
                    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_MouseLeft, false) || ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R, false)) {
                        if (std::find(equippedAttachments.begin(), equippedAttachments.end(), i) != equippedAttachments.end()) {
                            equippedAttachments.erase(std::find(equippedAttachments.begin(), equippedAttachments.end(), i));
                            SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
                        }
                        else {
                            equippedAttachments.emplace_back(i);
                            if (equippedAttachments.size() > selectedWeapon.m_NumAttachments) {
                                equippedAttachments.erase(equippedAttachments.begin());
                            }
                            SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
                        }
                    }
                }
            }
            // Num attachments selected
            for (int i = 0; i < static_cast<int>(equippedAttachments.size()); i++) {
                const ImVec2 pos = ImGui::GetWindowPos() + ImVec2(50 + i * (20 + style.ItemSpacing.x), 50 + attachmentSize.y * 2 + style.ItemSpacing.y * 5);
                drawList->AddRectFilled(pos, pos + ImVec2(20, 20), COLOR_WHITE);
            }
            for (int i = static_cast<int>(equippedAttachments.size()); i < selectedWeapon.m_NumAttachments; i++) {
                const ImVec2 pos = ImGui::GetWindowPos() + ImVec2(50 + i * (20 + style.ItemSpacing.x), 50 + attachmentSize.y * 2 + style.ItemSpacing.y * 5);
                drawList->AddRect(pos, pos + ImVec2(20, 20), COLOR_WHITE);
            }
            // Camo Selection
            {
                const ImVec2 pos = ImGui::GetWindowPos() + ImVec2(50, 50 + attachmentSize.y * 2 + style.ItemSpacing.y * 10);
                const ImVec2 posEnd = pos + camoPreviewSize;
                const ImVec2 textPos = pos + ImVec2(5, camoPreviewSize.y - 25);
                const ImRect imageRect(pos, posEnd);
                if (!presetWeapon.m_Camo.first.empty() && !presetWeapon.m_Camo.second.empty()) {
                    drawList->AddImage(camosImgList[presetWeapon.m_Camo.first].m_Camos[presetWeapon.m_Camo.second]->GetDescriptorSet(), pos, posEnd);
                    drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize(presetWeapon.m_Camo.second.c_str()), IM_COL32(0, 0, 0, 170));
                    drawList->AddText(textPos, COLOR_WHITE, presetWeapon.m_Camo.second.c_str());
                }
                else {
                    drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize("Choose Camo"), IM_COL32(0, 0, 0, 170));
                    drawList->AddText(textPos, COLOR_WHITE, "Choose Camo");
                }
                drawList->AddRect(pos, posEnd, COLOR_WHITE);
                if (imageRect.Contains(ImGui::GetMousePos())) {
                    drawList->AddRect(pos, posEnd, COLOR_ORANGE);
                    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_MouseLeft, false)) {
                        camoSelection = true;
                    }
                    else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R, false)) {
                        presetWeapon.m_Camo = { "", "" };
                        SaveWeaponLoadout(weaponPresets[currentWeaponPreset]);
                    }
                }
            }
            // Gun selections
            for (int i = 0; i < static_cast<int>(menuWeaponLists.m_Weapons[currentWeaponPresetMenu].size()); i++) {
                const MenuWeapon& weapon = menuWeaponLists.m_Weapons[currentWeaponPresetMenu][i];
                const std::string& weaponType = menuWeaponLists.m_WeaponTypes[currentWeaponPresetMenu];
                const ImVec2 pos = ImVec2(ImGui::GetWindowPos().x + (buttonPosEnd.x - buttonPos.x) + 50 + i * (gunSize.x + style.ItemSpacing.x), ImGui::GetContentRegionMaxAbs().y - gunSize.y + style.ItemSpacing.y);
                const ImVec2 posEnd = pos + gunSize;
                const ImVec2 textPos = pos + ImVec2(5, gunSize.y - 25);
                const ImRect imageRect(pos, posEnd);

                if (i == currentWeaponEdit) {
                    drawList->AddRectFilled(pos, posEnd, COLOR_BLUE);
                }
                drawList->AddImage(weaponIconsImgList[weapon.m_Name]->GetDescriptorSet(), pos, posEnd);
                drawList->AddRectFilled(textPos, textPos + ImGui::CalcTextSize(weapon.m_Name.c_str()), IM_COL32(0, 0, 0, 170));
                drawList->AddText(textPos, COLOR_WHITE, weapon.m_Name.c_str());
                drawList->AddRect(pos, posEnd, COLOR_WHITE);
                if (imageRect.Contains(ImGui::GetMousePos())) {
                    drawList->AddRect(pos, posEnd, COLOR_ORANGE);
                    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_MouseLeft, false)) {
                        currentWeaponEdit = i;
                    }
                }
            }
        }
    }
}

void AutosplitsFunc()
{
    if (addSplitView) {
        if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
            addSplitView = false;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        if (ImGui::InputInt("Round to split", &addSplitRound)) {
            if (addSplitRound < -1) {
                addSplitRound = -1;
            }
            else if (addSplitRound > 255) {
                addSplitRound = 255;
            }
        }
        ImGui::SameLine();
        if (Button::RenderSingle("Add Split", ImVec2(100, 25))) {
            if (std::find_if(autosplitPresets[currentAutosplitPreset].m_Splits.begin(), autosplitPresets[currentAutosplitPreset].m_Splits.end(), [&](const auto& pair) {
                return pair.first == autosplitSelections[autosplitPresets[currentAutosplitPreset].m_Map].GetItemAtIndex();  //tombStaffSplits[tombSplits[0]];
                }) == autosplitPresets[currentAutosplitPreset].m_Splits.end())
            {
                autosplitPresets[currentAutosplitPreset].m_Splits.push_back({ autosplitSelections[autosplitPresets[currentAutosplitPreset].m_Map].GetItemAtIndex() , addSplitRound });
                autosplitPresets[currentAutosplitPreset].m_NumSplits++;
                SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
            }
        }
        ImGui::SameLine();
        HelpMarker::Render(R"(How to use:
- Select an item to add to your layout
- Input a number from -1-255 in the "Round to split" input box.
    - -1 means the split will complete when the task is completed, with no attachment to any round
    - 0 means the split will wait for the task to complete, and then wait for the current round to end
    - 1-255 means the split will wait for the task to complete, and then wait for the given round to pass
- Click the "Add Split" button to add the split to your layout)");

        if (!autosplits.m_AutosplitLists[t7Maps[autosplitPresets[currentAutosplitPreset].m_Map]].size()) {
            ImGui::Text("No additional splits supported for this map");
        }
        else {
            ImGui::Dummy(ImVec2(25, 0));
            ImGui::SameLine();
            ImGui::BeginGroup();
            for (int i = 0; i < static_cast<int>(autosplits.m_AutosplitLists[t7Maps[autosplitPresets[currentAutosplitPreset].m_Map]].size()); i++) {
                if (ImGui::BeginListBox(std::string("##" + autosplits.m_AutosplitLists[t7Maps[autosplitPresets[currentAutosplitPreset].m_Map]][i].m_Name).c_str(), ImVec2(200.0f, (autosplits.m_AutosplitLists[t7Maps[autosplitPresets[currentAutosplitPreset].m_Map]][i].m_Splits.size() * 25.0f) + 1.0f))) {
                    autosplitSelections[autosplitPresets[currentAutosplitPreset].m_Map].Render(i);
                    ImGui::EndListBox();
                }
                if (!((i + 1) % 3)) {
                    ImGui::EndGroup();
                    ImGui::Dummy(ImVec2(0, 15));
                    ImGui::Dummy(ImVec2(25, 0));
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                }
                else {
                    ImGui::SameLine();
                    ImGui::Dummy(ImVec2(25, 0));
                    ImGui::SameLine();
                }
            }
            ImGui::EndGroup();
        }
    }
    else {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
        ImGui::BeginGroup();
        if (Button::RenderSingle(ICON_FA_FILE_CIRCLE_PLUS " New Preset", ImVec2(150, 25))) {
            Popup::PrepPopup(PopupStates::AutosplitPresetCreation);
        }
        if (autosplitPresets.size()) {
            if (Button::RenderSingle(ICON_FA_FILE_CIRCLE_MINUS " Delete Preset", ImVec2(150, 25))) {
                DeleteAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
            }
            ImGui::EndGroup();
            ImGui::PopStyleVar();
            if (autosplitPresets.size()) {
                ImGui::SameLine();
                ImGui::BeginGroup();
                // presets dropdown
                ImGui::SetNextItemWidth(250);
                if (ImGui::BeginCombo("Autosplit Presets", autosplitPresets[currentAutosplitPreset].m_Name.c_str(), ImGuiComboFlags_HeightRegular)) {
                    int prevItem = currentAutosplitPreset;

                    for (int i = 0; i < autosplitPresets.size(); i++) {
                        const bool is_selected = currentAutosplitPreset == i;
                        if (ImGui::Selectable(autosplitPresets[i].m_Name.c_str(), is_selected)) {
                            currentAutosplitPreset = i;
                        }
                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    if (prevItem != currentAutosplitPreset) {
                        WriteAutosplitPresetToGame(autosplitPresets[currentAutosplitPreset]);
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                if (!GUIState::IsStateSet(Active)) {
                    ImGui::BeginDisabled();
                }
                if (ImGui::Checkbox("Active", &writeSplits)) {
                    WriteAutosplitPresetToGame(autosplitPresets[currentAutosplitPreset]);
                }
                if (!GUIState::IsStateSet(Active)) {
                    ImGui::EndDisabled();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                if (ImGui::Checkbox("In Game Timer", &autosplitPresets[currentAutosplitPreset].m_IGT)) {
                    SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                }
                ImGui::SameLine();
                if (!autosplitPresets[currentAutosplitPreset].m_Splits.size()) {
                    ImGui::BeginDisabled();
                }
                if (Button::RenderSingle("Create Livesplit Files", ImVec2(175, 25))) {
                    WriteSplitXML(autosplitPresets[currentAutosplitPreset].m_Name, autosplitPresets[currentAutosplitPreset].m_Splits);
                    WriteLayoutXML(autosplitPresets[currentAutosplitPreset].m_Name, static_cast<int>(autosplitPresets[currentAutosplitPreset].m_Splits.size()));
                }
                if (!autosplitPresets[currentAutosplitPreset].m_Splits.size()) {
                    ImGui::EndDisabled();
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 160, ImGui::GetCursorPosY() - 30));
                ImGui::BeginGroup();
                if (ImGui::Checkbox("In Game Round Timer", &autosplitPresets[currentAutosplitPreset].m_IGRT)) {
                    SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);
                int previousMap = autosplitPresets[currentAutosplitPreset].m_Map;
                if (ImGui::BeginCombo("##Select A Map", t7MapsVerbose[autosplitPresets[currentAutosplitPreset].m_Map].c_str())) {
                    if (Selection::Render(t7MapsVerbose, autosplitPresets[currentAutosplitPreset].m_Map)) {
                        if (autosplitPresets[currentAutosplitPreset].m_NumSplits) {
                            Popup::PrepPopup(PopupStates::AutosplitMapError);
                            mapError = autosplitPresets[currentAutosplitPreset].m_Map;
                            autosplitPresets[currentAutosplitPreset].m_Map = previousMap;
                        }
                        else {
                            SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(250);
                if (ImGui::BeginCombo("##Select Split Type", generalSplitData[autosplitPresets[currentAutosplitPreset].m_SplitType].c_str())) {
                    if (Selection::Render(generalSplitData, autosplitPresets[currentAutosplitPreset].m_SplitType)) {
                        SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();

                HelpMarker::Render(R"(How to use:
- Select if you want an in game timer (displays total game time with precision down to the second)
- Select if you want an in game round timer (displays current round time in seconds::milliseconds)
- Select the map to choose the splits for (defaults to SOE)
- Select the type of autosplits you want to setup. This determines the ending point of the splits. If you choose "Egg Autosplit" and add no other splits, it will only split once the egg is completed)");

                ImGui::EndGroup();
                if (ImGui::BeginTable("Splits", 4, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders)) {
                    // lol idk how else to center names in a header
                    ImGui::TableSetupColumn("                       Split Names", ImGuiTableColumnFlags_WidthFixed, 300.0f);
                    ImGui::TableSetupColumn("     Round", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Add/Remove", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Move Layer", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();

                    for (int row = 0; row < autosplitPresets[currentAutosplitPreset].m_NumSplits + 1; row++) {
                        ImGui::TableNextRow();
                        for (int column = 0; column < 4; column++) {
                            ImGui::TableSetColumnIndex(column);

                            if (row < autosplitPresets[currentAutosplitPreset].m_NumSplits) {
                                switch (column) {
                                case 0: {
                                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(autosplitPresets[currentAutosplitPreset].m_Splits[row].first.c_str()).x / 2, ImGui::GetCursorPosY() + 2.5f));
                                    ImGui::Text(autosplitPresets[currentAutosplitPreset].m_Splits[row].first.c_str());
                                    break;
                                }
                                case 1: {
                                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(std::to_string(autosplitPresets[currentAutosplitPreset].m_Splits[row].second).c_str()).x / 2, ImGui::GetCursorPosY() + 2.5f));
                                    ImGui::Text(std::to_string(autosplitPresets[currentAutosplitPreset].m_Splits[row].second).c_str());
                                    break;
                                }
                                case 2: {
                                    if (ImGui::Button(std::string(ICON_FA_CIRCLE_MINUS " Remove##" + std::to_string(row)).c_str(), ImVec2(100, 25))) {
                                        autosplitPresets[currentAutosplitPreset].m_Splits.erase(autosplitPresets[currentAutosplitPreset].m_Splits.begin() + row);
                                        autosplitPresets[currentAutosplitPreset].m_NumSplits--;
                                        SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                                        row--;
                                    }
                                    break;
                                }
                                case 3: {
                                    if (row != autosplitPresets[currentAutosplitPreset].m_NumSplits - 1) {
                                        if (ImGui::Button(std::string(ICON_FA_CIRCLE_ARROW_DOWN "##MoveSplit" + std::to_string(row)).c_str(), ImVec2(45, 25))) {
                                            std::iter_swap(autosplitPresets[currentAutosplitPreset].m_Splits.begin() + row, autosplitPresets[currentAutosplitPreset].m_Splits.begin() + row + 1);
                                            SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                                        }
                                        ImGui::SameLine();
                                    }
                                    if (row != 0) {
                                        if (row == autosplitPresets[currentAutosplitPreset].m_NumSplits - 1) {
                                            ImGui::Dummy(ImVec2(45, 0));
                                            ImGui::SameLine();
                                        }
                                        if (ImGui::Button(std::string(ICON_FA_CIRCLE_ARROW_UP "##MoveSplit" + std::to_string(row)).c_str(), ImVec2(45, 25))) {
                                            std::iter_swap(autosplitPresets[currentAutosplitPreset].m_Splits.begin() + row, autosplitPresets[currentAutosplitPreset].m_Splits.begin() + row - 1);
                                            SaveAutosplitPreset(autosplitPresets[currentAutosplitPreset]);
                                        }
                                    }
                                    break;
                                }
                                default:
                                    break;
                                }
                            }
                            else {
                                switch (column)
                                {
                                case 2: {
                                    if (ImGui::Button(std::string(ICON_FA_CIRCLE_PLUS " Add##" + std::to_string(row)).c_str(), ImVec2(100.0f, 25.0f))) {
                                        addSplitView = true;
                                    }
                                }
                                default:
                                    break;
                                }
                            }
                        }
                    }

                    ImGui::EndTable();
                }
            }
        }
        else {
            ImGui::EndGroup();
            ImGui::PopStyleVar();
        }
    }
}

void PracticePatchesFunc()
{
    for (MapPracticePatch& list : practicePatches.m_PracticePatches) {
        float spaceLeft = ImGui::GetContentRegionAvail().x;
        ImGui::BeginGroup();
        ImGui::Text(std::string(ICON_FA_LOCATION_DOT + list.m_Map).c_str());
        if (ListBox::Render("##" + list.m_Map, list.m_Items, list.m_Index, ImVec2(200, 176))) {
            WritePracticePatches();
        }
        ImGui::EndGroup();
        if (spaceLeft > 430.0f) {
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(15, 0));
            ImGui::SameLine();
        }
    }
}

void SettingsFunc()
{
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25.0f);
    TextFont::Render("In Game Keybinds", titleFont);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 135.0f);
    if (Button::RenderSingle("Reset Keybinds##In Game", ImVec2(125, 25))) {
        for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs) {
            if (hotkey.second.splitGroup != "In Game") {
                continue;
            }
            hotkey.second.keyNames = "";
        }
        RemoveDuplicates();
    }
    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(25, 25));
    ImGui::BeginGroup();
    int count = 0;
    float cursorPosX = ImGui::GetCursorPosX();
    for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs) {
        if (hotkey.second.splitGroup != "In Game") {
            continue;
        }
        ImGui::Text(hotkey.first.c_str());
        ImGui::SameLine();
        if (count < 13) {
            ImGui::SetCursorPosX(cursorPosX + 170.0f);
        }
        else {
            ImGui::SetCursorPosX(cursorPosX + 190.0f);
        }
        if (Button::RenderSingle(hotkey.second.keyNames + "##" + hotkey.first, ImVec2(225, 25)) && !registerHotKey) {
            AssignHotKey(hotkey.first, hotkey);
        }
        count++;
        if (count == 13) {
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            cursorPosX = ImGui::GetCursorPosX();
        }
    }
    ImGui::EndGroup();

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 25, ImGui::GetCursorPosY() + 25));
    TextFont::Render("Gum Tracker Keybinds", titleFont);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 135.0f);

    if (Button::RenderSingle("Reset Keybinds##Gum Track", ImVec2(125, 25))) {
        for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs) {
            if (hotkey.second.splitGroup != "Gum Tracker") {
                continue;
            }
            hotkey.second.keyNames = "";
        }
        RemoveDuplicates();
    }
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 25, ImGui::GetCursorPosY() + 25));
    ImGui::BeginGroup();
    cursorPosX = ImGui::GetCursorPosX();
    for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs) {
        if (hotkey.second.splitGroup != "Gum Tracker") {
            continue;
        }
        ImGui::Text(hotkey.first.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(cursorPosX + 200.0f);
        if (Button::RenderSingle(hotkey.second.keyNames + "##" + hotkey.first, ImVec2(225, 25)) && !registerHotKey) {
            AssignHotKey(hotkey.first, hotkey);
        }
    }
    ImGui::EndGroup();
}

void PlayerOptionsFunc()
{
    static bool displayOnly = !GUIState::IsStateSet(Active);
    if (ImGui::BeginTabBar("Player Options Tabs")) {
        if (ImGui::BeginTabItem(ICON_FA_GEAR " General Options")) {
            if (Button::RenderToggleOut("Godmode", ImVec2(100, 25), &godActive, displayOnly)) {
                NotifyGame({ 0, 11, godActive });
            }
            ImGui::SameLine();
            if (Button::RenderToggleOut("Infinite Ammo", ImVec2(150, 25), &infAmmoActive, displayOnly)) {
                NotifyGame({ 0, 12, infAmmoActive });
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(175.0f);
            if (ImGui::SliderInt("Timescale", &timescaleInt, 1, 10)) {
                NotifyGame({ 0, 13, timescaleInt });
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Weapon Options")) {
            if (Button::RenderSingle(ICON_FA_CIRCLE_PLUS " Give Selected Weapon", ImVec2(200, 25), displayOnly)) {
                std::pair<int, int> indexes = weaponSelections.GetMultiIndex();
                NotifyGame({ 0, 1, indexes.first, indexes.second, upgradedWeapon });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Current Weapon", ImVec2(200, 25), displayOnly)) {
                NotifyGame({ 0, 2, 0 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Alt Weapon", ImVec2(200, 25), displayOnly)) {
                NotifyGame({ 0, 2, 7 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Hero Weapon", ImVec2(200, 25), displayOnly)) {
                NotifyGame({ 0, 2, 6 });
            }
            ImGui::SameLine();
            ImGui::Checkbox("Upgraded", &upgradedWeapon);
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Lethals", ImVec2(158, 25), displayOnly)) {
                NotifyGame({ 0, 2, 1 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Tacticals", ImVec2(158, 25), displayOnly)) {
                NotifyGame({ 0, 2, 2 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Shield Slot", ImVec2(158, 25), displayOnly)) {
                NotifyGame({ 0, 2, 3 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Reset Melee", ImVec2(158, 25), displayOnly)) {
                NotifyGame({ 0, 2, 4 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Mine Slot", ImVec2(158, 25), displayOnly)) {
                NotifyGame({ 0, 2, 5 });
            }
            ImGui::BeginChild(ImGui::GetID("Weapon List 1"), ImVec2(310, ImGui::GetContentRegionAvail().y - 40));
            {
                if (ImGui::CollapsingHeader("Assault Rifles")) {
                    if (ImGui::ListBoxHeader("##ARs", ImVec2(300, weapons.m_Weapons[currentMap].m_AR.size() * 25 + 1.0f))) {
                        weaponSelections.Render(0);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Submachine Guns")) {
                    if (ImGui::ListBoxHeader("##SMGs", ImVec2(300, weapons.m_Weapons[currentMap].m_SMG.size() * 25 + 1.0f))) {
                        weaponSelections.Render(1);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Light Machine Guns")) {
                    if (ImGui::ListBoxHeader("##LMGs", ImVec2(300, weapons.m_Weapons[currentMap].m_LMG.size() * 25 + 1.0f))) {
                        weaponSelections.Render(2);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Shotguns")) {
                    if (ImGui::ListBoxHeader("##Shotguns", ImVec2(300, weapons.m_Weapons[currentMap].m_Shotgun.size() * 25 + 1.0f))) {
                        weaponSelections.Render(3);
                        ImGui::ListBoxFooter();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild(ImGui::GetID("Weapon List 2"), ImVec2(310, ImGui::GetContentRegionAvail().y - 40));
            {
                if (ImGui::CollapsingHeader("Snipers")) {
                    if (ImGui::ListBoxHeader("##Snipers", ImVec2(300, weapons.m_Weapons[currentMap].m_Sniper.size() * 25 + 1.0f))) {
                        weaponSelections.Render(4);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Pistols"))
                {
                    if (ImGui::ListBoxHeader("##Pistols", ImVec2(300, weapons.m_Weapons[currentMap].m_Pistol.size() * 25 + 1.0f))) {
                        weaponSelections.Render(5);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Launchers")) {
                    if (ImGui::ListBoxHeader("##Launchers", ImVec2(300, weapons.m_Weapons[currentMap].m_Launcher.size() * 25 + 1.0f))) {
                        weaponSelections.Render(6);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Melee")) {
                    if (ImGui::ListBoxHeader("##Melee", ImVec2(300, weapons.m_Weapons[currentMap].m_Melee.size() * 25 + 1.0f))) {
                        weaponSelections.Render(7);
                        ImGui::ListBoxFooter();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild(ImGui::GetID("Weapon List 3"), ImVec2(310, ImGui::GetContentRegionAvail().y - 40));
            {
                if (ImGui::CollapsingHeader("Special")) {
                    if (ImGui::ListBoxHeader("##Special", ImVec2(300, weapons.m_Weapons[currentMap].m_Special.size() * 25 + 1.0f))) {
                        weaponSelections.Render(8);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Equipment")) {
                    if (ImGui::ListBoxHeader("##Equipment", ImVec2(300, weapons.m_Weapons[currentMap].m_Equipment.size() * 25 + 1.0f))) {
                        weaponSelections.Render(9);
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Hero")) {
                    if (ImGui::ListBoxHeader("##Hero", ImVec2(300, weapons.m_Weapons[currentMap].m_Hero.size() * 25 + 1.0f))) {
                        weaponSelections.Render(10);
                        ImGui::ListBoxFooter();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Point Options")) {
            if (Button::RenderSingle(ICON_FA_CIRCLE_PLUS " Max Score", ImVec2(125, 25), displayOnly)) {
                NotifyGame({ 0, 0, 4194303 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Reset Score", ImVec2(125, 25), displayOnly)) {
                NotifyGame({ 0, 0, 500 });
            }
            ImGui::SetNextItemWidth(200.0f);
            if (ImGui::InputInt("Set Points", &pointInput, 1000, 10000, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (pointInput < 0) {
                    pointInput = 0;
                }
                else if (pointInput > 4194303) {
                    pointInput = 4194303;
                }
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_ARROW_RIGHT " Send##Point Input", ImVec2(80, 25), displayOnly)) {
                NotifyGame({ 0, 0, pointInput });
            }
            ImGui::SameLine();
            HelpMarker::Render("Increment +- 1000\nCtrl + Click increment +- 10000");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Perk Options")) {
            if (Button::RenderSingle(ICON_FA_CIRCLE_PLUS " Give All Perks", ImVec2(145, 25), displayOnly)) {
                NotifyGame({ 0, 7 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take All Perks", ImVec2(145, 25), displayOnly)) {
                NotifyGame({ 0, 8 });
            }
            if (Button::RenderSingle(ICON_FA_CIRCLE_PLUS " Give Perk", ImVec2(145, 25), displayOnly)) {
                NotifyGame({ 0, 9, perkSelectIndex });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Perk", ImVec2(145, 25), displayOnly)) {
                NotifyGame({ 0, 10, perkSelectIndex });
            }
            ListBox::Render("##Perk List", perks.m_Perks[currentMap], perkSelectIndex, ImVec2(300, 300));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Gobblegum Options")) {
            if (Button::RenderSingle(ICON_FA_CIRCLE_PLUS " Give Classic Gum", ImVec2(125, 25), displayOnly)) {
                NotifyGame({ 0, 3, bgbClassicSelectIndex });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_PLUS " Give Mega Gum", ImVec2(175, 25), displayOnly)) {
                NotifyGame({ 0, 3, bgbMegaSelectIndex + 19 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Gum", ImVec2(125, 25), displayOnly)) {
                NotifyGame({ 0, 4 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Take Gum Charge", ImVec2(175, 25), displayOnly)) {
                NotifyGame({ 0, 5 });
            }
            ImGui::SameLine();
            if (Button::RenderSingle(ICON_FA_CIRCLE_MINUS " Activate Gum", ImVec2(175, 25), displayOnly)) {
                NotifyGame({ 0, 6 });
            }
            if (ImGui::ListBoxHeader("##Classic Gums List", ImVec2(300, ImGui::GetContentRegionAvail().y - 40))) {
                Selection::RenderBGB(bgbs.m_Classics, bgbClassicSelectIndex);
                ImGui::ListBoxFooter();
            }
            ImGui::SameLine();
            if (ImGui::ListBoxHeader("##Megas Gums List", ImVec2(300, ImGui::GetContentRegionAvail().y - 40))) {
                Selection::RenderBGB(bgbs.m_Megas, bgbMegaSelectIndex);
                ImGui::ListBoxFooter();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void ZombieOptionsFunc()
{
    static bool displayOnly = !GUIState::IsStateSet(Active);
    if (Button::RenderToggleOut("Ignore Players", ImVec2(140, 25), &zombiesIgnore, displayOnly)) {
        NotifyGame({ 1, 0, zombiesIgnore });
    }
    ImGui::SameLine();
    if (Button::RenderSingle("Kill Horde", ImVec2(140, 25), displayOnly)) {
        NotifyGame({ 1, 1 });
    }
    ImGui::SameLine();
    if (Button::RenderToggleOut("Freeze Zombies", ImVec2(140, 25), &zombiesFreeze, displayOnly)) {
        NotifyGame({ 1, 2, zombiesFreeze });
    }
    ImGui::SameLine();
    if (Button::RenderToggleOut("Toggle Spawning", ImVec2(140, 25), &zombiesSpawn, displayOnly)) {
        NotifyGame({ 1, 3, zombiesSpawn });
    }
    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Text("Zombie Speed Options");
    ImGui::SameLine();
    HelpMarker::Render("Only 1 option may be active at once");
    ImGui::Dummy(ImVec2(0, 5));

    if (Button::RenderToggleOut("Walk", ImVec2(100, 25), &zombieSpeedWalk, displayOnly)) {
        zombieSpeedRun = false;
        zombieSpeedSprint = false;
        if (zombieSpeedWalk) {
            NotifyGame({ 1, 4, 0 });
        }
        else {
            NotifyGame({ 1, 4, 3 });
        }
    }
    ImGui::SameLine();
    if (Button::RenderToggleOut("Run", ImVec2(100, 25), &zombieSpeedRun, displayOnly)) {
        zombieSpeedWalk = false;
        zombieSpeedSprint = false;
        if (zombieSpeedRun) {
            NotifyGame({ 1, 4, 1 });
        }
        else {
            NotifyGame({ 1, 4, 3 });
        }
    }
    ImGui::SameLine();
    if (Button::RenderToggleOut("Sprint", ImVec2(100, 25), &zombieSpeedSprint, displayOnly)) {
        zombieSpeedWalk = false;
        zombieSpeedRun = false;
        if (zombieSpeedSprint) {
            NotifyGame({ 1, 4, 2 });
        }
        else {
            NotifyGame({ 1, 4, 3 });
        }
    }

}

void RoundOptionsFunc()
{
    if (Button::RenderSingle("End Round", ImVec2(120, 25))) {
        NotifyGame({ 2, 0 });
    }
    ImGui::SameLine();
    if (Button::RenderSingle("Restart Round", ImVec2(120, 25))) {
        NotifyGame({ 2, 1 });
    }
    ImGui::SetNextItemWidth(125.0f);
    if (ImGui::InputInt("Set Round", &roundInput, 1, 10)) {
        if (roundInput < 1) {
            roundInput = 1;
        }
        else if (roundInput > 255) {
            roundInput = 255;
        }
    }
    ImGui::SameLine();
    if (Button::RenderSingle(ICON_FA_ARROW_RIGHT " Send##Round Input", ImVec2(80, 25))) {
        NotifyGame({ 2, 1, roundInput });
    }
    ImGui::SameLine();
    HelpMarker::Render("Increment +- 1\nCtrl + Click increment +- 10");
    ImGui::SetNextItemWidth(100.0f);
    if (ImGui::InputInt("Set Zombie Count", &zombieCount, NULL, NULL, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (zombieCount < 0) {
            zombieCount = 0;
        }
        else if (zombieCount > INT_MAX) {
            zombieCount = INT_MAX;
        }
    }
    ImGui::SameLine();
    if (Button::RenderSingle(ICON_FA_ARROW_RIGHT " Send##Zombie Count", ImVec2(80, 25))) {
        NotifyGame({ 2, 2, zombieCount });
    }
}

void PowerupOptionsFunc()
{
    ListBox::Render("##Powerup List", powerups.m_Powerups[currentMap], powerupListIndex, ImVec2(300, (powerups.m_Powerups[currentMap].size() * 25 + 1.0f)));
    ImGui::SameLine();
    ImGui::BeginGroup();
    if (Button::RenderSingle("Give Powerup", ImVec2(125, 25))) {
        NotifyGame({ 3, 0, powerupListIndex, instaGrab });
    }
    ImGui::Checkbox("Insta Grab", &instaGrab);
    ImGui::EndGroup();
}

// TODO: Refactor this
void EggStepOptionsFunc()
{
    HelpMarker::Render("This can be buggy and may not work correctly for every situation. The game may crash in certain events, use at your own risk. For best results, don't do multiple steps from different boxes at once.");
    switch (hashstr(currentMap.c_str())) {
    case hashstr("zm_zod"): {
        // row 1
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Ritual Options");
            ListBox::Render("##Ritual Options", zodRitualSteps, zodRitualIndex, ImVec2(300, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Ritual", ImVec2(125, 25))) {
                NotifyGame({ 4, 0, zodRitualIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Ritual", ImVec2(155, 25))) {
                NotifyGame({ 4, 0, 4 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Egg Options");
            ListBox::Render("##Egg Options", zodEggSteps, zodEggIndex, ImVec2(300, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Egg", ImVec2(125, 25))) {
                if (zodEggIndex < 2) {
                    NotifyGame({ 4, 2, zodEggIndex });
                }
                else {
                    NotifyGame({ 4, 2, zodEggIndex + 1 });
                }
            }
            if (Button::RenderSingle("Complete All Steps##Egg", ImVec2(155, 25))) {
                NotifyGame({ 4, 1 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        // row 2
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Ovum Options");
            ListBox::Render("##Ovum Options", zodOvumSteps, zodOvumIndex, ImVec2(300, 150));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Ovum", ImVec2(125, 25))) {
                NotifyGame({ 4, 4, zodOvumIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Ovum", ImVec2(155, 25))) {
                NotifyGame({ 4, 3 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Flag Options");
            ListBox::Render("##Flag Options", zodFlagSteps, zodFlagIndex, ImVec2(300, 150));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Flag", ImVec2(125, 25))) {
                NotifyGame({ 4, 6, zodFlagIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Flag", ImVec2(155, 25))) {
                NotifyGame({ 4, 5 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        break;
    }
    case hashstr("zm_castle"): {
        // row 1
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Dragon Options");
            ListBox::Render("##Dragon Options", castleDragonSteps, castleDragonIndex, ImVec2(155, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Dragon", ImVec2(125, 25))) {
                NotifyGame({ 4, 0, castleDragonIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Dragon", ImVec2(155, 25))) {
                NotifyGame({ 4, 1 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Lighnting Options");
            ListBox::Render("##Lighnting Options", castleLighntingSteps, castleLighntingIndex, ImVec2(145, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Lighnting", ImVec2(125, 25))) {
                NotifyGame({ 4, 2, castleLighntingIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Lighnting", ImVec2(155, 25))) {
                NotifyGame({ 4, 2, 6 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Fire Options");
            ListBox::Render("##Fire Options", castleFireSteps, castleFireIndex, ImVec2(150, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Fire", ImVec2(125, 25))) {
                NotifyGame({ 4, 3, castleFireIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Fire", ImVec2(155, 25))) {
                NotifyGame({ 4, 3, 6 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        // row 2
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Void Options");
            ListBox::Render("##Void Options", castleVoidSteps, castleVoidIndex, ImVec2(150, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Void", ImVec2(125, 25))) {
                NotifyGame({ 4, 4, castleVoidIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Void", ImVec2(155, 25))) {
                NotifyGame({ 4, 4, 6 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Wolf Options");
            ListBox::Render("##Wolf Options", castleWolfSteps, castleWolfIndex, ImVec2(150, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Wolf", ImVec2(125, 25))) {
                NotifyGame({ 4, 5, castleWolfIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Wolf", ImVec2(155, 25))) {
                NotifyGame({ 4, 5, 5 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Time Travel Options");
            ListBox::Render("##Time Travel Options", castleTimeTravelSteps, castleTimeTravelIndex, ImVec2(150, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Time Travel", ImVec2(125, 25))) {
                NotifyGame({ 4, 6, castleTimeTravelIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Time Travel", ImVec2(155, 25))) {
                NotifyGame({ 4, 6, 2 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        // row 3
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Simon Options");
            ListBox::Render("##Simon Options", castleSimonSteps, castleSimonIndex, ImVec2(150, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Simon", ImVec2(125, 25))) {
                NotifyGame({ 4, 7, castleSimonIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Simon", ImVec2(155, 25))) {
                NotifyGame({ 4, 8 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Keeper Options");
            ListBox::Render("##Keeper Options", castleKeeperSteps, castleKeeperIndex, ImVec2(150, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Keeper", ImVec2(125, 25))) {
                NotifyGame({ 4, 9, castleKeeperIndex });
            }
            if (Button::RenderSingle("Complete All Steps##Keeper", ImVec2(155, 25))) {
                NotifyGame({ 4, 10 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        break;
    }
    case hashstr("zm_island"): {
        // row 1
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Skull Options");
            ListBox::Render("##Skull Options", islandSkullSteps, islandSkullIndex, ImVec2(300, 126));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Skull", ImVec2(125, 25))) {
                NotifyGame({ 4, 0, islandSkullIndex + 1 });
            }
            if (Button::RenderSingle("Complete All Steps##Skull", ImVec2(155, 25))) {
                NotifyGame({ 4, 1 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " EE Options");
            ListBox::Render("##EE Options", islandEESteps, islandEEIndex, ImVec2(300, 176));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##EE", ImVec2(125, 25))) {
                NotifyGame({ 4, islandEEIndex + 2 });
            }
            if (Button::RenderSingle("Complete All Steps##EE", ImVec2(155, 25))) {
                NotifyGame({ 4, 9 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        break;
    }
    case hashstr("zm_stalingrad"): {
        // row 1
        {
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " PAP Options");
            ListBox::Render("##PAP Options", stalingradPAPSteps, stalingradPAPIndex, ImVec2(125, 26));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##PAP", ImVec2(125, 25))) {
                NotifyGame({ 4, 0 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Gauntlet Options");
            ListBox::Render("##Gauntlet Options", stalingradGauntletSteps, stalingradGauntletIndex, ImVec2(185, 201));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Gauntlet", ImVec2(125, 25))) {
                NotifyGame({ 4, stalingradGauntletIndex + 1 });
            }
            if (Button::RenderSingle("Complete All Steps##Gauntlet", ImVec2(155, 25))) {
                NotifyGame({ 4, 8 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text(ICON_FA_GEAR " Main Options");
            ListBox::Render("##Main Options", stalingradEESteps, stalingradEEIndex, ImVec2(175, 151));
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (Button::RenderSingle("Complete Step##Main", ImVec2(125, 25))) {
                NotifyGame({ 4, stalingradEEIndex + 9 });
            }
            if (Button::RenderSingle("Complete All Steps##Main", ImVec2(155, 25))) {
                NotifyGame({ 4, 14 });
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
        }
        break;
    }
    default:
        break;
    }
}

void CraftableOptionsFunc()
{
    int craftSize = (int)craftables.m_Craftables[currentMap].size();
    if (craftSize > 0) {
        if (Button::RenderSingle("Pickup Every Part", ImVec2(150, 25))) {
            NotifyGame({ 5, 0 });
        }
        for (int i = 0; i < craftSize; i++) {
            if (ImGui::CollapsingHeader(craftables.m_Craftables[currentMap][i].m_CraftableName.c_str())) {
                std::string listName = "##Craftables" + currentMap + std::to_string(i);
                ImGui::SameLine();
                ImGui::BeginGroup();
                if (Button::RenderSingle("Pickup Part##" + std::to_string(i), ImVec2(150, 25))) {
                    NotifyGame({ 5, 1, i, craftComboIndexes[currentMap][i] });
                }
                if (Button::RenderSingle("Pickup All Parts##" + std::to_string(i), ImVec2(150, 25))) {
                    NotifyGame({ 5, 1, i, 0, 1 });
                }
                ImGui::EndGroup();
            }
        }
    }
    else {
        ImGui::Text("No Craftables For Current Map");
    }
}

void BlockerOptionsFunc()
{
    if (Button::RenderSingle("Open All Doors", ImVec2(150, 25))) {
        NotifyGame({ 6, 0 });
    }
    ImGui::SameLine();
    if (Button::RenderSingle("Global Power On", ImVec2(150, 25))) {
        NotifyGame({ 6, 2 });
    }
    ImGui::SameLine();
    if (Button::RenderSingle("Open All Barriers", ImVec2(150, 25))) {
        NotifyGame({ 6, 5, 0 });
    }
    ImGui::SameLine();
    if (Button::RenderSingle("Close All Barriers", ImVec2(150, 25))) {
        NotifyGame({ 6, 5, 1 });
    }

    for (const MapBlockersButton& button : blockersButtons.m_Buttons[currentMap]) {
        if (Button::RenderSingle(button.m_Name, ImVec2(ImGui::CalcTextSize(button.m_Name.c_str()).x + 10, 25))) {
            if (button.m_ListIndex >= 0) {
                NotifyGame({ 6, button.m_NotifyNum1, blockers.m_BlockersLists[currentMap][button.m_ListIndex].m_Index });
            }
            else {
                NotifyGame({ 6, button.m_NotifyNum1, button.m_NotifyNum2 });
            }
        }
        if (&button != &blockersButtons.m_Buttons[currentMap].back()) {
            ImGui::SameLine();
        }
    }

    for (MapBlockers& list : blockers.m_BlockersLists[currentMap]) {
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150.0f - ImGui::CalcTextSize(list.m_Name.c_str()).x / 2);
        ImGui::Text(list.m_Name.c_str());
        ListBox::Render("##" + list.m_Name, list.m_Blockers, list.m_Index, ImVec2(300.0f, min(list.m_Blockers.size() * 25 + 1.0f, 401)));
        ImGui::EndGroup();
        ImGui::SameLine();
    }
}

void MapOptionsFunc()
{
    ImGui::BeginGroup();
    ListBox::Render("##Load Map List", t7MapsVerbose, mapListIndex, ImVec2(200, 175));
    ImGui::SameLine();
    if (Button::RenderSingle("Load Map", ImVec2(100, 25))) {
        NotifyGame({ 7, 0, mapListIndex });
    }
    ImGui::EndGroup();
}

void GumTrackerFunc()
{
    if (!showBGBSelection) {
        if (bgbPresets.size()) {
            ImGui::SetNextItemWidth(250);
            if (ImGui::BeginCombo("Gum Presets", bgbPresets[currentBGBPreset].m_Name.c_str(), ImGuiComboFlags_HeightRegular)) {
                for (int i = 0; i < bgbPresets.size(); i++) {
                    const bool is_selected = currentBGBPreset == i;
                    if (ImGui::Selectable(bgbPresets[i].m_Name.c_str(), is_selected)) {
                        currentBGBPreset = i;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (Button::RenderSingle("Load Preset", ImVec2(100, 25)) && bgbPresets.size()) {
                for (int i = 0; i < 5; i++) {
                    gumTrackBGBs[i] = bgbPresets[currentBGBPreset].m_BGBs[i];
                }
            }
        }

        ImGui::Dummy(ImVec2(25, 0));
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
        for (int i = 0; i < 5; i++)
        {
            ImGui::BeginGroup();
            if (Button::RenderSingle(std::string("Choose Gum##" + std::to_string(i)) + std::to_string(i + 1), ImVec2(155, 25))) {
                showBGBSelection = true;
                gumTrackCurrentIndex = i;
                bgbToSwap = gumTrackBGBs[i];
            }
            if (i == gumTrackCurrentIndex) {
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_BLUE_WEAK);
            }
            if (!gumTrackChosen[i]) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
            }
            if (ImGui::ImageButton(gumTrackBGBs[i].m_Name.c_str(), bgbImgList[gumTrackBGBs[i].m_Name]->GetDescriptorSet(), ImVec2(145, 145))) {
                gumTrackChosen[i] = !gumTrackChosen[i];
                bgbToSwap = bgbContext;
                if (std::all_of(std::begin(gumTrackChosen), std::end(gumTrackChosen), [](bool value) { return value; })) {
                    std::fill(std::begin(gumTrackChosen), std::end(gumTrackChosen), false);
                }
            }
            ImGui::PopStyleColor(ImGui::GetCurrentContext()->ColorStack.Size - 2);
            if (ImGui::IsItemHovered()) {
                bgbContext = gumTrackBGBs[i];
                gumTrackCurrentIndex = i;
            }
            ImGui::EndGroup();
            ImGui::PopStyleVar(ImGui::GetCurrentContext()->StyleVarStack.Size);

            if (i != 4) {
                ImGui::SameLine();
            }
        }
        ImGui::PopStyleColor(2);
    }
    else {
        if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
            showBGBSelection = false;
        }
        ImGui::SameLine();
        if (ImGui::BeginTabBar("Gum Type Choice")) {
            if (ImGui::BeginTabItem("Classics")) {
                static char searchText[64] = "";
                if (gumSelectMenu) {
                    strcpy_s(searchText, "");
                }
                gumSelectMenu = 0;
                if (!strcmp(searchText, "")) {
                    bgbDisplayList = bgbs.m_Classics;
                }
                if (ImGui::InputText("Gobblegum Search", searchText, IM_ARRAYSIZE(searchText))) {
                    bgbDisplayList = BGBSearch(0, searchText);
                }
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
                ImGui::BeginGroup();
                int numBGB = 0;
                for (const BGB& bgb : bgbDisplayList) {
                    if (ImGui::ImageButton(bgb.m_Name.c_str(), bgbImgList[bgb.m_Name]->GetDescriptorSet(), ImVec2(145, 145))) {
                        bgbContext = bgb;
                        strcpy_s(searchText, "");
                        showBGBSelection = false;
                        SwapBGBTrack(bgbToSwap, bgbContext);
                        break;
                    }
                    if (ImGui::IsItemHovered()) {
                        bgbContext = bgb;
                    }
                    numBGB++;
                    if (numBGB > 5) {
                        numBGB = 0;
                        ImGui::EndGroup();
                        ImGui::BeginGroup();
                    }
                    else {
                        ImGui::SameLine();
                    }
                }
                ImGui::EndGroup();
                ImGui::PopStyleColor(2);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Megas")) {
                static char searchText[64] = "";
                if (!gumSelectMenu) {
                    strcpy_s(searchText, "");
                }
                gumSelectMenu = 1;
                if (!strcmp(searchText, "")) {
                    bgbDisplayList = bgbs.m_Megas;
                }
                if (ImGui::InputText("Gobblegum Search", searchText, IM_ARRAYSIZE(searchText))) {
                    bgbDisplayList = BGBSearch(1, searchText);
                }
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
                ImGui::BeginGroup();
                int numBGB = 0;
                for (const BGB& bgb : bgbDisplayList) {
                    if (ImGui::ImageButton(bgb.m_Name.c_str(), bgbImgList[bgb.m_Name]->GetDescriptorSet(), ImVec2(145, 145))) {
                        bgbContext = bgb;
                        strcpy_s(searchText, "");
                        showBGBSelection = false;
                        SwapBGBTrack(bgbToSwap, bgbContext);
                        break;
                    }
                    if (ImGui::IsItemHovered()) {
                        bgbContext = bgb;
                    }
                    numBGB++;
                    if (numBGB > 5) {
                        numBGB = 0;
                        ImGui::EndGroup();
                        ImGui::BeginGroup();
                    }
                    else {
                        ImGui::SameLine();
                    }
                }
                ImGui::EndGroup();
                ImGui::PopStyleColor(2);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    ImGui::Begin("##Gum Context Menu", 0, dockFlags);
    ImGui::Image(bgbImgList[bgbContext.m_Name]->GetDescriptorSet(), ImVec2(196, 196));
    ImGui::SameLine();
    ImVec2 contPos = ImGui::GetCursorPos();
    TextFont::Render(bgbImgList[bgbContext.m_Name]->GetFilename().c_str(), sidebarFont, 1, 0);
    ImGui::SetCursorPos(ImVec2(contPos.x, contPos.y + 30));
    TextFont::RenderWrapped(bgbContext.m_Description.c_str(), sidebarFont, 0, 1);
    ImGui::End();
}

void ZombieCalculatorFunc()
{
    if (ImGui::BeginTabBar("Zombie Calc Tabs")) {
        if (ImGui::BeginTabItem("Basic")) {
            // Calc Player Input
            {
                ImGui::BeginChild("Calc Input", ImVec2(300, 130), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players").x) / 2 + 10);
                ImGui::Text("Number Of Players");
                ImGui::SetNextItemWidth(170.0f);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 + 10);
                if (ImGui::InputInt("##Number Of Players", &playerCount, 1, NULL)) {
                    if (playerCount < 1) {
                        playerCount = 1;
                    }
                    else if (playerCount > 4) {
                        playerCount = 4;
                    }
                    zombiesForRound = GetZombieCountForRound(roundNumber, playerCount);
                    hordesForRound = zombiesForRound / 24.0f;
                    zombiesUpToRound = GetZombiesUpToRound(roundNumber, playerCount);
                    zombieHealthForRound = GetZombieHealthForRound(roundNumber);
                    zombieSpawnRate = GetZombieSpawnRateForRound(roundNumber, playerCount);
                    zombieSpawnRateStr = ParseTimeFromMilli(zombieSpawnRate);
                    roundTime = CalcRoundTime(roundNumber, playerCount);
                    expectedRoundTime = CalcRoundTime(roundNumber, playerCount, corpseDelay[playerCount - 1][roundNumber - 1]);
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Current Round").x) / 2 + 10);
                ImGui::Text("Current Round");
                ImGui::SetNextItemWidth(170.0f);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 + 10);
                if (ImGui::InputInt("##Current Round", &roundNumber, 1, NULL)) {
                    if (roundNumber < 1) {
                        roundNumber = 1;
                    }
                    else if (roundNumber > 255) {
                        roundNumber = 255;
                    }
                    zombiesForRound = GetZombieCountForRound(roundNumber, playerCount);
                    hordesForRound = zombiesForRound / 24.0f;
                    zombiesUpToRound = GetZombiesUpToRound(roundNumber, playerCount);
                    zombieHealthForRound = GetZombieHealthForRound(roundNumber);
                    zombieSpawnRate = GetZombieSpawnRateForRound(roundNumber, playerCount);
                    zombieSpawnRateStr = ParseTimeFromMilli(zombieSpawnRate);
                    roundTime = CalcRoundTime(roundNumber, playerCount);
                    expectedRoundTime = CalcRoundTime(roundNumber, playerCount, corpseDelay[playerCount - 1][roundNumber - 1]);
                }
                ImGui::EndChild();
            }
            // Calc Data 1
            {
                ImGui::SameLine();
                ImGui::BeginChild("Calc Data 1", ImVec2(300, 130), true);
                std::string numZombiesText("Zombies On Round " + std::to_string(roundNumber) + ":");
                std::string numZombiesNum(CommifyNumString(zombiesForRound));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesText.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesNum.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesNum.c_str());
                std::string numHordesText("Hordes On Round " + CommifyNumString(roundNumber) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numHordesText.c_str()).x) / 2 + 10);
                ImGui::Text(numHordesText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(CommifyNumString(hordesForRound).c_str()).x) / 2 + 10);
                ImGui::Text(CommifyNumString(hordesForRound).c_str());
                ImGui::EndChild();
            }
            // Calc Data 2
            {
                ImGui::SameLine();
                ImGui::BeginChild("Calc Data 2", ImVec2(300, 130), true);
                std::string numZombiesUpToText("Zombies Up To Round " + std::to_string(roundNumber) + ":");
                std::string numZombiesUpToNum(CommifyNumString(zombiesUpToRound));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesUpToText.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesUpToText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesUpToNum.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesUpToNum.c_str());
                std::string numZombieHealthText("Zombie Health On Round " + CommifyNumString(roundNumber) + ":");
                std::string numZombieHealthNum(CommifyNumString(zombieHealthForRound));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombieHealthText.c_str()).x) / 2 + 10);
                ImGui::Text(numZombieHealthText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombieHealthNum.c_str()).x) / 2 + 10);
                ImGui::Text(numZombieHealthNum.c_str());
                ImGui::EndChild();
            }
            ImGui::Dummy(ImVec2(150, 0));
            ImGui::SameLine();
            // Calc Data 3
            {
                ImGui::BeginChild("Calc Data 3", ImVec2(300, 130), true);
                std::string spawnRateText("Spawn Rate On Round " + std::to_string(roundNumber) + ":");
                std::string spawnRateNum(zombieSpawnRateStr);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spawnRateText.c_str()).x) / 2 + 10);
                ImGui::Text(spawnRateText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spawnRateNum.c_str()).x) / 2 + 10);
                ImGui::Text(spawnRateNum.c_str());
                std::string roundTimeText("Perfect Round Time For Round " + std::to_string(roundNumber) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(roundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(roundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(roundTime.c_str()).x) / 2 + 10);
                ImGui::Text(roundTime.c_str());
                ImGui::EndChild();
            }
            // Calc Data 4
            {
                ImGui::SameLine();
                ImGui::BeginChild("Calc Data 4", ImVec2(300, 130), true);
                std::string corpseDelayText("Corpse Delay On Round " + std::to_string(roundNumber) + ":");
                std::string corpseDelayNum(CommifyNumString(corpseDelay[playerCount - 1][roundNumber - 1]));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(corpseDelayText.c_str()).x) / 2 + 10);
                ImGui::Text(corpseDelayText.c_str());
                HelpMarker::Render("Expected corpse delay killing every zombie exactly 2.25 seconds after it spawns. Actual value may vary in game.");
                ImGui::SameLine();
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(corpseDelayNum.c_str()).x) / 2 + 10);
                ImGui::Text(corpseDelayNum.c_str());
                std::string expectedRoundTimeText("Expected Round Time For Round " + std::to_string(roundNumber) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(expectedRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(expectedRoundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(expectedRoundTime.c_str()).x) / 2 + 10);
                ImGui::Text(expectedRoundTime.c_str());
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Advanced")) {
            // Calc Data 5
            {
                ImGui::BeginChild("Calc Data 5", ImVec2(300, 400), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                ImGui::Dummy(ImVec2(7.5f, 0));
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - Special Enemies", &specialEnemyPlayerCount, 1, NULL)) {
                    if (specialEnemyPlayerCount < 1) {
                        specialEnemyPlayerCount = 1;
                    }
                    else if (specialEnemyPlayerCount > 4) {
                        specialEnemyPlayerCount = 4;
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                    meatballDelay = "Meatball Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Meatballs"));
                    dogDelay = "Dog Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
                    spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
                    manglerDelay = "Mangler Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Manglers"));
                    valkDelay = "Valk Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Valks"));
                    furyDelay = "Fury Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Furys"));
                    keeperDelay = "Keeper Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Keepers"));
                    if (meatballCount > specialZombiesForRound) {
                        meatballCount = specialZombiesForRound;
                        specialEnemyCount_1 = meatballCount;
                    }
                    if (dogCount > specialZombiesForRound) {
                        dogCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (islandSpidersCount > specialZombiesForRound) {
                        islandSpidersCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (manglersCount + valksCount > specialZombiesForRound) {
                        manglersCount = specialZombiesForRound - valksCount;
                        specialEnemyCount_1 = manglersCount;
                    }
                    if (valksCount + manglersCount > specialZombiesForRound) {
                        valksCount = specialZombiesForRound - manglersCount;
                        specialEnemyCount_2 = valksCount;
                    }
                    if (lastEditedSpecialEnemy == "GenesisSpiders" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                        genesisSpidersCount = specialZombiesForRound - furysCount - keepersCount;
                        specialEnemyCount_1 = genesisSpidersCount;
                    }
                    if (lastEditedSpecialEnemy == "Furys" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                        furysCount = specialZombiesForRound - genesisSpidersCount - keepersCount;
                        specialEnemyCount_2 = furysCount;
                    }
                    if (lastEditedSpecialEnemy == "Keepers" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                        keepersCount = specialZombiesForRound - genesisSpidersCount - furysCount;
                        specialEnemyCount_3 = keepersCount;
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                } ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - Special Enemies", &specialEnemyRound, 1, NULL)) {
                    if (specialEnemyRound < 1) {
                        specialEnemyRound = 1;
                    }
                    else if (specialEnemyRound > 255) {
                        specialEnemyRound = 255;
                    }
                    specialZombiesForRound = GetZombieCountForRound(specialEnemyRound, specialEnemyPlayerCount);
                    if (meatballCount > specialZombiesForRound) {
                        meatballCount = specialZombiesForRound;
                        specialEnemyCount_1 = meatballCount;
                    }
                    if (dogCount > specialZombiesForRound) {
                        dogCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (islandSpidersCount > specialZombiesForRound) {
                        islandSpidersCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (manglersCount + valksCount > specialZombiesForRound) {
                        manglersCount = specialZombiesForRound - valksCount;
                        specialEnemyCount_1 = manglersCount;
                    }
                    if (valksCount + manglersCount > specialZombiesForRound) {
                        valksCount = specialZombiesForRound - manglersCount;
                        specialEnemyCount_2 = valksCount;
                    }
                    if (lastEditedSpecialEnemy == "GenesisSpiders" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                        genesisSpidersCount = specialZombiesForRound - furysCount - keepersCount;
                        specialEnemyCount_1 = genesisSpidersCount;
                    }
                    if (lastEditedSpecialEnemy == "Furys" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                        furysCount = specialZombiesForRound - genesisSpidersCount - keepersCount;
                        specialEnemyCount_2 = furysCount;
                    }
                    if (lastEditedSpecialEnemy == "Keepers" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                        keepersCount = specialZombiesForRound - genesisSpidersCount - furysCount;
                        specialEnemyCount_3 = keepersCount;
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                }
                int prevMap = currentSpecialEnemyMap;
                int prevDogRoundCount = numDogRounds;
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("Map").x / 2 + 10);
                if (ImGui::BeginCombo("Map", specialEnemiesMapCombo[currentSpecialEnemyMap].c_str(), ImGuiComboFlags_HeightRegular)) {
                    for (int i = 0; i < specialEnemiesMapCombo.size(); i++) {
                        const bool is_selected = currentSpecialEnemyMap == i;
                        if (ImGui::Selectable(specialEnemiesMapCombo[i].c_str(), is_selected)) {
                            currentSpecialEnemyMap = i;
                        }
                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                if (prevMap != currentSpecialEnemyMap) {
                    switch (hashstr(specialEnemiesMapCombo[currentSpecialEnemyMap].c_str())) {
                    case hashstr("SOE"): {
                        specialEnemyCount_1 = meatballCount;
                        specialEnemyCount_2 = 0;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Meatballs";
                        break;
                    }
                    case hashstr("Giant + DE"): {
                        specialEnemyCount_1 = dogCount;
                        specialEnemyCount_2 = 0;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Dogs";
                        break;
                    }
                    case hashstr("ZNS"): {
                        specialEnemyCount_1 = islandSpidersCount;
                        specialEnemyCount_2 = 0;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Spiders";
                        break;
                    }
                    case hashstr("GK"): {
                        specialEnemyCount_1 = manglersCount;
                        specialEnemyCount_2 = valksCount;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Manglers";
                        specialEnemy_2 = "Valks";
                        break;
                    }
                    case hashstr("Rev"): {
                        specialEnemyCount_1 = genesisSpidersCount;
                        specialEnemyCount_2 = furysCount;
                        specialEnemyCount_3 = keepersCount;
                        specialEnemy_1 = "Spiders";
                        specialEnemy_2 = "Furys";
                        specialEnemy_3 = "Keepers";
                        break;
                    }
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                }
                switch (hashstr(specialEnemiesMapCombo[currentSpecialEnemyMap].c_str())) {
                case hashstr("SOE"): {
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(meatballDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(meatballDelay.c_str());
                    HelpMarker::Render("Bugs add no spawn delay, and are thus not included.");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(130.0f);
                    if (ImGui::InputInt("# of Meatballs", &meatballCount, 1, NULL)) {
                        if (meatballCount < 0) {
                            meatballCount = 0;
                        }
                        if (meatballCount > specialZombiesForRound) {
                            meatballCount = specialZombiesForRound;
                        }
                        specialEnemyCount_1 = meatballCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        meatballDelay = "Meatball Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Meatballs"));
                        lastEditedSpecialEnemy = "Meatballs";
                    }
                    break;
                }
                case hashstr("Giant + DE"): {
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(dogDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(dogDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Dogs").x / 2 + 10);
                    if (ImGui::InputInt("# of Dogs", &dogCount, 1, NULL)) {
                        if (dogCount < 0) {
                            dogCount = 0;
                        }
                        if (dogCount > specialZombiesForRound) {
                            dogCount = specialZombiesForRound;
                        }
                        specialEnemyCount_1 = dogCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        dogDelay = "Dogs Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
                        lastEditedSpecialEnemy = "Dogs";
                    }
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Dog Rounds").x / 2 + 10);
                    if (ImGui::BeginCombo("# of Dog Rounds", dogRoundCountCombo[numDogRounds - 1].c_str(), ImGuiComboFlags_HeightRegular)) {
                        for (int i = 1; i < dogRoundCountCombo.size() + 1; i++) {
                            const bool is_selected = numDogRounds == i;
                            if (ImGui::Selectable(dogRoundCountCombo[i - 1].c_str(), is_selected)) {
                                numDogRounds = i;
                            }
                            if (is_selected) {
                                ImGui::SetItemDefaultFocus();
                                specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                                dogDelay = "Dogs Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (prevDogRoundCount != numDogRounds) {
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                    }
                    break;
                }
                case hashstr("ZNS"): {
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spiderDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(spiderDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Spiders").x / 2 + 10);
                    if (ImGui::InputInt("# of Spiders", &islandSpidersCount, 1, NULL)) {
                        if (islandSpidersCount < 0) {
                            islandSpidersCount = 0;
                        }
                        if (islandSpidersCount > specialZombiesForRound) {
                            islandSpidersCount = specialZombiesForRound;
                        }
                        specialEnemyCount_1 = islandSpidersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
                        lastEditedSpecialEnemy = "IslandSpiders";
                    }
                    break;
                }
                case hashstr("GK"): {
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(manglerDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(manglerDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Manglers").x / 2 + 10);
                    if (ImGui::InputInt("# of Manglers", &manglersCount, 1, NULL)) {
                        if (manglersCount < 0) {
                            manglersCount = 0;
                        }
                        if (manglersCount + valksCount > specialZombiesForRound) {
                            manglersCount = specialZombiesForRound - valksCount;
                        }
                        specialEnemyCount_1 = manglersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        manglerDelay = "Mangler Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Manglers"));
                        lastEditedSpecialEnemy = "Manglers";
                    }
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(valkDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(valkDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Valks").x / 2 + 10);
                    if (ImGui::InputInt("# of Valks", &valksCount, 1, NULL)) {
                        if (valksCount < 0) {
                            valksCount = 0;
                        }
                        if (valksCount + manglersCount > specialZombiesForRound) {
                            valksCount = specialZombiesForRound - manglersCount;
                        }
                        specialEnemyCount_2 = valksCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        valkDelay = "Valk Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Valks"));
                        lastEditedSpecialEnemy = "Valks";
                    }
                    break;
                }
                case hashstr("Rev"): {
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spiderDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(spiderDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Spiders").x / 2 + 10);
                    if (ImGui::InputInt("# of Spiders", &genesisSpidersCount, 1, NULL)) {
                        if (genesisSpidersCount < 0) {
                            genesisSpidersCount = 0;
                        }
                        if (genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                            genesisSpidersCount = specialZombiesForRound - furysCount - keepersCount;
                        }
                        specialEnemyCount_1 = genesisSpidersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
                        lastEditedSpecialEnemy = "GenesisSpiders";
                    }
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(furyDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(furyDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Furys").x / 2 + 10);
                    if (ImGui::InputInt("# of Furys", &furysCount, 1, NULL)) {
                        if (furysCount < 0) {
                            furysCount = 0;
                        }
                        if (genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                            furysCount = specialZombiesForRound - genesisSpidersCount - keepersCount;
                        }
                        specialEnemyCount_2 = furysCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        furyDelay = "Fury Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Furys"));
                        lastEditedSpecialEnemy = "Furys";
                    }
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(keeperDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(keeperDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Keepers").x / 2 + 10);
                    if (ImGui::InputInt("# of Keepers", &keepersCount, 1, NULL)) {
                        if (keepersCount < 0) {
                            keepersCount = 0;
                        }
                        if (genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound) {
                            keepersCount = specialZombiesForRound - genesisSpidersCount - furysCount;
                        }
                        specialEnemyCount_3 = keepersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        keeperDelay = "Keeper Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Keepers"));
                        lastEditedSpecialEnemy = "Keepers";
                    }
                    break;
                }
                }
                std::string specialEnemiesRoundTimeText("Estimated Round Time:");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(specialEnemiesRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(specialEnemiesRoundTimeText.c_str());
                HelpMarker::Render("This doesn't account for reduced corpse delay by having more special enemies instead of normal zombies. Times will differ slightly from actual observed times, growing more inaccurate with higher rounds.");
                ImGui::SameLine();
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(specialEnemiesRoundTime.c_str()).x) / 2 + 17.5f);
                ImGui::Text(specialEnemiesRoundTime.c_str());
                ImGui::EndChild();
            }
            // Calc Data 6
            {
                ImGui::SameLine();
                ImGui::BeginChild("Calc Data 6", ImVec2(300, 265), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                HelpMarker::Render("Moon doesn't increase spawn rate with player count. On top of that, spawn rate increases 1 round every time you travel to the Earth/Moon.");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - Moon", &moonPlayerCount, 1, NULL)) {
                    if (moonPlayerCount < 1) {
                        moonPlayerCount = 1;
                    }
                    else if (moonPlayerCount > 4) {
                        moonPlayerCount = 4;
                    }
                    if (moonRoundSkip) {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    }
                    else {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                    }
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - Moon", &moonRound, 1, NULL)) {
                    if (moonRound < 1) {
                        moonRound = 1;
                    }
                    else if (moonRound > 255) {
                        moonRound = 255;
                    }
                    if (moonRoundSkip) {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    }
                    else {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                    }
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Earth Travels").x) / 2 + 10);
                ImGui::Text("Number Of Earth Travels");
                ImGui::SetNextItemWidth(120.0f);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 + 10);
                if (ImGui::InputInt("##Number of Earth travels", &moonEarthTravels, 1, NULL)) {
                    if (moonEarthTravels < 0) {
                        moonEarthTravels = 0;
                    }
                    if (moonEarthTravels > 27) {
                        moonEarthTravels = 27;
                    }
                    if (moonRoundSkip) {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    }
                    else {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                    }
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Round Skips").x) / 2 + 10);
                ImGui::Text("Number Of Round Skips");
                ImGui::Dummy(ImVec2(7.5f, 0));
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number of Round Skips", &moonRoundSkips, 1, NULL)) {
                    if (moonRoundSkips < 0) {
                        moonRoundSkips = 0;
                    }
                    if (moonRoundSkips > moonEarthTravels) {
                        moonRoundSkips = moonEarthTravels;
                    }
                    if (moonRoundSkip) {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    }
                    else {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Checkbox("This Round?", &moonRoundSkip)) {
                    if (moonRoundSkip) {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    }
                    else {
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                    }
                }
                std::string moonExpectedRoundTimeText("Expected Round Time For Round " + std::to_string(moonRound) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(moonExpectedRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(moonExpectedRoundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(moonExpectedRoundTime.c_str()).x) / 2 + 10);
                ImGui::Text(moonExpectedRoundTime.c_str());
                ImGui::EndChild();
            }
            // Calc Data 7
            {
                ImGui::SameLine();
                ImGui::BeginChild("Calc Data 7", ImVec2(300, 265), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                HelpMarker::Render("The GK Lockdown scales to the current round and player count, as well as increasing spawn rate each wave if not already maxed. The time presented will be when to nuke as long as you stay under 22 zombies at once. A new wave starts when the windows close.");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - GK Lockdown", &gkLockdownPlayerCount, 1, NULL)) {
                    if (gkLockdownPlayerCount < 1) {
                        gkLockdownPlayerCount = 1;
                    }
                    else if (gkLockdownPlayerCount > 4) {
                        gkLockdownPlayerCount = 4;
                    }
                    CalcLockdownTime(gkLockdownRound, gkLockdownPlayerCount);
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - GK Lockdown", &gkLockdownRound, 1, NULL)) {
                    if (gkLockdownRound < 1) {
                        gkLockdownRound = 1;
                    }
                    else if (gkLockdownRound > 255) {
                        gkLockdownRound = 255;
                    }
                    CalcLockdownTime(gkLockdownRound, gkLockdownPlayerCount);
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 1: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 1: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_1.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_1.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 2: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 2: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_2.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_2.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 3: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 3: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_3.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_3.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 4: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 4: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_4.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_4.c_str());
                ImGui::EndChild();
            }
            // Calc Data 8
            {
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 310, ImGui::GetCursorPosY() - 135));
                ImGui::BeginChild("Calc Data 8", ImVec2(300, 130), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                HelpMarker::Render("SOE has a constant spawn rate for rounds 1-4 regardless of player count.");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - SOE", &soePlayerCount, 1, NULL)) {
                    if (soePlayerCount < 1) {
                        soePlayerCount = 1;
                    }
                    else if (soePlayerCount > 4) {
                        soePlayerCount = 4;
                    }
                    soeExpectedRoundTime = SpecialRoundTime(soeRound, soePlayerCount, corpseDelay[soePlayerCount - 1][soeRound - 1], true);
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - SOE", &soeRound, 1, NULL)) {
                    if (soeRound < 1) {
                        soeRound = 1;
                    }
                    else if (soeRound > 4) {
                        soeRound = 4;
                    }
                    soeExpectedRoundTime = SpecialRoundTime(soeRound, soePlayerCount, corpseDelay[soePlayerCount - 1][soeRound - 1], true);
                }
                std::string soeExpectedRoundTimeText("Expected Round Time For Round " + std::to_string(soeRound) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(soeExpectedRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(soeExpectedRoundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(soeExpectedRoundTime.c_str()).x) / 2 + 10);
                ImGui::Text(soeExpectedRoundTime.c_str());
                ImGui::EndChild();
            }
            // Calc Data 9
            {
                ImGui::SameLine();
                ImGui::BeginChild("Calc Data 9", ImVec2(300, 130), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Zombies/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Zombies/Current Round");
                HelpMarker::Render("If you know the amount of zombies left on a round, and you want to figure out how much longer the round is, you can input the data here to get that time. The main use is finding how long to wait after 1st flag on SOE before nuking. Does not account for corpse delay.");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Zombies - Player Calc", &customZombiesLeft, 1, NULL)) {
                    if (customZombiesLeft < 1) {
                        customZombiesLeft = 1;
                    }
                    else if (customZombiesLeft > GetZombieCountForRound(customZombiesLeftRound, customZombiesLeftPlayerCount)) {
                        customZombiesLeft = GetZombieCountForRound(customZombiesLeftRound, customZombiesLeftPlayerCount);
                    }
                    customCalcExpectedRoundTime = CustomRoundTime(customZombiesLeftRound, customZombiesLeftPlayerCount, customZombiesLeft);
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - Player Calc", &customZombiesLeftRound, 1, NULL)) {
                    if (customZombiesLeftRound < 1) {
                        customZombiesLeftRound = 1;
                    }
                    else if (customZombiesLeftRound > 255) {
                        customZombiesLeftRound = 255;
                    }
                    customCalcExpectedRoundTime = CustomRoundTime(customZombiesLeftRound, customZombiesLeftPlayerCount, customZombiesLeft);
                }
                ImGui::Dummy(ImVec2(15, 0));
                ImGui::SameLine();
                ImGui::Text("Number Of Players");
                ImGui::Dummy(ImVec2(15, 0));
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - Player Calc", &customZombiesLeftPlayerCount, 1, NULL)) {
                    if (customZombiesLeftPlayerCount < 0) {
                        customZombiesLeftPlayerCount = 0;
                    }
                    else if (customZombiesLeftPlayerCount > 4) {
                        customZombiesLeftPlayerCount = 4;
                    }
                    customCalcExpectedRoundTime = CustomRoundTime(customZombiesLeftRound, customZombiesLeftPlayerCount, customZombiesLeft);
                }
                ImGui::SameLine();
                std::string calcTimeText("Time: " + customCalcExpectedRoundTime);
                ImGui::Text(calcTimeText.c_str());
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void CodeGuidesFunc()
{
    if (ImGui::BeginTabBar("Code Guide Tabs")) {
        if (ImGui::BeginTabItem("SOE Code")) {
            ImGui::SetNextItemWidth(130.0f);
            if (ImGui::BeginCombo("Code 1", soeCodeCombo[codeIndex_1].c_str(), ImGuiComboFlags_HeightRegular)) {
                for (int i = 0; i < soeCodeCombo.size(); i++) {
                    const bool is_selected = codeIndex_1 == i;
                    if (ImGui::Selectable(soeCodeCombo[i].c_str(), is_selected)) {
                        codeIndex_1 = i;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SetNextItemWidth(130.0f);
            if (ImGui::BeginCombo("Code 2", soeCodeCombo[codeIndex_2].c_str(), ImGuiComboFlags_HeightRegular)) {
                for (int i = 0; i < soeCodeCombo.size(); i++) {
                    const bool is_selected = codeIndex_2 == i;
                    if (ImGui::Selectable(soeCodeCombo[i].c_str(), is_selected)) {
                        codeIndex_2 = i;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Image(soeCodeImgList[codeIndex_1]->GetDescriptorSet(), ImVec2(490, 490));
            ImGui::SameLine();
            ImGui::Image(soeCodeImgList[codeIndex_2]->GetDescriptorSet(), ImVec2(490, 490));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ice Code")) {
            if (showSolution) {
                if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
                    showSolution = false;
                }
                ImGui::Image(iceCodeImgList["solution"]->GetDescriptorSet(), ImVec2(min(1280, ImGui::GetContentRegionAvail().x), min(720, ImGui::GetContentRegionAvail().y - 30)));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_TRANSPARENT); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_BLUE_WEAK);
                bool skip = false;
                // Images group 1
                {
                    ImGui::BeginGroup();
                    if (gameChecked[0]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[0].symbolImage->GetFilename().c_str(), randomIceCodePairs[0].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[0]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[0].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 0);
                        }
                        else {
                            ProgressGame(false, 0);
                        }
                    }
                    if (!skip && gameChecked[0]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    ImGui::SameLine();
                    if (gameChecked[1]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[1].symbolImage->GetFilename().c_str(), randomIceCodePairs[1].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[1]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[1].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 1);
                        }
                        else {
                            ProgressGame(false, 1);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[1]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[2]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[2].symbolImage->GetFilename().c_str(), randomIceCodePairs[2].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[2]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[2].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 2);
                        }
                        else {
                            ProgressGame(false, 2);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[2]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[3]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[3].symbolImage->GetFilename().c_str(), randomIceCodePairs[3].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[3]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[3].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 3);
                        }
                        else {
                            ProgressGame(false, 3);
                        }
                    }
                    if (!skip && gameChecked[3]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    ImGui::EndGroup();
                }
                ImGui::SameLine();
                ImGui::BeginGroup();
                if (Button::RenderSingle("Show Solution Key", ImVec2(150, 25))) {
                    showSolution = true;
                }
                ImGui::Text(gameTime.c_str());
                ImGui::Text(accuracy.c_str());
                ImGui::EndGroup();
                // Images group 2
                {
                    ImGui::BeginGroup();
                    if (gameChecked[4]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[4].symbolImage->GetFilename().c_str(), randomIceCodePairs[4].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[4]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[4].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 4);
                        }
                        else {
                            ProgressGame(false, 4);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[4]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[5]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[5].symbolImage->GetFilename().c_str(), randomIceCodePairs[5].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[5]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[5].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 5);
                        }
                        else {
                            ProgressGame(false, 5);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[5]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[6]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[6].symbolImage->GetFilename().c_str(), randomIceCodePairs[6].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[6]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[6].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 6);
                        }
                        else {
                            ProgressGame(false, 6);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[6]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[7]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[7].symbolImage->GetFilename().c_str(), randomIceCodePairs[7].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[7]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[7].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 7);
                        }
                        else {
                            ProgressGame(false, 7);
                        }
                    }
                    if (!skip && gameChecked[7]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    ImGui::EndGroup();
                }
                ImGui::SameLine();
                ImGui::Dummy(ImVec2(15, 0));
                ImGui::SameLine();
                ImGui::Image(iceCodePairs[randomList[gameProgress]].digitImage->GetDescriptorSet(), ImVec2(170, 152));
                // Images group 3
                {
                    ImGui::BeginGroup();
                    if (gameChecked[8]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[7].symbolImage->GetFilename().c_str(), randomIceCodePairs[8].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[8]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[8].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 8);
                        }
                        else {
                            ProgressGame(false, 8);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[8]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[9]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[9].symbolImage->GetFilename().c_str(), randomIceCodePairs[9].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[9]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[9].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 9);
                        }
                        else {
                            ProgressGame(false, 9);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[9]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[10]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[10].symbolImage->GetFilename().c_str(), randomIceCodePairs[10].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[10]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[10].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 10);
                        }
                        else {
                            ProgressGame(false, 10);
                        }
                    }
                    ImGui::SameLine();
                    if (!skip && gameChecked[10]) {
                        ImGui::PopStyleColor(3);
                    }
                    skip = false;
                    if (gameChecked[11]) {
                        ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED);
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[11].symbolImage->GetFilename().c_str(), randomIceCodePairs[11].symbolImage->GetDescriptorSet(), ImVec2(170, 152))) {
                        if (!gameChecked[11]) {
                            skip = true;
                        }
                        if (randomIceCodePairs[11].symbolImage->GetFilename() == iceCodePairs[randomList[gameProgress]].symbolImage->GetFilename()) {
                            ProgressGame(true, 11);
                        }
                        else {
                            ProgressGame(false, 11);
                        }
                    }
                    if (!skip && gameChecked[11]) {
                        ImGui::PopStyleColor(3);
                    }
                    ImGui::EndGroup();
                }
                ImGui::PopStyleColor(2);

                // Timer operations
                if (gameStarted) {
                    gameTime = "Time: " + ParseTimeFromMilli(gameTimer.Elapsed());
                }
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void GKValveSolverFunc()
{
    if (showEvaluation) {
        if (Button::RenderSingle(ICON_FA_ARROW_LEFT, ImVec2(50, 25))) {
            showEvaluation = false;
        }

        ImGui::Dummy(ImVec2(15, 0));
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 450 - ImGui::CalcTextSize("Possible Solutions").x / 2);
        ImGui::Text("Possible Solutions");

        if (ImGui::BeginTable("PossibleValves", 6, ImGuiTableFlags_NoBordersInBody, ImVec2(910, 50))) {
            ImGui::TableNextRow();
            int used = 1;
            ImGui::TableSetColumnIndex(0);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREY);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Green, P").x / 2);
            ImGui::Text("Green, P");
            for (int column = 0; column < 6; column++) {
                if (valveLocations[column] == passwordLocation) {
                    continue;
                }
                ImGui::TableSetColumnIndex(used);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREY);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveLocations[column].c_str()).x / 2);
                ImGui::Text(valveLocations[column].c_str());
                used++;
            }
            for (int row = 0; row < 6; row++) {
                if (possibleValves_1.find(valveLocations[row]) == possibleValves_1.end()) {
                    continue;
                }
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREY);
                std::string key = valveLocationsAbbr[valveLocations[row]] + ", " + valveLocationsAbbr[passwordLocation] + " - 1";
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(key.c_str()).x / 2);
                ImGui::Text(key.c_str());
                used = 1;
                for (int column = 0; column < 6; column++) {
                    if (valveLocations[column] == passwordLocation) {
                        continue;
                    }
                    ImU32 color;
                    ImGui::TableSetColumnIndex(used);
                    if (possibleValves_1[valveLocations[row]][column] == "P") {
                        color = COLOR_PINK;
                    }
                    else if (valveDirections[column][static_cast<size_t>(std::stoi(possibleValves_1[valveLocations[row]][column])) - 1]) {
                        color = COLOR_GREEN;
                    }
                    else {
                        color = COLOR_RED;
                    }
                    Button::RenderFake(possibleValves_1[valveLocations[row]][column].c_str(), ImVec2(145, 25), color);
                    used++;
                }
                if (possibleValves_2.find(valveLocations[row]) == possibleValves_2.end()) {
                    continue;
                }
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREY);
                key = valveLocationsAbbr[valveLocations[row]] + ", " + valveLocationsAbbr[passwordLocation] + " - 2";
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(key.c_str()).x / 2);
                ImGui::Text(key.c_str());
                used = 1;
                for (int column = 0; column < 6; column++) {
                    if (valveLocations[column] == passwordLocation) {
                        continue;
                    }
                    ImU32 color;
                    ImGui::TableSetColumnIndex(used);
                    if (possibleValves_2[valveLocations[row]][column] == "P") {
                        color = COLOR_PINK;
                    }
                    else if (valveDirections[column][static_cast<size_t>(std::stoi(possibleValves_2[valveLocations[row]][column])) - 1]) {
                        color = COLOR_GREEN;
                    }
                    else {
                        color = COLOR_RED;
                    }
                    Button::RenderFake(possibleValves_2[valveLocations[row]][column].c_str(), ImVec2(145, 25), color);
                    used++;
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }
    else {
        ImGui::Dummy(ImVec2(30, 0));
        ImGui::SameLine();
        for (int i = 0; i < 6; i++) {
            ImGui::BeginGroup();
            ImGui::AlignTextToFramePadding();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (60 - ImGui::CalcTextSize(valveLocations[i].c_str()).x / 2));
            ImGui::Text(valveLocations[i].c_str());
            ImGui::SameLine();
            std::string checkedGreen = "Not Green##";
            checkedGreen += valveLocations[i];
            bool disableCheck = false;
            if (greenLocation == valveLocations[i] || passwordLocation == valveLocations[i]) {
                ImGui::BeginDisabled();
                disableCheck = true;
                checkedGreenArray[i] = false;
            }
            if (ImGui::Checkbox(checkedGreen.c_str(), &checkedGreenArray[i])) {
                valveGreen[i] = false;
                CalcValveProbabilities();
                CalcRemainingGreen();
            }
            if (disableCheck && (greenLocation == valveLocations[i] || passwordLocation == valveLocations[i])) {
                ImGui::EndDisabled();
            }
            if (checkedGreenArray[i]) {
                ImGui::BeginDisabled();
            }
            ImU32 color = COLOR_RED;
            if (valveGreen[i]) {
                color = COLOR_GREEN;
            }
            std::string green = "Green##";
            green += valveLocations[i];
            if (Button::RenderSingle(green, ImVec2(120, 25), false, color)) {
                if (valveGreen[i]) {
                    if (!noGreenChoice) {
                        valveGreen = { false, false, false, false, false, false };
                        greenChosen = false;
                        greenLocation = "";
                        valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                        valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                        CalcValveProbabilities();
                    }
                }
                else {
                    valveGreen = { false, false, false, false, false, false };
                    valveGreen[i] = true;
                    greenLocation = valveLocations[i];
                    if (valvePassword[i]) {
                        valvePassword[i] = false;
                        passwordChosen = false;
                        valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                        valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                    }
                    if (passwordChosen) {
                        valveSolutionsText_1 = valveSolutions_1[passwordLocation][greenLocation];
                        valveSolutionsText_2 = valveSolutions_2[passwordLocation][greenLocation];
                    }
                    greenChosen = true;
                    CalcValveProbabilities();
                }
            }
            ImGui::SameLine();
            if (checkedGreenArray[i]) {
                ImGui::EndDisabled();
            }
            if (valvePassword[i]) {
                color = COLOR_GREEN;
            }
            else {
                color = COLOR_RED;
            }
            std::string password = "Password##";
            password += valveLocations[i];
            if (Button::RenderSingle(password, ImVec2(120, 25), false, color)) {
                if (valvePassword[i]) {
                    valvePassword = { false, false, false, false, false, false };
                    passwordChosen = false;
                    passwordLocation = "";
                    valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                    valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                    CalcExcludedValves();
                    CalcValveProbabilities();
                }
                else {
                    valvePassword = { false, false, false, false, false, false };
                    valvePassword[i] = true;
                    passwordLocation = valveLocations[i];
                    if (valveGreen[i]) {
                        valveGreen[i] = false;
                        greenChosen = false;
                        valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                        valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                    }
                    if (greenChosen) {
                        valveSolutionsText_1 = valveSolutions_1[passwordLocation][greenLocation];
                        valveSolutionsText_2 = valveSolutions_2[passwordLocation][greenLocation];
                    }
                    passwordChosen = true;
                    CalcExcludedValves();
                    CalcValveProbabilities();
                }
            }
            ImGui::Image(valveSolverImgList[0]->GetDescriptorSet(), ImVec2(250, 78));
            if (valvePassword[i]) {
                ImGui::BeginDisabled();
                valveDirections[i] = { false, false, false };
            }
            if (valveDirections[i][0]) {
                color = COLOR_GREEN;
            }
            else {
                color = COLOR_RED;
            }
            std::string num_1 = "1##";
            num_1 += valveLocations[i];
            float startPos_1 = ImGui::GetCursorPosX();
            if (Button::RenderSingle(num_1, ImVec2(78, 25), false, color)) {
                if (valveDirections[i][0]) {
                    valveDirections[i] = { false, false, false };
                }
                else {
                    valveDirections[i] = { true, false, false };
                }
                CalcExcludedValves();
                CalcValveProbabilities();
            } ImGui::SameLine();
            if (valveDirections[i][1]) {
                color = COLOR_GREEN;
            }
            else {
                color = COLOR_RED;
            }
            std::string num_2 = "2##";
            num_2 += valveLocations[i];
            ImGui::SameLine();
            float startPos_2 = ImGui::GetCursorPosX();
            if (Button::RenderSingle(num_2, ImVec2(78, 25), false, color)) {
                if (valveDirections[i][1]) {
                    valveDirections[i] = { false, false, false };
                }
                else {
                    valveDirections[i] = { false, true, false };
                }
                CalcExcludedValves();
                CalcValveProbabilities();
            }
            ImGui::SameLine();
            if (valveDirections[i][2]) {
                color = COLOR_GREEN;
            }
            else {
                color = COLOR_RED;
            }
            std::string num_3 = "3##";
            num_3 += valveLocations[i];
            float startPos_3 = ImGui::GetCursorPosX();
            if (Button::RenderSingle(num_3, ImVec2(78, 25), false, color)) {
                if (valveDirections[i][2]) {
                    valveDirections[i] = { false, false, false };
                }
                else {
                    valveDirections[i] = { false, false, true };
                }
                CalcExcludedValves();
                CalcValveProbabilities();
            }
            if (valvePassword[i]) {
                ImGui::EndDisabled();
            }

            // lmao this is scuffed af
            {
                int addPos = 10;
                if (valvePassword[i]) {
                    addPos = 0;
                }
                ImGui::SetCursorPosX(startPos_1 + 39 - ImGui::CalcTextSize(valveDirectionOdds_1[i][0].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_1[i][0].c_str());
                ImGui::SameLine();
                addPos = 10;
                if (valvePassword[i]) {
                    addPos = 0;
                }
                ImGui::SetCursorPosX(startPos_2 + 39 - ImGui::CalcTextSize(valveDirectionOdds_1[i][1].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_1[i][1].c_str());
                ImGui::SameLine();
                addPos = 10;
                if (valvePassword[i]) {
                    addPos = 0;
                }
                ImGui::SetCursorPosX(startPos_3 + 39 - ImGui::CalcTextSize(valveDirectionOdds_1[i][2].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_1[i][2].c_str());

                addPos = 10;
                if (valvePassword[i]) {
                    addPos = 0;
                }
                ImGui::SetCursorPosX(startPos_1 + 39 - ImGui::CalcTextSize(valveDirectionOdds_2[i][0].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_2[i][0].c_str());
                ImGui::SameLine();
                addPos = 10;
                if (valvePassword[i]) {
                    addPos = 0;
                }
                ImGui::SetCursorPosX(startPos_2 + 39 - ImGui::CalcTextSize(valveDirectionOdds_2[i][1].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_2[i][1].c_str());
                ImGui::SameLine();
                addPos = 10;
                    if (valvePassword[i]) {
                }
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_3 + 39 - ImGui::CalcTextSize(valveDirectionOdds_2[i][2].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_2[i][2].c_str());
            }

            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(30, 0));
            if (i != 2 && i != 5) {
                ImGui::SameLine();
            }
            else if (i == 2) {
                ImGui::Dummy(ImVec2(0, 30));
                ImGui::Dummy(ImVec2(30, 0));
                ImGui::SameLine();
            }
        }
        ImGui::Dummy(ImVec2(15, 0));
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(0, 15));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 450 - ImGui::CalcTextSize("Solutions").x / 2);
        ImGui::Text("Solutions");
        ImGui::SameLine();
        if (!passwordChosen || greenChosen) {
            ImGui::BeginDisabled();
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.5f);
        if (Button::RenderSingle("Evaluate Valves", ImVec2(150, 25))) {
            showEvaluation = true;
        }
        if (!passwordChosen || greenChosen) {
            ImGui::EndDisabled();
        }
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.5f);
        if (Button::RenderSingle("Reset Valves", ImVec2(150, 25))) {
            greenChosen = false;
            passwordChosen = false;
            noGreenChoice = false;
            valveComboSet = false;
            showEvaluation = false;
            greenLocation = "";
            passwordLocation = "";
            valveGreen = { false, false, false, false, false, false };
            valvePassword = { false, false, false, false, false, false };
            valveDirections = { { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false } };
            CalcValveProbabilities();
            CalcRemainingGreen();
            CalcExcludedValves();
        }
        if (ImGui::BeginTable("ValveSolutions", 6, ImGuiTableFlags_Borders, ImVec2(900, 50))) {
            for (int row = 0; row < 3; row++) {
                ImGui::TableNextRow();
                for (int column = 0; column < 6; column++) {
                    ImGui::TableSetColumnIndex(column);
                    if (!row) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREY);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveLocations[column].c_str()).x / 2);
                        ImGui::Text(valveLocations[column].c_str());
                        continue;
                    }
                    if (!greenChosen || !passwordChosen) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 90.0f));
                    }
                    else {
                        if (row == 1) {
                            if (valveSolutionsText_1[column] == "P") {
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_PINK);
                            }
                            else if (valveDirections[column][static_cast<size_t>(std::stoi(valveSolutionsText_1[column])) - 1]) {
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREEN);
                            }
                            else {
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_RED);
                            }
                        }
                        else {
                            if (valveSolutionsText_2[column] == "P") {
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_PINK);
                            }
                            else if (valveDirections[column][static_cast<size_t>(std::stoi(valveSolutionsText_2[column])) - 1]) {
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_GREEN);
                            }
                            else {
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, COLOR_RED);
                            }
                        }
                    }
                    ImGui::TableSetColumnIndex(column);
                    if (row == 1) {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveSolutionsText_1[column].c_str()).x / 2);
                        ImGui::Text(valveSolutionsText_1[column].c_str());
                    }
                    else {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveSolutionsText_2[column].c_str()).x / 2);
                        ImGui::Text(valveSolutionsText_2[column].c_str());
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }
}