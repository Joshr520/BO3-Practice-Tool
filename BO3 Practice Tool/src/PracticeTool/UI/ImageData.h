#pragma once

#include "PracticeTool/StructDefs/BGBData.h"
#include "PracticeTool/StructDefs/WeaponData.h"

namespace BO3PracticeTool
{
	inline void ManageImageLifetimes(std::string_view window)
	{
		if (bgbImages.size() && (window != "Gobblegum Presets" && window != "Gum Tracker")) {
			UnloadBGBImages();
		}
		if (weaponIconsImgList.size()) {
			UnloadWeaponImages();
		}

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