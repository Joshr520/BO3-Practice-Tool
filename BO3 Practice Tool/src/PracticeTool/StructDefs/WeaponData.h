#pragma once
#include "ImGui/ImGuiExtendedWidgets.h"
#include "Walnut/Image.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace BO3PracticeTool
{
	struct Weapon {
		std::string Name;
		size_t NumAttachments;
		std::vector<std::string> Optics;
		std::vector<std::string> Attachments;
	};

	struct WeaponList {
		std::vector<std::string> Types;
		std::vector<std::vector<Weapon>> Weapons;
	};

	inline WeaponList weaponList = {
		{
			{ "Submachine Guns", "Assault Rifles", "Shotguns", "Light Machine Guns", "Sniper Rifles", "Secondaries" }
		},
		{
			{
				{ "Vesper", 5, { "Reflex", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "VMP", 5, { "Reflex", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "Kuda", 5, { "Reflex", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "Pharo", 5, { "Reflex", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "Weevil", 5, { "Reflex", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } }
			},
			{
				{ "ICR-1", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "KN-44", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "M8A7", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "Sheiva", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "HVK-30", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } },
				{ "Man-O-War", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Long Barrel", "Fast Mags" } }
			},
			{
				{ "KRM-262", 5, { "Reflex", "ELO", "BOA 3" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Long Barrel", "Fast Mags" } },
				{ "Argus", 5, { "Reflex", "ELO", "BOA 3" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Long Barrel", "Fast Mags" } },
				{ "205 Brecci", 5, { "Reflex", "ELO", "BOA 3" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Long Barrel", "Fast Mags" } },
				{ "Haymaker 12", 5, { "Reflex", "ELO", "BOA 3" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Long Barrel", "Fast Mags" } }
			},
			{
				{ "BRM", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Fast Mags" } },
				{ "Dingo", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Fast Mags" } },
				{ "48 Dredge", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Fast Mags" } },
				{ "Gorgon", 5, { "Reflex", "Thermal", "Varix 3", "ELO", "BOA 3", "Recon" }, { "Quickdraw", "Stock", "Laser Sight", "Rapid Fire", "Grip", "Fast Mags" } }
			},
			{
				{ "Locus", 4, { "ELO", "Thermal", "Varix 3", "Recon" }, { "Ballistics CPU", "Stock", "Rapid Fire", "Fast Mags" } },
				{ "Drakon", 4, { "ELO", "Thermal", "Varix 3", "Recon" }, { "Ballistics CPU", "Stock", "Rapid Fire", "Fast Mags" } },
				{ "SVG-100", 4, { "ELO", "Thermal", "Varix 3", "Recon" }, { "Ballistics CPU", "Stock", "Rapid Fire", "Fast Mags" } }
			},
			{
				{ "Bowie Knife", 0 },
				{ "RK5", 2, { "Reflex", "ELO" }, { "Quickdraw", "Laser Sight", "Long Barrel", "Fast Mags" } },
				{ "L-CAR 9", 2, { "Reflex", "ELO" }, { "Quickdraw", "Laser Sight", "Long Barrel", "Fast Mags" } },
				{ "XM-53", 0 }
			}
		}
	};

	struct CamoList {
		std::vector<std::string> Types;
		std::vector<std::vector<std::string>> Camos;
	};

	inline CamoList camoList = {
		{
			{ "Campaign", "Multiplayer", "Zombies", "Black Market", "Extras" }
		},
		{
			{
				{ "Arctic", "Jungle", "Huntsman", "Woodlums" },
				{ "Jungle Tech", "Ash", "Flectarn", "Heat Stroke", "Snow Job", "Dante", "Integer", "6 Speed", "Policia", "Ardent", "Burnt", "Bliss", "Battle", "Chameleon", "Gold", "Diamond", "Dark Matter" },
				{ "Contagious", "Fear", "WMD", "Red Hex", "Lucid" },
				{ "Ritual", "Transgression", "Storm", "Wartorn", "Prestige", "Etching", "Ice", "Dust", "Jungle Party", "Contrast", "Verde", "Firebrand", "Field", "Stealth", "Light", "Spark", "Timber", "Inferno", "Hallucination", "Pixel", "Royal", "Infrared", "Heat", "Violet", "Halcyon", "Gem", "Monochrome", "Sunshine", "Swindler",
					"Intensity", "Energeon", "Haptic" },
				{ "Black Ops III", "Weaponized 115", "Cyborg", "True Vet", "Take Out", "Urban Digital", "Nuk3Town", "Mindful", "Jungle Cat", "C.O.D.E. Warriors", "Underadar", "Vintage", "Walker", "WW3", "Topaz", "Garnet", "Sapphire", "Emerald", "Amethyst", "Opal", "Overgrowth", "Bloody Valentine", "Dragon Fire", "Glacies Fire",
					"Atomic Fire", "Everlasting Fire", "Arcane Fire", "COD XP", "CWL Champions", "Excellence", "MindFreak", "Nv", "OrbitGG", "Tainted Minds", "Epsilon eSports", "Team Infused", "Team LDLC", "Millenium", "Splyce", "Supremacy", "Cloud9", "eLevate", "Team EnVy", "Faze Clan", "OpTic Gaming", "Rise Nation",
					"Loading...", "Underworld", "Cosmos", "Cosmic", "Infinitus", "Into the Void", "Universe", "Luck of the Irish", "Circuits", "Divinium", "Cherry Fizz", "Empire", "Permafrost", "Hive", "Watermelon" },
			}
		}
	};

	struct Camo {
		std::string Name;
		std::unique_ptr<Walnut::Image> Image;
	};

	struct WeaponItem {
		int8_t Optic;
		std::vector<size_t> Attachments;
		int8_t Camo;
	};

	class WeaponPreset {
	public:
		WeaponPreset(std::string_view name) : m_Name(name) { }

		std::string GetPresetName() const { return m_Name; }
		WeaponItem GetWeaponItem(std::string_view type, size_t gun) { return m_Items[type.data()][gun]; }

		void AddWeapons(std::string_view type, std::vector<WeaponItem> items) { m_Items.insert({ type.data(), items}); }
		void SetPresetName(std::string_view name) { m_Name = name; }

		void SavePreset();
		static void SavePreset(std::string_view name);

		bool operator==(const WeaponPreset& preset) { return m_Name == preset.m_Name; }
	private:
		std::string m_Name;
		std::unordered_map<std::string, std::vector<WeaponItem>> m_Items = { };
	};

	inline std::vector<WeaponPreset> weaponPresets;
	inline size_t weaponPresetsIndex = 0;
	inline size_t weaponPresetsMenu = 0;
	inline size_t weaponBeingEdited = 0;

	inline ImGui::FileEditor weaponFiles(ImGui::FileEditor{ "Weapon Preset", std::vector<ImGui::TextEditSelectable>{  } });
	inline ImGui::ImageSelection weaponMainWeapons;
	inline ImGui::ImageSelection weaponOpticSelection(-1);
	inline ImGui::ImageMultiSelection weaponAttachmentsSelection;

	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> weaponIconsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> opticsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> attachmentsImgList;
	inline std::vector<Camo> camosImgList;

	void LoadWeaponPresets();
	void AddWeaponPreset(std::string_view name);
	void DeleteWeaponPreset(std::string_view name);
	inline void WeaponPresetIndexChanged(size_t index) { weaponPresetsIndex = index; }
	void EditedWeaponPreset(std::string_view oldName, std::string_view newName);

	void LoadWeaponImages();
	static void UnloadWeaponImages() { weaponIconsImgList.clear(); opticsImgList.clear(); attachmentsImgList.clear(); camosImgList.clear(); }

	inline void SafeEraseWeaponPreset(std::vector<WeaponPreset>& presets, std::string_view presetName)
	{
		presets.erase(std::remove_if(presets.begin(), presets.end(), [&](const WeaponPreset& preset) {
			return preset.GetPresetName() == presetName;
			}), presets.end());
	}
}