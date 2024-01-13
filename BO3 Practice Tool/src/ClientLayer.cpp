#include "ClientLayer.h"

#include <Windows.h>
#include <memory>
#include <vector>
#include <thread>
#include <filesystem>
#include <regex>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "Walnut/Input/Input.h"
#include "Walnut/ImGui/ImGuiTheme.h"
#include "Walnut/Core/Log.h"
#include "imgui.h"
#include "PracticeTool/StructDefs/BGBData.h"
#include "PracticeTool/StructDefs/WeaponData.h"
#include "PracticeTool/StructDefs/AutosplitData.h"
#include "PracticeTool/UI/ImageData.h"
#include "PracticeTool/UI/WindowLogic.h"
#include "PracticeTool/UI/LogUI.h"
#include "PracticeTool/UI/Options.h"
#include "PracticeTool/UI/Notifications.h"
#include "PracticeTool/UI/KeyboardIcons.h"
#include "PracticeTool/UI/WindowBody.h"
#include "PracticeTool/Core/Log.h"
#include "PracticeTool/Core/Memory.h"
#include "PracticeTool/Core/Interop.h"
#include "PracticeTool/Core/Registry.h"
#include "PracticeTool/Core/Hash.h"

#include "ImGui/ImGuiUtils.h"

#include "Embed/Icons.embed"

std::unique_ptr<Walnut::Image> g_DiscordIconImage;

ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

#define EXCLUSION_PATH "SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Paths"
#define STEAM_PATH "SOFTWARE\\WOW6432Node\\Valve\\SteamService"

bool PowershellInstalled()
{
	char buffer[256];
	GetSystemDirectoryA(buffer, sizeof(buffer));
	std::string powershell = std::format("{}\\WindowsPowerShell\\v1.0\\powershell.exe", buffer);
	return std::filesystem::exists(powershell);
}

bool FindSteamData()
{
	if (Registry::GetStringRegKey(HKEY_LOCAL_MACHINE, STEAM_PATH, "installpath_default", BO3PracticeTool::steamDirectory) != ERROR_SUCCESS) {
		return false;
	}

	std::string libraries = std::format("{}\\config\\libraryfolders.vdf", BO3PracticeTool::steamDirectory);
	if (!std::filesystem::exists(libraries)) {
		return false;
	}

	std::ifstream file(libraries);
	if (!file.is_open()) {
		return false;
	}

	std::vector<std::string> paths;
	std::vector<std::string> tokens;
	std::string line;
	std::string token;
	while (std::getline(file, line)) {
		if (line.find("\"path\"") != line.npos) {
			size_t pos = line.find("path");
			if (pos != std::string::npos) {
				line.erase(pos, std::string("path").length());
			}
			size_t backslash;
			while ((backslash = line.find("\\\\")) != std::string::npos) {
				line.replace(backslash, std::string("\\\\").length(), "\\");
			}

			std::stringstream ss(line);
			while (std::getline(ss, token, '\"')) {
				tokens.emplace_back(token);
			}
		}
	}

	for (const std::string& token : tokens) {
		if (token.empty() || std::all_of(token.begin(), token.end(), isspace)) {
			continue;
		}
		paths.emplace_back(token);
	}

	for (const std::string& path : paths) {
		if (!std::filesystem::is_directory(path)) {
			LOG_ERROR("Attempted to iterate over invalid directory: {}", path);
			continue;
		}
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path)) {
			if (std::filesystem::is_regular_file(entry) && entry.path().filename() == "BlackOps3.exe") {
				if (Hash::GetFileHash(entry.path()) != "f734b3d51bf83d4c5b79ffeaf013ffe3") {
					return false;
				}
				BO3PracticeTool::bo3Directory = entry.path().parent_path().string();
				BO3PracticeTool::practiceToolDirectory = BO3PracticeTool::bo3Directory + "\\Practice Tool";
				LOG_DEBUG("Steam Directory: {}", BO3PracticeTool::steamDirectory);
				LOG_DEBUG("BO3 Directory: {}", BO3PracticeTool::bo3Directory);
				LOG_DEBUG("Practice Tool Directory: {}", BO3PracticeTool::practiceToolDirectory);
				if (!std::filesystem::exists(BO3PracticeTool::practiceToolDirectory)) {
					std::filesystem::create_directories(BO3PracticeTool::practiceToolDirectory);
				}
				return true;
			}
		}
	}

	return false;
}

void FindTempData()
{
	char buffer[256];
	GetTempPathA(sizeof(buffer), buffer);
	BO3PracticeTool::tempDirectory = std::format("{}BO3PracticeTool", buffer);
	if (!std::filesystem::exists(BO3PracticeTool::tempDirectory)) {
		std::filesystem::create_directories(BO3PracticeTool::tempDirectory);
	}
}

void ClientLayer::OnAttach()
{
	BO3PracticeTool::Log::Init();

	FindTempData();
	bool steamData = FindSteamData();
	if (PowershellInstalled) {
		char buffer[256];
		GetCurrentDirectoryA(sizeof(buffer), buffer);
		std::string script = std::format("\"{}\\Scripts\\Exclusions.ps1\"", buffer);
		std::string args = std::format("-noprofile -noexit -file {} -elevated -ExclusionPath {}", script, BO3PracticeTool::tempDirectory);
		ShellExecuteA(NULL, "runas", "powershell.exe", args.c_str(), NULL, SW_HIDE);
		if (steamData) {
			args = std::format("-noprofile -noexit -file {} -elevated -ExclusionPath {}", script, std::format("\"{}\\Practice Tool\"", BO3PracticeTool::bo3Directory));
			ShellExecuteA(NULL, "runas", "powershell.exe", args.c_str(), NULL, SW_HIDE);
		}
	}

	uint32_t w, h;
	void* data = Walnut::Image::Decode(g_DiscordIcon.data(), g_DiscordIcon.size(), w, h);
	g_DiscordIconImage = std::make_unique<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data);

	BO3PracticeTool::OnInit();

	BO3PracticeTool::LoadKeyboardIcons();
	BO3PracticeTool::ManageImageLifetimes("Gobblegum Presets");
	BO3PracticeTool::BGBPreset::LoadBGBPresets();
	BO3PracticeTool::WeaponPreset::LoadWeaponPresets();
	BO3PracticeTool::AutosplitPreset::LoadAutosplitPresets();
	BO3PracticeTool::LoadWeaponSelections();
	std::thread(Memory::ProcessLifetime).detach();
	std::thread(BO3PracticeTool::ReadGame).detach();

	ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImColor(70, 70, 70, 255);
	ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImColor(30, 30, 30, 255);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 255);
	ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	ImGui::GetStyle().Colors[ImGuiCol_TableBorderLight] = ImColor(70, 70, 70, 255);
}

void ClientLayer::OnDetach()
{
	BO3PracticeTool::Log::Shutdown();
	BO3PracticeTool::ManageImageLifetimes("");
	BO3PracticeTool::UnloadKeyboardIcons();
	BO3PracticeTool::OnShutdown();
	g_DiscordIconImage.reset();

	BO3PracticeTool::BGBPreset::WritePresetToGame(true);
	BO3PracticeTool::AutosplitPreset::WritePresetToGame(true);
	BO3PracticeTool::WeaponPreset::WritePresetToGame(true);
}

void ClientLayer::OnUIRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, 58));
	ImGui::SetNextWindowSize(viewport->Size - ImVec2(0, 58));

	if (!ImGui::Begin("##Main Window", NULL, windowFlags)) {
		return ImGui::End();;
	}

	BO3PracticeTool::InfoNotification::RenderInfoNotis();
	BO3PracticeTool::ImGuiLogs::RenderLogWindow();
	BO3PracticeTool::RenderOptionsWindow();
	BO3PracticeTool::UserTabSelection();
	BO3PracticeTool::RenderTabWindow();

	ImGui::End();
}

void ClientLayer::MenuBar()
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	Walnut::UI::ShiftCursorY(2);
	ImVec2 discordMin = ImGui::GetCursorScreenPos();
	ImVec2 discordImage = discordMin + ImVec2(40, 40);
	ImVec2 discordText = ImVec2(discordImage.x + 2.5f, discordMin.y + 2.5f);
	ImVec2 discordMax = discordMin + ImVec2(40, 40) + ImVec2(ImGui::CalcTextSize("Discord").x, 0);
	ImVec2 discordBorderMin = discordMin - ImVec2(10, 5);
	ImVec2 discordBorderMax = discordMax + ImVec2(10, 5);
	ImRect discordRect = ImRect(discordBorderMin, discordBorderMax);

	Walnut::UI::ShiftCursorX(-5);
	if (ImGui::Button("##Join Discord", discordRect.Max - discordRect.Min - ImVec2(5, 5))) {
		std::thread([]() { ShellExecuteA(NULL, "open", "https://discord.gg/SnaSjkPEmV", NULL, NULL, SW_SHOWNORMAL); }).detach();
	}

	drawList->AddImage(g_DiscordIconImage->GetDescriptorSet(), discordMin + ImVec2(0, 4), discordImage + ImVec2(0, 4));
	drawList->AddText(discordText, Walnut::UI::Colors::Theme::text, "  Join\nDiscord");

	if (ImGui::Button("About")) {
		ImGui::OpenOverridePopup("About");
	}

	ImGui::SetNextWindowSize(ImVec2(400, 175));
	if (ImGui::BeginOverridePopup("About", ImGuiWindowFlags_NoDocking)) {
		ImGui::CenterText("Github Repository");
		ImGui::TextURL("Github Repository", "https://github.com/Joshr520/BO3-Practice-Tool", 0, 0);
		ImGui::TextWrapped("A tool for the Steam version of Call of Duty: Black Ops III. Used to speedrun the Zombies mode.");
		ImGui::TextWrapped("Allows the user to manipulate rng for practicing. Also provides many resources for doing real runs.");
		if (Walnut::UI::ButtonCentered("Close")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndOverridePopup();
	}

	if (ImGui::Button("Open Logs")) {
		if (BO3PracticeTool::ImGuiLogs::GetShowLogs()) {
			ImGuiWindow* window = ImGui::FindWindowByName("Logs");
			ImGui::GetPlatformIO().Platform_ShowWindow(window->Viewport);
		}
		else {
			BO3PracticeTool::ImGuiLogs::SetShowLogs(true);
		}
	}

	if (ImGui::Button("Options")) {
		if (BO3PracticeTool::showOptions) {
			ImGuiWindow* window = ImGui::FindWindowByName("Options");
			ImGui::GetPlatformIO().Platform_ShowWindow(window->Viewport);
		}
		else {
			BO3PracticeTool::showOptions = true;
		}
	}
}