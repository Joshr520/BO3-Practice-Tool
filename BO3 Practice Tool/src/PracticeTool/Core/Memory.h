#pragma once
#include <string>
#include <vector>
#include <Windows.h>

namespace Memory
{
	void ProcessLifetime();

	inline bool validMap = false;
	inline bool validCraftMap = false;
	inline bool overrideMap = false;
	inline bool mapChanged = false;
	inline bool mapRestarted = false;
	inline bool injected = false;
	inline int levelTime = 0;
	inline uint8_t round = 0;
	inline std::string mapName;
}