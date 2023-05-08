#include "KeyBinds.h"
#include "GUIWindow.h"
#include "PlayerOptions.h"
#include "ZombieOptions.h"
#include "Helper.h"
#include "Resources.h"

#include <thread>
#include <hidusage.h>
#include <fstream>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

HHOOK keyboardHook;

bool InternalKeyValidation(const std::vector<int>& keys);
bool VectorSort(const std::string& a, const std::string& b) { return a < b; }

namespace KeyBinds
{
	void InitHotKeyBinds()
	{
		hotkeyDefs.insert({ "Godmode", { 0, {}, "", std::function<void()>(GodmodeOn)} }); hotkeyDefs.insert({ "Infinite Ammo", { 1, {}, "", std::function<void()>(InfiniteAmmo)} }); hotkeyDefs.insert({ "Increment Timescale", { 2, {}, "", std::function<void()>(TimescaleIncrease)} });
		hotkeyDefs.insert({ "Decrement Timescale", { 3, {}, "", std::function<void()>(TimescaleDecrease)} }); hotkeyDefs.insert({ "Max Points", { 4, {}, "", std::function<void()>(MaxPoints)} }); hotkeyDefs.insert({ "Reset Points", { 5, {}, "", std::function<void()>(ResetPoints)} });
		hotkeyDefs.insert({ "Give All Perks", { 6, {}, "", std::function<void()>(GiveAllPerks)} }); hotkeyDefs.insert({ "Take All Perks", { 7, {}, "", std::function<void()>(TakeAllPerks)} }); hotkeyDefs.insert({ "Take Gum", { 8, {}, "", std::function<void()>(TakeGum)} });
		hotkeyDefs.insert({ "Take Gum Charge", { 9, {}, "", std::function<void()>(TakeGumCharge)} }); hotkeyDefs.insert({ "Activate Gum", { 10, {}, "", std::function<void()>(ActivateGum)} }); hotkeyDefs.insert({ "Zombies Ignore Players", { 11, {}, "", std::function<void()>(ZombiesIgnorePlayer)} });
		hotkeyDefs.insert({ "Kill Horde", { 12, {}, "", std::function<void()>(KillHorde)} }); hotkeyDefs.insert({ "Freeze Zombies", { 13, {}, "", std::function<void()>(FreezeZombies)} }); hotkeyDefs.insert({ "Toggle Zombie Spawns", { 14, {}, "", std::function<void()>(ToggleSpawning)} });
		hotkeyDefs.insert({ "Zombies Walk", { 15, {}, "", std::function<void()>(ZombiesWalk)} }); hotkeyDefs.insert({ "Zombies Run", { 16, {}, "", std::function<void()>(ZombiesRun)} }); hotkeyDefs.insert({ "Zombies Sprint", { 17, {}, "", std::function<void()>(ZombiesSprint)} });
		hotkeyDefs.insert({ "End Round", { 18, {}, "", std::function<void()>(EndRound)} }); hotkeyDefs.insert({ "Restart Round", { 19, {}, "", std::function<void()>(RestartRound)} }); hotkeyDefs.insert({ "Pickup Every Part", { 20, {}, "", std::function<void()>(PickupEveryPart)} });
		hotkeyDefs.insert({ "Open All Doors", { 21, {}, "", std::function<void()>(OpenAllDoors)} }); hotkeyDefs.insert({ "Global Power On", { 22, {}, "", std::function<void()>(GlobalPowerOn)} }); hotkeyDefs.insert({ "Open All Barriers", { 23, {}, "", std::function<void()>(OpenAllBarriers)} });
		hotkeyDefs.insert({ "Close All Barriers", { 34, {}, "", std::function<void()>(CloseAllBarriers)} });

		std::ifstream bindingsInFile(GUIWindow::selfDirectory + "\\bindings.json");
		json data = json::parse(bindingsInFile);
		bindingsInFile.close();
		bool valid = true;
		for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs)
		{
			if (!data.contains(hotkey.first))
			{
				data[hotkey.first] = "";
				valid = false;
			}
			else
			{
				std::string ssInput = data.at(hotkey.first);
				std::stringstream ss(ssInput);
				std::string token;
				while (std::getline(ss, token, '+'))
				{
					auto it = std::find_if(keyDictionary.begin(), keyDictionary.end(), [&token](auto&& pair) {
						return pair.second == token;
						});
					if (it == keyDictionary.end())
					{
						data[hotkey.first] = "";
						valid = false;
					}
					else
						hotkey.second.keys.push_back(it->first);
				}

				if (!ValidateKeybind(hotkey))
				{
					data[hotkey.first] = "";
					valid = false;
				}
				else if (hotkey.second.keys.size())
					hotkey.second.keyNames = ssInput;
			}
		}

		if (!valid)
		{
			std::ofstream bindingsOutFile(GUIWindow::selfDirectory + "\\bindings.json");
			bindingsOutFile << std::setw(4) << data;
			bindingsOutFile.close();
		}
		
	}

	bool KeyPressed(int key, bool global)
	{
		if (keyMap.find(key) == keyMap.end())
			keyMap.insert({ key, false });
		if (keyMapHeld.find(key) == keyMapHeld.end())
			keyMapHeld.insert({ key, false });
		if (global)
			return keyMap[key] && !keyMapHeld[key];
		return keyMap[key] && !keyMapHeld[key] && GetForegroundWindow() == GUIWindow::hWnd;
	}

	void AssignHotKey(const std::string& jsonKey, std::pair<const std::string, HotKeyBind>& hotkey)
	{
		jsonKeyToAssign = jsonKey;
		hotkeyToAssign = &hotkey;
		registerHotKey = true;
	}

	bool ValidateKeybind(std::pair<const std::string, HotKeyBind>& hotkey, bool write)
	{
		std::ifstream bindingsInFile(GUIWindow::selfDirectory + "\\bindings.json");
		json data = json::parse(bindingsInFile);
		bindingsInFile.close();
		if (!InternalKeyValidation(hotkey.second.keys))
		{
			hotkey.second.keyNames = "";
			hotkey.second.keys = { };
			if (write && jsonKeyToAssign.size())
			{
				data[jsonKeyToAssign] = "";
				std::ofstream bindingsOutFile(GUIWindow::selfDirectory + "\\bindings.json");
				bindingsOutFile << std::setw(4) << data;
				bindingsOutFile.close();
			}
			return false;
		}
		std::stringstream sortString(hotkey.second.keyNames);
		std::string ctrl = "";
		std::string shift = "";
		std::string alt = "";
		std::string activator = "";
		std::string token;
		while (std::getline(sortString, token, '+'))
		{
			switch (hashstr(token.c_str()))
			{
			case hashstr("Ctrl"):
			case hashstr("L Ctrl"):
			case hashstr("R Ctrl"):
				ctrl = token + "+";
				break;
			case hashstr("Shift"):
			case hashstr("L Shift"):
			case hashstr("R Shift"):
				shift = token + "+";
				break;
			case hashstr("Alt"):
			case hashstr("L Alt"):
			case hashstr("R Alt"):
				alt = token + "+";
				break;
			default:
				activator = token;
				break;
			}
		}
		hotkey.second.keyNames = ctrl + shift + alt + activator;
		for (std::pair<const std::string, HotKeyBind>& hotkeyDef : hotkeyDefs)
		{
			if (hotkeyDef.second.keyNames != hotkey.second.keyNames || hotkeyDef.first == hotkey.first)
				continue;
			hotkeyDef.second.keyNames = "";
			hotkeyDef.second.keys = { };
			if (write && jsonKeyToAssign.size())
			{
				data[hotkeyDef.first] = "";
				std::ofstream bindingsOutFile(GUIWindow::selfDirectory + "\\bindings.json");
				bindingsOutFile << std::setw(4) << data;
				bindingsOutFile.close();
			}
		}
		if (write && jsonKeyToAssign.size())
		{
			data[jsonKeyToAssign] = hotkey.second.keyNames;
			std::ofstream bindingsOutFile(GUIWindow::selfDirectory + "\\bindings.json");
			bindingsOutFile << std::setw(4) << data;
			bindingsOutFile.close();
		}
		return true;
	}

	void CheckAndRunKeybind()
	{
		//if (registerHotKey || GUIWindow::appStatus != "Status: Active" || GUIWindow::currentMap == "core_frontend")
			//return;
		if (usedKeys.size() > 3 || !usedKeys.size())
			return;
		if (usedKeys.size() > 1)
		{
			if (!InternalKeyValidation(usedKeys))
				return;
		}
		for (std::pair<std::string, HotKeyBind> hotkey : hotkeyDefs)
		{
			if (usedKeys.size() != hotkey.second.keys.size())
				continue;
			bool skip = false;
			for (const int& key : usedKeys)
			{
				if (std::find(hotkey.second.keys.begin(), hotkey.second.keys.end(), key) == hotkey.second.keys.end())
				{
					skip = true;
					break;
				}
			}
			if (skip)
				continue;
			hotkey.second.activatedFunc();
			break;
		}
	}

	bool RegisterRawInput(const HWND& hTarget)
	{
		RAWINPUTDEVICE hid;
		hid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		hid.usUsage = HID_USAGE_GENERIC_KEYBOARD;
		hid.dwFlags = RIDEV_INPUTSINK;
		hid.hwndTarget = hTarget;

		return RegisterRawInputDevices(&hid, 1, sizeof(hid));
	}

	void GodmodeOn()
	{
		PlayerOptions::godActive = !PlayerOptions::godActive;
		GUIWindow::NotifyGame({ 0, 11, PlayerOptions::godActive });
	}
	void InfiniteAmmo()
	{
		PlayerOptions::infAmmoActive = !PlayerOptions::infAmmoActive;
		GUIWindow::NotifyGame({ 0, 12, PlayerOptions::infAmmoActive });
	}
	void TimescaleIncrease()
	{
		if (PlayerOptions::timescaleInt < 10)
			PlayerOptions::timescaleInt++;
		GUIWindow::NotifyGame({ 0, 13, PlayerOptions::timescaleInt });
	}
	void TimescaleDecrease()
	{
		if (PlayerOptions::timescaleInt > 1)
			PlayerOptions::timescaleInt--;
		GUIWindow::NotifyGame({ 0, 13, PlayerOptions::timescaleInt });
	}
	void MaxPoints()
	{
		GUIWindow::NotifyGame({ 0, 0, 4194303 });
	}
	void ResetPoints()
	{
		GUIWindow::NotifyGame({ 0, 0, 500 });
	}
	void GiveAllPerks()
	{
		GUIWindow::NotifyGame({ 0, 7 });
	}
	void TakeAllPerks()
	{
		GUIWindow::NotifyGame({ 0, 8 });
	}
	void TakeGum()
	{
		GUIWindow::NotifyGame({ 0, 4 });
	}
	void TakeGumCharge()
	{
		GUIWindow::NotifyGame({ 0, 5 });
	}
	void ActivateGum()
	{
		GUIWindow::NotifyGame({ 0, 6 });
	}
	void ZombiesIgnorePlayer()
	{
		GUIWindow::NotifyGame({ 1, 0, ZombieOptions::zombiesIgnore });
	}
	void KillHorde()
	{
		GUIWindow::NotifyGame({ 1, 1 });
	}
	void FreezeZombies()
	{
		GUIWindow::NotifyGame({ 1, 2, ZombieOptions::zombiesFreeze });
	}
	void ToggleSpawning()
	{
		GUIWindow::NotifyGame({ 1, 3, ZombieOptions::zombiesSpawn });
	}
	void ZombiesWalk()
	{
		ZombieOptions::zombieSpeedRun = false;
		ZombieOptions::zombieSpeedSprint = false;
		if (ZombieOptions::zombieSpeedWalk)
			GUIWindow::NotifyGame({ 1, 4, 0 });
		else
			GUIWindow::NotifyGame({ 1, 4, 3 });
	}
	void ZombiesRun()
	{
		ZombieOptions::zombieSpeedWalk = false;
		ZombieOptions::zombieSpeedSprint = false;
		if (ZombieOptions::zombieSpeedRun)
			GUIWindow::NotifyGame({ 1, 4, 1 });
		else
			GUIWindow::NotifyGame({ 1, 4, 3 });
	}
	void ZombiesSprint()
	{
		ZombieOptions::zombieSpeedWalk = false;
		ZombieOptions::zombieSpeedRun = false;
		if (ZombieOptions::zombieSpeedSprint)
			GUIWindow::NotifyGame({ 1, 4, 2 });
		else
			GUIWindow::NotifyGame({ 1, 4, 3 });
	}
	void EndRound()
	{
		GUIWindow::NotifyGame({ 2, 0 });
	}
	void RestartRound()
	{
		GUIWindow::NotifyGame({ 2, 1 });
	}
	void PickupEveryPart()
	{
		GUIWindow::NotifyGame({ 5, 0 });
	}
	void OpenAllDoors()
	{
		GUIWindow::NotifyGame({ 6, 0 });
	}
	void GlobalPowerOn()
	{
		GUIWindow::NotifyGame({ 6, 2 });
	}
	void OpenAllBarriers()
	{
		GUIWindow::NotifyGame({ 6, 5, 0 });
	}
	void CloseAllBarriers()
	{
		GUIWindow::NotifyGame({ 6, 5, 1 });
	}
}

bool InternalKeyValidation(const std::vector<int>& keys)
{
	bool activatorFound = false;
	bool validKeybind = true;
	for (const int& key : keys)
	{
		if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL || key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT || key == VK_MENU || key == VK_LMENU || key == VK_RMENU)
		{
			if (activatorFound)
				validKeybind = false;
			continue;
		}
		else if (KeyBinds::keyDictionary.contains(key))
			activatorFound = true;
	}
	return validKeybind && activatorFound;
}