#pragma once

#include <string>

namespace BO3PracticeTool
{
	void OnInit();
	void OnShutdown();
	void RenderWindowBody(size_t TabGroup, size_t TabItem);
	void ResetVariables();
	void OnGameShutdown();

	inline bool validMap = false;
	inline bool validCraftMap = false;
	inline std::string mapName;
	inline std::string tempDirectory;
	inline std::string steamDirectory;
	inline std::string bo3Directory;
	inline std::string practiceToolDirectory;
}