#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <algorithm>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include "imgui.h"
#include "Walnut/Image.h"
#include "json.h"

inline bool done = false;
inline bool enabled = false;
inline std::string appStatus = "Status: Inactive";
inline std::string currentMap = "";
inline std::string bo3Directory;
inline std::string selfDirectory;
inline std::string downloadURL;
inline std::vector<Walnut::Image*> bgbImgList;
inline std::vector<Walnut::Image*> codeImgList;
inline std::vector<Walnut::Image*> valveSolverImgList;
inline std::unordered_map<std::string, Walnut::Image*> iceCodeImgList;

namespace GUIFunctions
{
	void InitVariables();
	void InitImages();
	void HelpMarker(const std::string& text);
	void DummySpace(float x, float y);
	void SetToggleButtonColor(bool active);
	void FakeButton(const std::string& name, const ImVec2& size, const ImVec4& color);
	void SwapGumSelection(int newGum, int gumSlot);
	void VerifyFileStructure();
	void WritePracticePatches(const int patch[8]);
	void NotifyGame(const std::vector<int>& passList);
	void InjectTool(bool enable, bool& injectResponse);
	void ResetToggles();

	bool CreateButton(const std::string& name, const ImVec2& size, bool* value = NULL, bool toggle = false, const ImVec4& color = { 25, 100, 128, 255 }, bool inGame = false);
	bool CreateListBox(const std::string& name, const std::vector<std::string>& items, int& currentItem, const ImVec2& boxSize);
	bool CreateGumImages(const std::vector<int>& gumArr, const ImVec2& imgSize, int numOnLine, const std::string& type, const std::function<void(int input)>& funcOnPress);
	bool DoesPathExist(const std::string_view& s);
	bool CheckVersions(const std::string& newVersion, const std::string& oldVersion);
	bool DownloadAndExtractZip(const std::unordered_set<std::string_view>& wantedFiles);

	std::vector<int> GumSearch(const std::vector<int>& inGumArr, const std::string& searchText);	
	std::vector<int> GetWeaponIndex(const std::string& currentMap, const std::string& weaponSelectName);
}

void LogFile(const std::string& text, bool initialBoot = false);
void NLog(const std::string& text);