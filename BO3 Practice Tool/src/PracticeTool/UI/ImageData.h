#pragma once

#include "PracticeTool/StructDefs/BGBData.h"
#include "PracticeTool/StructDefs/WeaponData.h"

namespace BO3PracticeTool
{
	inline void ManageImageLifetimes(std::string_view window)
	{
		if (bgbClassicImages.size() && (window != "Gobblegum Presets" && window != "Gum Tracker")) {
			BO3PracticeTool::UnloadBGBImages();
		}
		UnloadWeaponImages();

		if (window.empty()) {
			return;
		}

		if (window == "Gobblegum Presets") {
			LoadBGBImages();
		}
		else if (window == "Weapon Presets") {
			LoadWeaponImages();
		}
	}
}