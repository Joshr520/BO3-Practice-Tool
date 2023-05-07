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
	bool CreateButton(const std::string& name, const ImVec2& size, bool* value = NULL, bool toggle = false, const ImVec4& color = { 25, 100, 128, 255 }, bool inGame = false);
	void FakeButton(const std::string& name, const ImVec2& size, const ImVec4& color);
	bool CreateListBox(const std::string& name, const std::vector<std::string>& items, int& currentItem, const ImVec2& boxSize);
	void HelpMarker(const char* text);
	bool CreateGumImages(std::vector<int>& gumArr, ImVec2 imgSize, int numOnLine, const char* type, std::function<void(int input)> funcOnPress);
	void SwapGumSelection(int newGum, int gumSlot);
	std::vector<int> GumSearch(const std::vector<int>& inGumArr, const std::string& searchText);
	bool DoesPathExist(const std::string& s);
	void VerifyFileStructure();
	void WritePracticePatches(int patch[8]);
	void NotifyGame(std::vector<int> passList);
	void InjectTool(bool enable, bool& injectResponse);
	std::vector<int> GetWeaponIndex(std::string currentMap, std::string weaponSelectName);
	void ResetToggles();
	bool CheckVersions(const std::string& newVersion, const std::string& oldVersion);
}

void LogFile(std::string text, bool initialBoot = false);
void NLog(std::string text);