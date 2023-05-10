#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <filesystem>
#include "imgui.h"

namespace GUIWindow
{
	void InitVariables();
	void DummySpace(float x, float y);
	void SetToggleButtonColor(bool active);
	bool CreateButton(const std::string& name, const ImVec2& size, bool* value = NULL, bool toggle = false, const ImVec4& color = { 25, 100, 128, 255 }, bool inGame = false);
	void FakeButton(const std::string& name, const ImVec2& size, const ImVec4& color);
	bool CreateListBox(const std::string& name, const std::vector<std::string>& items, int& currentItem, const ImVec2& boxSize);
	void HelpMarker(const std::string& text);
	bool CreateGumImages(const std::vector<int>& gumArr, const ImVec2& imgSize, int numOnLine, const std::string& type, const std::function<void(int input)>& funcOnPress);
	void SwapGumSelection(int newGum, int gumSlot);
	std::vector<int> GumSearch(const std::vector<int>& inGumArr, const std::string& searchText);
	bool DoesPathExist(const std::string& s);
	void VerifyFileStructure();
	void WritePracticePatches(const int patch[8]);
	void NotifyGame(const std::vector<int>& passList);
	void InjectTool(bool enable, bool& injectResponse);
	std::vector<int> GetWeaponIndex(const std::string& currentMap, const std::string& weaponSelectName);
	void ResetToggles();
	bool CheckVersions(const std::string& newVersion, const std::string& oldVersion);
	bool DownloadAndExtractZip(const std::unordered_set<std::string_view>& wantedFiles);
}

void LogFile(const std::string& text, bool initialBoot = false);
void NLog(const std::string& text);