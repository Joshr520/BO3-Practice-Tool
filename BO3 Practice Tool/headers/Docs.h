#pragma once

#include <vector>
#include <string>
#include <functional>

namespace BO3PT {

	inline void SidebarDocsFunc();
	inline void BGBLoadoutDocsFunc();
	inline void WeaponLoadoutDocsFunc();
	inline void AutosplitsDocsFunc();
	inline void PracticePatchesDocsFunc();
	inline void SettingsDocsFunc();
	inline void PlayerOptionsDocsFunc();
	inline void ZombieOptionsDocsFunc();
	inline void RoundOptionsDocsFunc();
	inline void PowerupOptionsDocsFunc();
	inline void EggStepOptionsDocsFunc();
	inline void CraftableOptionsDocsFunc();
	inline void BlockerOptionsDocsFunc();
	inline void MapOptionsDocsFunc();
	inline void GumTrackerDocsFunc();
	inline void ZombieCalculatorDocsFunc();
	inline void CodeGuidesDocsFunc();
	inline void GKValveSolverDocsFunc();

	struct DocChild {
		std::string m_ChildTopic;
		std::function<void()> m_RenderFunc;
	};

	struct DocParent {
		std::string m_RootTopic;
		std::vector<DocChild> m_ChildTopics;
	};

	inline std::vector<DocParent> docMembers = {
		{
			"Sidebar", {
				{ "Sidebar", std::function<void()>(SidebarDocsFunc) }
			}
		},
		{
			"Frontend", {
				{ "Gobblegum Lodaouts", std::function<void()>(BGBLoadoutDocsFunc) },
				{ "Weapon Lodaouts", std::function<void()>(WeaponLoadoutDocsFunc) },
				{ "Autosplits", std::function<void()>(AutosplitsDocsFunc) },
				{ "Practice Patches", std::function<void()>(PracticePatchesDocsFunc) },
				{ "Settings", std::function<void()>(SettingsDocsFunc) }
			}
		},
		{
			"In Game", {
				{ "Player Options", std::function<void()>(PlayerOptionsDocsFunc) },
				{ "Zombie Options", std::function<void()>(ZombieOptionsDocsFunc) },
				{ "Round Options", std::function<void()>(RoundOptionsDocsFunc) },
				{ "Powerup Options", std::function<void()>(PowerupOptionsDocsFunc) },
				{ "Egg Step Options", std::function<void()>(EggStepOptionsDocsFunc) },
				{ "Craftable Options", std::function<void()>(CraftableOptionsDocsFunc) },
				{ "Blocker Options", std::function<void()>(BlockerOptionsDocsFunc) },
				{ "Map Options", std::function<void()>(MapOptionsDocsFunc) }
			}
		},
		{
			"Resources", {
				{ "Gum Tracker", std::function<void()>(GumTrackerDocsFunc) },
				{ "Zombie Calculator", std::function<void()>(ZombieCalculatorDocsFunc) },
				{ "Code Guides", std::function<void()>(CodeGuidesDocsFunc) },
				{ "GK Valve Solver", std::function<void()>(GKValveSolverDocsFunc) }
			}
		}
	};

}