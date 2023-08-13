#include "ClientLayer.h"

#include <Windows.h>
#include <memory>
#include <vector>
#include <thread>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "Walnut/Input/Input.h"
#include "Walnut/ImGui/ImGuiTheme.h"
#include "Walnut/Core/Log.h"
#include "imgui.h"
#include "PracticeTool/UI/ImageData.h"
#include "PracticeTool/UI/WindowLogic.h"
#include "PracticeTool/UI/LogUI.h"
#include "PracticeTool/UI/Notifications.h"
#include "PracticeTool/UI/KeyboardIcons.h"
#include "PracticeTool/Core/Log.h"

#include "ImGui/ImGuiUtils.h"

#include "Embed/Icons.embed"

std::unique_ptr<Walnut::Image> g_DiscordIconImage;

bool docked = false;

ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

void ClientLayer::OnAttach()
{
	BO3PracticeTool::Log::Init();

	uint32_t w, h;
	void* data = Walnut::Image::Decode(g_DiscordIcon.data(), g_DiscordIcon.size(), w, h);
	g_DiscordIconImage = std::make_unique<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data);

	BO3PracticeTool::LoadKeyboardIcons();
	BO3PracticeTool::ManageImageLifetimes("Gobblegum Presets");
	BO3PracticeTool::LoadBGBPresets();
	BO3PracticeTool::LoadWeaponPresets();

	ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImColor(70, 70, 70, 255);
	ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImColor(30, 30, 30, 255);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 255);
	ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
}

void ClientLayer::OnDetach()
{
	BO3PracticeTool::Log::Shutdown();
	BO3PracticeTool::ManageImageLifetimes("");
	BO3PracticeTool::UnloadKeyboardIcons();
	g_DiscordIconImage.reset();
}

void ClientLayer::OnUIRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, 58));
	ImGui::SetNextWindowSize(viewport->Size - ImVec2(0, 58));

	if (!ImGui::Begin("##Main Window", NULL, windowFlags)) {
		return ImGui::End();;
	}

	BO3PracticeTool::RunPopups();
	BO3PracticeTool::InfoNotification::RenderInfoNotis();
	BO3PracticeTool::ImGuiLogs::RenderLogWindow();
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
		ImGui::PopupWrapper::PrepPopup(ImGui::PopupStates::About);
	}

	if (ImGui::Button("Open Logs")) {
		if (BO3PracticeTool::ImGuiLogs::GetShowLogs()) {
			ImGui::SetWindowFocus("Logs");
			if (ImGui::GetPlatformIO().Viewports.size() > 1) {
				for (ImGuiViewportP* viewport : ImGui::GetCurrentContext()->Viewports) {
					if (viewport->Window) {
						if (!std::strcmp(viewport->Window->Name, "Logs")) {
							ImGui::GetPlatformIO().Platform_ShowWindow(viewport);
						}
					}
				}
			}
		}
		else {
			BO3PracticeTool::ImGuiLogs::SetShowLogs(true);
		}
	}
}