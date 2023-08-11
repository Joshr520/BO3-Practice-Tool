#include "WindowLogic.h"
#include "WindowBody.h"
#include "ImGui/ImGuiUtils.h"
#include "Walnut/UI/UI.h"
#include "Walnut/ApplicationGUI.h"
#include "PracticeTool/UI/ImageData.h"

#include <vector>
#include <string>
#include <format>

struct TabSelection {
	std::string TabGroup;
	int TabItemIndex;
	std::vector<std::string> TabItems;
};

std::vector<TabSelection> userTabSelections = {
	{
		"Frontend Data", 0, { "Gobblegum Presets", "Weapon Presets", "Autosplit Presets", "Practice Patches" }
	},
	{
		"In Game Options", 0, { "Player Options", "Zombie Options", "Round Options", "Powerup Options", "Egg Step Options", "Craftable Options", "Blocker Options", "Map Options" }
	},
	{
		"Resources", 0, { "Gum Tracker", "Zombie Calculator", "Code Guides", "GK Valve Solver" }
	}
};
int currentTabGroup = 0;

void BO3PracticeTool::UserTabSelection()
{
	for (size_t i = 0; i < userTabSelections.size(); i++) {
		const TabSelection& group = userTabSelections[i];
		if (ImGui::BeginTabBar("User Tab Selection")) {
			if (ImGui::BeginTabItem(group.TabGroup.c_str())) {
				if (currentTabGroup != i) {
					currentTabGroup = static_cast<int>(i);
					std::string name = std::format("Black Ops 3 Practice Tool - {}: {}", group.TabGroup, group.TabItems[group.TabItemIndex]);
					Walnut::Application::Get().SetAppName(name);
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
		if (ImGui::Selection::RenderSelection(group.TabItems, group.TabItemIndex)) {
			std::string name = std::format("Black Ops 3 Practice Tool - {}: {}", group.TabGroup, group.TabItems[group.TabItemIndex]);
			Walnut::Application::Get().SetAppName(name);
			ManageImageLifetimes(group.TabItems[group.TabItemIndex]);
		}
		ImGui::EndCombo();
	}

	BO3PracticeTool::RenderWindowBody(currentTabGroup, group.TabItemIndex);	
}

void BO3PracticeTool::RunPopups()
{
	switch (ImGui::PopupWrapper::GetPrepState()) {
	case ImGui::PopupStates::About: {
		ImGui::OpenPopup("About");
		ImGui::PopupWrapper::OpenPopup(ImGui::PopupStates::About);
		break;
	}
	default:
		break;
	}

	switch (ImGui::PopupWrapper::GetOpenState()) {
	case ImGui::PopupStates::About: {
		ImGui::SetNextWindowSize(ImVec2(400, 175));
		if (ImGui::BeginPopup("About", ImGuiWindowFlags_NoDocking)) {
			ImGui::Text::CenterText("Github Repository");
			ImGui::TextURL::RenderTextURL("Github Repository", "https://github.com/Joshr520/BO3-Practice-Tool", 0, 0);
			ImGui::TextWrapped("A tool for the Steam version of Call of Duty: Black Ops III. Used to speedrun the Zombies mode.");
			ImGui::TextWrapped("Allows the user to manipulate rng for practicing. Also provides many resources for doing real runs.");
			if (Walnut::UI::ButtonCentered("Close")) {
				ImGui::CloseCurrentPopup();
				ImGui::PopupWrapper::ClosePopup();
			}
			ImGui::EndPopup();
		}
		break;
	}
	default:
		break;
	}

	if (ImGui::PopupWrapper::GetOpenState() != ImGui::PopupStates::None && !ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId)) {
		ImGui::PopupWrapper::ClosePopup();
	}
}