#include "WindowLogic.h"
#include "WindowBody.h"
#include "ImGui/ImGuiUtils.h"
#include "Walnut/UI/UI.h"
#include "Walnut/ApplicationGUI.h"
#include "PracticeTool/UI/ImageData.h"
#include "PracticeTool/Core/Memory.h"

#include <vector>
#include <string>
#include <format>

struct TabSelection {
	std::string TabGroup;
	size_t TabItemIndex;
	std::vector<std::string> TabItems;
};

std::vector<TabSelection> userTabSelections = {
	{
		"Frontend Data", 0, { "Gobblegum Presets", "Weapon Presets", "Autosplit Presets", "Practice Patches" }
	},
	{
		"In Game Options", 0, { "Player Options", "Zombie Options" }
	},
	{
		"Resources", 0, { "Gum Tracker", "Zombie Calculator", "Code Guides", "GK Valve Solver" }
	}
};
size_t currentTabGroup = 0;

void BO3PracticeTool::UserTabSelection()
{
	for (size_t i = 0; i < userTabSelections.size(); i++) {
		const TabSelection& group = userTabSelections[i];
		if (ImGui::BeginTabBar("User Tab Selection")) {
			if (ImGui::BeginTabItem(group.TabGroup.c_str())) {
				if (currentTabGroup != i) {
					currentTabGroup = static_cast<int>(i);
					ManageImageLifetimes(group.TabItems[group.TabItemIndex]);
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
}

void BO3PracticeTool::RenderTabWindow()
{
	TabSelection& group = userTabSelections[currentTabGroup];
	ImGui::SetNextItemWidth(250);
	if (ImGui::BeginCombo("##Tab Item Selection", group.TabItems[group.TabItemIndex].c_str())) {
		if (ImGui::Selection(group.TabItems, group.TabItemIndex)) {
			ManageImageLifetimes(group.TabItems[group.TabItemIndex]);
		}
		ImGui::EndCombo();
	}

	BO3PracticeTool::RenderWindowBody(currentTabGroup, group.TabItemIndex);

	if (Memory::mapChanged || Memory::mapRestarted) {
		BO3PracticeTool::ResetVariables();
		Memory::mapChanged = false;
		Memory::mapRestarted = false;
	}
}