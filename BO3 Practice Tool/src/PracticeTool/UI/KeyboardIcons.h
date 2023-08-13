#pragma once

#include "Walnut/Image.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace BO3PracticeTool
{
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> keyboardIcons;

	void LoadKeyboardIcons();
	inline void UnloadKeyboardIcons() { keyboardIcons.clear(); }
}