// I assume there's a library that can be used for keybinds. I only did a little google searching and this has been pretty fun tbh so I don't mind it being a little scuffed 

#pragma once
#include <unordered_map>
#include <map>
#include <string>
#include <functional>
#include <Windows.h>

namespace KeyBinds
{
	struct HotKeyBind
	{
		int type = 0; // 0 = keyboard, 1 = mouse
		std::vector<int> keys = { };
		std::string keyNames = "";
		std::function<void()> activatedFunc;
	};

	inline bool keyboardRegistered = false;
	inline bool globalHotKeys = false;
	inline bool registerHotKey = false;
	inline int initialKeyToRelease = 0;
	inline int modifiersPressed = 0;
	inline int totalNumKeys = 0;
	inline std::string jsonKeyToAssign;
	inline std::pair<const std::string , HotKeyBind>* hotkeyToAssign;
	inline std::map<std::string, HotKeyBind> hotkeyDefs;
	inline std::vector<int> usedKeys;
	inline std::unordered_map<int, bool> keyMap;
	inline std::unordered_map<int, bool> keyMapHeld;
	inline std::unordered_map<int, std::string> keyDictionary = { { 0x1b, "Escape" }, { 0x70, "F1" }, { 0x71, "F2" }, { 0x72, "F3" }, { 0x73, "F4" }, { 0x74, "F5" }, { 0x75, "F6" }, { 0x76, "F7" }, { 0x77, "F8" }, { 0x78, "F9" }, { 0x79, "F10" },
		{ 0x7a, "F11" }, { 0x7b, "F12" }, { 0x7c, "F13" }, { 0x7d, "F14" }, { 0x7e, "F15" }, { 0x7f, "F16" }, { 0x80, "F17" }, { 0x81, "F18" }, { 0x82, "F19" }, { 0x83, "F20" }, { 0x84, "F21" }, { 0x85, "F22" }, { 0x86, "F23" }, { 0x87, "F24" },
		{ 0x91, "Scroll Lock" }, { 0x13, "Pause" }, { 0xc0, "Tilde" }, { 0x31, "1" }, { 0x32, "2" }, { 0x33, "3" }, { 0x34, "4" }, { 0x35, "5" }, { 0x36, "6" }, { 0x37, "7" }, { 0x38, "8" }, { 0x39, "9" }, { 0x30, "0" }, { 0xbd, "-" }, { 0xbb, "=" },
		{ 0x08, "Backspace" }, { 0x2d, "Insert" }, { 0x24, "Home" }, { 0x21, "Page Up" }, { 0x90, "Num Lock" }, { 0x6f, "Num /" }, { 0x6a, "Num *" }, { 0x6d, "Num -" }, { 0x09, "Tab" }, { 0x51, "Q" }, { 0x57, "W" }, { 0x45, "E" }, { 0x52, "R" },
		{ 0x54, "T" }, { 0x59, "Y" }, { 0x55, "U" }, { 0x49, "I" }, { 0x4f, "O" }, { 0x50, "P" }, { 0xdb, "[" }, { 0xdd, "]" }, { 0xdc, "\\" }, { 0x2e, "Delete" }, { 0x23, "End" }, { 0x22, "Pg Down" }, { 0x67, "Num 7" }, { 0x68, "Num 8" },
		{ 0x69, "Num 9" }, { 0x6b, "Num + " }, { 0x14, "Caps Lock" }, { 0x41, "A" }, { 0x53, "S" }, { 0x44, "D" }, { 0x46, "F" }, { 0x47, "G" }, { 0x48, "H" }, { 0x4a, "J" }, { 0x4b, "K" }, { 0x4c, "L" }, { 0xba, "; " }, { 0xde, "'" },
		{ 0x0d, "Enter" }, { 0x64, "Num 4" }, { 0x65, "Num 5" }, { 0x66, "Num 6" }, { 0x10, "Shift" }, { 0xa0, "L Shift" }, { 0xa1, "R Shift" }, { 0x5a, "Z" }, { 0x58, "X" }, { 0x43, "C" }, { 0x56, "V" }, { 0x42, "B" }, { 0x4e, "N" },
		{ 0x4d, "M" }, { 0xbc, "," }, { 0xbe, "." }, { 0xbf, "/" }, { 0x26, "Up Arrow" }, { 0x61, "Num 1" }, { 0x62, "Num 2" }, { 0x63, "Num 3" }, { 0x11, "Ctrl" }, { 0xa2, "L Ctrl" }, { 0xa3, "R Ctrl" }, { 0x5b, "L Windows" },
		{ 0x5c, "R Windows" }, { 0x12, "Alt" }, { 0xa4, "L Alt" }, { 0xa5, "R Alt" }, { 0x20, "Space" }, { 0x25, "L Arrow" }, { 0x28, "Down Arrow" }, { 0x27, "R Arrow" }, { 0x60, "Num 0" }, { 0x6e, "Num ." } };
	inline std::vector<int> usedButtons;
	inline std::unordered_map<int, bool> buttonMap;
	inline std::unordered_map<int, bool> buttonMapHeld;
	inline std::unordered_map<int, std::string> buttonDictionaryDown = { { 0x01, "Left Click" }, { 0x04, "Right Click" }, { 0x10, "Middle Click" }, { 0x40, "Mouse 4" }, { 0x100, "Mouse 5" }, { 0x400, "Scroll Up" },
		{ -0x400, "Scroll Down" },  {0x800, "Scroll Right"}, { -0x800, "Scroll Left" } };
	inline std::unordered_map<int, std::string> buttonDictionaryUp = { { 0x02, "Left Click" }, { 0x08, "Right Click" }, { 0x20, "Middle Click" }, { 0x80, "Mouse 4" }, { 0x200, "Mouse 5" } };

	void InitHotKeyBinds();
	bool KeyPressed(int key, bool global = globalHotKeys);
	void AssignHotKey(const std::string& jsonKey, std::pair<const std::string, HotKeyBind>& hotkey);
	bool ValidateKeybind(std::pair<const std::string, HotKeyBind>& hotkey, bool write = false);
	void CheckAndRunKeybind(int type);
	bool RegisterRawInput(const HWND& hTarget, USHORT usage);

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