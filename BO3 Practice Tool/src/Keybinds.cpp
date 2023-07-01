#include "Keybinds.h"
#include "ToolData.h"
#include "GlobalData.h"
#include "GUIState.h"
#include "Walnut/Application.h"
#include "Walnut/Input/Input.h"
#include <Walnut/FileFormats/json.h>

#include <unordered_set>

std::unordered_map<Uint8, std::string> sdlMouseButtons = { { SDL_BUTTON_LEFT, "Left Click" }, { SDL_BUTTON_MIDDLE, "Middle Click" }, { SDL_BUTTON_RIGHT, "Right Click" }, { SDL_BUTTON_X1, "Mouse 4" }, { SDL_BUTTON_X2, "Mouse 5" } };
std::unordered_map<std::string, USHORT> stringToRawMouse = { { "Left Click", 0x01, }, { "Middle Click", 0x10 }, {"Right Click", 0x04 }, { "Mouse 4", 0x40 }, { "Mouse 5", 0x100 }, { "Scroll Up", 0x401 }, { "Scroll Down", 0x3FF } };
std::unordered_set<std::string> validMouseBinds = { "Left Click", "Middle Click", "Right Click", "Mouse 4", "Mouse 5", "Scroll Up", "Scroll Down" };


static bool writeKeys = false;
static Walnut::JSONBuilder keybindBuilder;

namespace BO3PT
{
	// This is scuffed but SDL keys are easier to convert into a name, so the keybind registration occurs over 2 functions. Fairly confident the order in which they execute is always the same so it shouldn't matter
	void SDLKeyCallback(SDL_Event event)
	{
		if (writeKeys)
		{
			SDL_Keycode key = event.key.keysym.sym;
			hotkeyToAssign->second.keyNames += SDL_GetKeyName(key);

			rapidjson::Value& bindingData = keybindBuilder.GetDocument()[hotkeyToAssign->second.splitGroup.c_str()][hotkeyToAssign->first.c_str()];
			keybindBuilder.ModifyString(bindingData, "KeyNames", hotkeyToAssign->second.keyNames);
			keybindBuilder.SaveToFile(selfDirectory + "\\bindings.json");

			RemoveDuplicates();
			hotkeyToAssign = nullptr;
			registerHotKey = false;
			writeKeys = false;
		}
	}

	void RawKeyboardCallback(RAWINPUT* raw)
	{
		USHORT key = raw->data.keyboard.VKey;
		if (key == VK_CONTROL || key == VK_SHIFT || key == VK_MENU)
			return;

		if (key == VK_ESCAPE)
		{
			rapidjson::Value& bindingData = keybindBuilder.GetDocument()[hotkeyToAssign->second.splitGroup.c_str()][hotkeyToAssign->first.c_str()];
			keybindBuilder.ModifyString(bindingData, "KeyNames", "");
			keybindBuilder.ModifyInt(bindingData, "Mods", 0);
			keybindBuilder.ModifyInt(bindingData, "Key", 0);
			keybindBuilder.SaveToFile(selfDirectory + "\\bindings.json");
			hotkeyToAssign->second.keyNames = "";
			hotkeyToAssign = nullptr;
			registerHotKey = false;
			return;
		}

		int mods = 0;
		hotkeyToAssign->second.keyNames = "";
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			hotkeyToAssign->second.keyNames += "Ctrl+";
			mods |= MOD_CONTROL;
		}
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			hotkeyToAssign->second.keyNames += "Shift+";
			mods |= MOD_SHIFT;
		}
		if (GetKeyState(VK_MENU) & 0x8000)
		{
			hotkeyToAssign->second.keyNames += "Alt+";
			mods |= MOD_ALT;
		}
		hotkeyToAssign->second.type = 0;

		rapidjson::Value& bindingData = keybindBuilder.GetDocument()[hotkeyToAssign->second.splitGroup.c_str()][hotkeyToAssign->first.c_str()];
		keybindBuilder.ModifyInt(bindingData, "Mods", mods);
		keybindBuilder.ModifyInt(bindingData, "Key", key);
		keybindBuilder.SaveToFile(selfDirectory + "\\bindings.json");

		writeKeys = true;
	}

	void SDLMouseCallback(SDL_Event event)
	{
		std::string mouseBind;
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (sdlMouseButtons.find(event.button.button) == sdlMouseButtons.end())
				return;
			mouseBind = sdlMouseButtons[event.button.button];
		}
		else if (event.type == SDL_MOUSEWHEEL)
			event.wheel.y > 0 ? mouseBind = "Scroll Up" : mouseBind = "Scroll Down";
		hotkeyToAssign->second.keyNames = mouseBind;
		hotkeyToAssign->second.type = 1;

		rapidjson::Value& bind = keybindBuilder.GetDocument()[hotkeyToAssign->second.splitGroup.c_str()][hotkeyToAssign->first.c_str()];
		keybindBuilder.ModifyString(bind, "KeyNames", hotkeyToAssign->second.keyNames);
		keybindBuilder.ModifyInt(bind, "Key", stringToRawMouse[mouseBind]);
		keybindBuilder.ModifyInt(bind, "Mods", 0);
		keybindBuilder.SaveToFile(selfDirectory + "\\bindings.json");

		RemoveDuplicates();
		hotkeyToAssign = nullptr;
		registerHotKey = false;
	}

	void CheckAndRunMouseBinds(USHORT flags, USHORT data)
	{
		if (flags > 0x400)
			return;

		if (flags == 0x400)
			((short)(unsigned short)data) < 0 ? flags-- : flags++;

		if (mouseCalls.find(flags) != mouseCalls.end())
			mouseCalls[flags]();
	}

	void InitHotKeyBinds()
	{
		hotkeyDefs.insert({ "Godmode", { "In Game", std::function<void()>(GodmodeOn)} }); hotkeyDefs.insert({ "Infinite Ammo", { "In Game", std::function<void()>(InfiniteAmmo)} }); hotkeyDefs.insert({ "Increment Timescale", { "In Game", std::function<void()>(TimescaleIncrease)} });
		hotkeyDefs.insert({ "Decrement Timescale", { "In Game", std::function<void()>(TimescaleDecrease)} }); hotkeyDefs.insert({ "Max Points", { "In Game", std::function<void()>(MaxPoints)} }); hotkeyDefs.insert({ "Reset Points", { "In Game", std::function<void()>(ResetPoints)} });
		hotkeyDefs.insert({ "Give All Perks", { "In Game", std::function<void()>(GiveAllPerks)} }); hotkeyDefs.insert({ "Take All Perks", { "In Game", std::function<void()>(TakeAllPerks)} }); hotkeyDefs.insert({ "Take Gum", { "In Game", std::function<void()>(TakeGum)} });
		hotkeyDefs.insert({ "Take Gum Charge", { "In Game", std::function<void()>(TakeGumCharge)} }); hotkeyDefs.insert({ "Activate Gum", { "In Game", std::function<void()>(ActivateGum)} }); hotkeyDefs.insert({ "Zombies Ignore Players", { "In Game", std::function<void()>(ZombiesIgnorePlayer)} });
		hotkeyDefs.insert({ "Kill Horde", { "In Game", std::function<void()>(KillHorde)} }); hotkeyDefs.insert({ "Freeze Zombies", { "In Game", std::function<void()>(FreezeZombies)} }); hotkeyDefs.insert({ "Toggle Zombie Spawns", { "In Game", std::function<void()>(ToggleSpawning)} });
		hotkeyDefs.insert({ "Zombies Walk", { "In Game", std::function<void()>(ZombiesWalk)} }); hotkeyDefs.insert({ "Zombies Run", { "In Game", std::function<void()>(ZombiesRun)} }); hotkeyDefs.insert({ "Zombies Sprint", { "In Game", std::function<void()>(ZombiesSprint)} });
		hotkeyDefs.insert({ "End Round", { "In Game", std::function<void()>(EndRound)} }); hotkeyDefs.insert({ "Restart Round", { "In Game", std::function<void()>(RestartRound)} }); hotkeyDefs.insert({ "Pickup Every Part", { "In Game", std::function<void()>(PickupEveryPart)} });
		hotkeyDefs.insert({ "Open All Doors", { "In Game", std::function<void()>(OpenAllDoors)} }); hotkeyDefs.insert({ "Global Power On", { "In Game", std::function<void()>(GlobalPowerOn)} }); hotkeyDefs.insert({ "Open All Barriers", { "In Game", std::function<void()>(OpenAllBarriers)} });
		hotkeyDefs.insert({ "Close All Barriers", { "In Game", std::function<void()>(CloseAllBarriers)} });

		hotkeyDefs.insert({ "Increment Gum Tracker", { "Gum Tracker", std::function<void()>(IncrementGumTracker) } }); hotkeyDefs.insert({ "Decrement Gum Tracker", { "Gum Tracker", std::function<void()>(DecrementGumTracker) } });
		hotkeyDefs.insert({ "Toggle Gum Tracker", { "Gum Tracker", std::function<void()>(ToggleGumTracker) } });

		keybindBuilder = Walnut::JSONBuilder::FromFile(selfDirectory + "\\bindings.json");

		LoadKeybinds();
		keybindBuilder.SaveToFile(selfDirectory + "\\bindings.json");
	}

	void LoadKeybinds()
	{
		bool valid = true;
		int index = 0;
		for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs)
		{
			rapidjson::Value& bindingData = keybindBuilder.GetDocument();
			hotkey.second.index = index;
			index++;
			if (!bindingData.HasMember(hotkey.second.splitGroup.c_str()))
			{
				rapidjson::Value& newGroup = keybindBuilder.AddObject(bindingData, hotkey.second.splitGroup.c_str());
				rapidjson::Value& newBind = keybindBuilder.AddObject(newGroup, hotkey.first.c_str());
				newBind.AddMember("KeyNames", "", keybindBuilder.GetAllocator());
				newBind.AddMember("Key", 0, keybindBuilder.GetAllocator());
				newBind.AddMember("Mods", 0, keybindBuilder.GetAllocator());
			}
			else if (!bindingData[hotkey.second.splitGroup.c_str()].HasMember(hotkey.first.c_str()))
			{
				rapidjson::Value& newBind = keybindBuilder.AddObject(bindingData[hotkey.second.splitGroup.c_str()], hotkey.first.c_str());
				newBind.AddMember("KeyNames", "", keybindBuilder.GetAllocator());
				newBind.AddMember("Key", 0, keybindBuilder.GetAllocator());
				newBind.AddMember("Mods", 0, keybindBuilder.GetAllocator());
			}
			else if (bindingData[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()]["KeyNames"].GetString() != "")
			{
				std::string keys = bindingData[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()]["KeyNames"].GetString();
				hotkey.second.keyNames = keys;

				if (validMouseBinds.find(keys) != validMouseBinds.end())
					hotkey.second.type = 1;
			}
		}

		RemoveDuplicates();
	}

	void RemoveDuplicates()
	{
		std::unordered_set<std::string> usedBinds;
		hotkeyCalls.clear();
		mouseCalls.clear();
		if (hotkeyToAssign)
		{
			usedBinds.insert(hotkeyToAssign->second.keyNames);
			int key = keybindBuilder.GetDocument()[hotkeyToAssign->second.splitGroup.c_str()][hotkeyToAssign->first.c_str()]["Key"].GetInt();
			int mods = keybindBuilder.GetDocument()[hotkeyToAssign->second.splitGroup.c_str()][hotkeyToAssign->first.c_str()]["Mods"].GetInt();
			if (!hotkeyToAssign->second.type)
				hotkeyCalls.insert({ { key, mods }, hotkeyToAssign->second.activatedFunc });
			else
				mouseCalls.insert({ key , hotkeyToAssign->second.activatedFunc });
		}

		for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs)
		{
			if (hotkey.second.keyNames == "")
			{
				hotkey.second.type = 0;
				keybindBuilder.ModifyString(keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()], "KeyNames", "");
				keybindBuilder.ModifyInt(keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()], "Key", 0);
				keybindBuilder.ModifyInt(keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()], "Mods", 0);
				continue;
			}
			if (hotkeyToAssign && hotkey.second.index == hotkeyToAssign->second.index)
				continue;

			if (usedBinds.find(hotkey.second.keyNames) == usedBinds.end())
				{
					usedBinds.insert(hotkey.second.keyNames);
					int key = keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()]["Key"].GetInt();
					int mods = keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()]["Mods"].GetInt();
					if (!hotkey.second.type)
						hotkeyCalls.insert({ { key, mods }, hotkey.second.activatedFunc });
					else
						mouseCalls.insert({ key , hotkey.second.activatedFunc });
				}
			else
			{
				hotkey.second.keyNames = "";
				hotkey.second.type = 0;
				keybindBuilder.ModifyString(keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()], "KeyNames", "");
				keybindBuilder.ModifyInt(keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()], "Key", 0);
				keybindBuilder.ModifyInt(keybindBuilder.GetDocument()[hotkey.second.splitGroup.c_str()][hotkey.first.c_str()], "Mods", 0);
			}
		}

		keybindBuilder.SaveToFile(selfDirectory + "\\bindings.json");
	}

	void AssignHotKey(const std::string& jsonKey, std::pair<const std::string, HotKeyBind>& hotkey)
	{
		jsonKeyToAssign = jsonKey;
		hotkeyToAssign = &hotkey;
		registerHotKey = true;
	}

	void GodmodeOn()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		godActive = !godActive;
		NotifyGame({ 0, 11, godActive });
	}
	void InfiniteAmmo()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		infAmmoActive = !infAmmoActive;
		NotifyGame({ 0, 12, infAmmoActive });
	}
	void TimescaleIncrease()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		if (timescaleInt < 10)
			timescaleInt++;
		NotifyGame({ 0, 13, timescaleInt });
	}
	void TimescaleDecrease()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		if (timescaleInt > 1)
			timescaleInt--;
		NotifyGame({ 0, 13, timescaleInt });
	}
	void MaxPoints()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 0, 4194303 });
	}
	void ResetPoints()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 0, 500 });
	}
	void GiveAllPerks()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 7 });
	}
	void TakeAllPerks()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 8 });
	}
	void TakeGum()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 4 });
	}
	void TakeGumCharge()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 5 });
	}
	void ActivateGum()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 0, 6 });
	}
	void ZombiesIgnorePlayer()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 1, 0, zombiesIgnore });
	}
	void KillHorde()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 1, 1 });
	}
	void FreezeZombies()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 1, 2, zombiesFreeze });
	}
	void ToggleSpawning()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 1, 3, zombiesSpawn });
	}
	void ZombiesWalk()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		zombieSpeedRun = false;
		zombieSpeedSprint = false;
		if (zombieSpeedWalk)
			NotifyGame({ 1, 4, 0 });
		else
			NotifyGame({ 1, 4, 3 });
	}
	void ZombiesRun()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		zombieSpeedWalk = false;
		zombieSpeedSprint = false;
		if (zombieSpeedRun)
			NotifyGame({ 1, 4, 1 });
		else
			NotifyGame({ 1, 4, 3 });
	}
	void ZombiesSprint()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		zombieSpeedWalk = false;
		zombieSpeedRun = false;
		if (zombieSpeedSprint)
			NotifyGame({ 1, 4, 2 });
		else
			NotifyGame({ 1, 4, 3 });
	}
	void EndRound()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 2, 0 });
	}
	void RestartRound()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 2, 1 });
	}
	void PickupEveryPart()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 5, 0 });
	}
	void OpenAllDoors()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 6, 0 });
	}
	void GlobalPowerOn()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 6, 2 });
	}
	void OpenAllBarriers()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 6, 5, 0 });
	}
	void CloseAllBarriers()
	{
		if (!GUIState::IsStateSet(Active) || currentMap.substr(0, 2) != "zm")
			return;
		NotifyGame({ 6, 5, 1 });
	}

	void IncrementGumTracker()
	{
		if (showBGBSelection)
			return;
		if (gumTrackCurrentIndex == 4)
			gumTrackCurrentIndex = 0;
		else
			gumTrackCurrentIndex++;
		bgbContext = gumTrackBGBs[gumTrackCurrentIndex];
	}
	void DecrementGumTracker()
	{
		if (showBGBSelection)
			return;
		if (gumTrackCurrentIndex == 0)
			gumTrackCurrentIndex = 4;
		else
			gumTrackCurrentIndex--;
		bgbContext = gumTrackBGBs[gumTrackCurrentIndex];
	}
	void ToggleGumTracker()
	{
		if (showBGBSelection)
			return;
		gumTrackChosen[gumTrackCurrentIndex] = !gumTrackChosen[gumTrackCurrentIndex];
		if (std::all_of(std::begin(gumTrackChosen), std::end(gumTrackChosen), [](bool value) { return value; }))
			std::fill(std::begin(gumTrackChosen), std::end(gumTrackChosen), false);
	}
}