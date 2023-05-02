#pragma once

#include <string>
#include <vector>
#include <functional>
#include "imgui.h"

namespace GUIWindow
{
	void InitVariables();
	void DummySpace(float x, float y);
	void SetToggleButtonColor(bool active);
	bool CreateButton(std::string name, ImVec2 size, bool* value = NULL, bool toggle = false, ImVec4 color = { 25, 100, 128, 255 }, bool inGame = false);
	void FakeButton(std::string name, ImVec2 size, ImVec4 color);
	bool CreateListBox(const char* name, std::vector<std::string> items, int& currentItem, ImVec2 boxSize);
	void HelpMarker(const char* text);
	bool CreateGumImages(std::vector<int>& gumArr, ImVec2 imgSize, int numOnLine, const char* type, std::function<void(int input)> funcOnPress);
	void SwapGumSelection(int newGum, int gumSlot);
	std::vector<int> GumSearch(std::vector<int> inGumArr, char searchText[64]);
	bool DoesPathExist(const std::string& s);
	void VerifyFileStructure();
	void WritePracticePatches(int patch[8]);
	void NotifyGame(std::vector<int> passList);
	void InjectTool(bool enable);
	std::vector<int> GetWeaponIndex(std::string currentMap, std::string weaponSelectName);
	void ResetToggles();
}

void LogFile(std::string text);
void NLog(std::string text);