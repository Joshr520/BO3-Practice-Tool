#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include "SDL.h"

#include <Windows.h>

struct hashPair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);

		if (hash1 != hash2) {
			return hash1 ^ hash2;
		}

		return hash1;
	}
};

namespace BO3PT
{
	struct HotKeyBind
	{
		// splitGroup = how to categorize the keybinds in the json
		std::string splitGroup;
		std::function<void()> activatedFunc;
		std::string keyNames = "";
		// 0 = keyboard, 1 = mouse
		int type = 0;
		int index = 0;
	};

	// HOTKEY PIPELINE
	// Call InitHotKeyBinds() to initialize at startup
	// When changing a hotkey, modify the keybindBuilder json object with the updated keybind, then call RemoveDuplicates() which will update the json data and keybind call data

	inline bool globalHotKeys = false;
	inline bool registerHotKey = false;
	inline std::string jsonKeyToAssign;
	inline std::pair<const std::string, HotKeyBind>* hotkeyToAssign = nullptr;
	inline std::map<const std::string, HotKeyBind> hotkeyDefs;
	inline std::unordered_map<std::pair<int, int>, std::function<void()>, hashPair> hotkeyCalls;
	inline std::unordered_map<int, std::function<void()>> mouseCalls;

	void SDLKeyCallback(SDL_Event event);
	void RawKeyboardCallback(RAWINPUT* raw);
	void SDLMouseCallback(SDL_Event event);
	void CheckAndRunMouseBinds(USHORT flags, USHORT data);
	void InitHotKeyBinds();
	void LoadKeybinds();
	void RemoveDuplicates();
	void AssignHotKey(const std::string& jsonKey, std::pair<const std::string, HotKeyBind>& hotkey);

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

	void IncrementGumTracker();
	void DecrementGumTracker();
	void ToggleGumTracker();
}