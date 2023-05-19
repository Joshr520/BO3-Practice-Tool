#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>

namespace BO3PT
{
	struct HotKeyBind
	{
		int type = 0; // 0 = keyboard, 1 = mouse
		std::vector<int> keys = { };
		std::string keyNames = "";
		std::function<void()> activatedFunc;
	};

	inline bool globalHotKeys = false;
	inline bool registerHotKey = false;
	inline std::set<int> modsToAssign;
	inline std::pair<const std::string, HotKeyBind>* hotkeyToAssign;
	inline std::map<std::string, HotKeyBind> inGameHotkeyDefs;

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// HotKey Functions - valid function to be called on a hotkey press
	void GodmodeOn();
	void InfiniteAmmo();
	void TimescaleIncrease();
	void TimescaleDecrease();
	void MaxPoints();
	void ResetPoints();
	void GiveAllPerks();
	void TakeAllPerks();
	void TakeGum();
	void TakeGumCharge();
	void ActivateGum();
	void ZombiesIgnorePlayer();
	void KillHorde();
	void FreezeZombies();
	void ToggleSpawning();
	void ZombiesWalk();
	void ZombiesRun();
	void ZombiesSprint();
	void EndRound();
	void RestartRound();
	void PickupEveryPart();
	void OpenAllDoors();
	void GlobalPowerOn();
	void OpenAllBarriers();
	void CloseAllBarriers();
}