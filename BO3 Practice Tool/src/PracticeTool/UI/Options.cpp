#include "Options.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "Walnut/ImGui/ImGuiTheme.h"
#include "ImGui/ImGuiExtendedWidgets.h"
#include "PracticeTool/StructDefs/GameData.h"
#include "PracticeTool/Core/Memory.h"

#include <vector>
#include <string>

void SettingsWindow();
void DebugWindow();

ImGuiWindowFlags optionFlags = ImGuiWindowFlags_NoCollapse;

std::vector<std::string> tabNames = { "Settings" };
std::vector<std::string> debugTabNames = { "Settings", "Debug Setting" };
std::vector<std::function<void()>> tabWindows = { SettingsWindow };
std::vector<std::function<void()>> debugTabWindows = { SettingsWindow, DebugWindow };
size_t tabIndex = 0;
size_t debugTabIndex = 0;

namespace BO3PracticeTool
{
	void RenderOptionsWindow()
	{
		if (!showOptions) {
			return;
		}

		ImVec2 size = ImGui::GetWindowSize() * 0.75f;
		ImVec2 pos = ImGui::GetWindowPos() + (ImGui::GetWindowSize() * 0.25f) / 2.0f;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, Walnut::UI::Colors::ColorWithMultipliedValue(Walnut::UI::Colors::Theme::titlebar, 1.5f));

		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
		if (!ImGui::Begin("Options", &showOptions, optionFlags)) {
			ImGui::PopStyleColor();
			return ImGui::End();
		}

		if (ImGui::BeginChild("Options Tabs", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, 0))) {
#ifdef WL_DEBUG
			ImGui::Selection(debugTabNames, debugTabIndex);
#else
			ImGui::Selection(tabNames, tabIndex);
#endif
			ImGui::EndChild();
		}
		ImGui::SameLine();
#ifdef WL_DEBUG
		debugTabWindows[debugTabIndex]();
#else
		tabWindows[tabIndex]();
#endif

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

void SettingsWindow()
{

}

bool overrideMap = false;
size_t mapIndex = 0;
void DebugWindow()
{
	if (ImGui::Checkbox("Override Map", &overrideMap)) {
		if (overrideMap) {
			Memory::overrideMap = true;
			Memory::validMap = true;
			Memory::injected = true;
			Memory::mapName = BO3PracticeTool::t7MapsInternal[mapIndex];
		}
		else {
			Memory::overrideMap = false;
			Memory::validMap = false;
			Memory::injected = false;
		}
	}

	if (!overrideMap) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Map Override Combo", BO3PracticeTool::t7MapsInternal[mapIndex].c_str())) {
		if (ImGui::Selection(BO3PracticeTool::t7MapsInternal, mapIndex)) {
			Memory::mapName = BO3PracticeTool::t7MapsInternal[mapIndex];
		}
		ImGui::EndCombo();
	}
	if (!overrideMap) {
		ImGui::EndDisabled();
	}
}