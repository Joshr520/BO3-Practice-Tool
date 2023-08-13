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

	inline std::unordered_map<std::string, int> camoNameToIndex = {
		{
			{ "Jungle Tech", 1 }, { "Ash", 2 }, { "Flectarn", 3 }, { "Heat Stroke", 4 }, { "Snow Job", 5 }, { "Dante", 6 }, { "Integer", 7 }, { "6 Speed", 8 }, { "Policia", 9 }, { "Ardent", 10 }, { "Burnt", 11 }, { "Bliss", 12 },
			{ "Battle", 13 }, { "Chameleon", 14 }, { "Gold", 15 }, { "Diamond", 16 }, { "Dark Matter", 17 }, { "Arctic", 18 }, { "Jungle", 19 }, { "Huntsman", 20 }, { "Woodlums", 21 }, { "Contagious", 22 }, { "Fear", 23 },
			{ "Wmd", 24 }, { "Red Hex", 25 }, { "Ritual", 26 }, { "Black Ops III", 27 }, { "Weaponized 115", 28 }, { "Cyborg", 29 }, { "True Vet", 30 }, { "Snow Job", 31 }, { "6 Speed", 32 }, { "Take Out", 33 }, { "Urban Digital", 34 },
			{ "Nuk3Town", 35 }, { "Transgression", 36 }, { "Storm", 38 }, { "Wartorn", 39 }, { "Prestige", 40 }, { "Mindful", 41 }, { "Etching", 42 }, { "Ice", 43 }, { "Dust", 44 }, { "Jungle Cat", 45 }, { "Jungle Party", 46 },
			{ "Contrast", 47 }, { "Verde", 48 }, { "Firebrand", 49 }, { "Field", 50 }, { "Stealth", 51 }, { "Light", 52 }, { "Spark", 53 }, { "Timber", 54 }, { "Inferno", 55 }, { "Hallucination", 56 }, { "Pixel", 57 }, { "Royal", 58 },
			{ "Infrared", 59 }, { "Heat", 60 }, { "Violet", 61 }, { "Halcyon", 62 }, { "Gem", 63 }, { "Monochrome", 64 }, { "Sunshine", 65 }, { "Swindler", 66 }, { "C.O.D.E. Warriors", 67 }, { "Intensity", 68 }, { "Underadar", 69 },
			{ "Vintage", 71 }, { "Walker", 72 }, { "WW3", 73 }, { "Energeon", 74 }, { "Topaz", 75 }, { "Garnet", 76 }, { "Sapphire", 77 }, { "Emerald", 78 }, { "Amethyst", 79 }, { "Opal", 80 }, { "Overgrowth", 81 },
			{ "Bloody Valentine", 82 }, { "Haptic", 83 }, { "Dragon Fire", 84 }, { "Glacies Fire", 85 }, { "Atomic Fire", 86 }, { "Everlasting Fire", 87 }, { "Arcane Fire", 88 }, { "COD XP", 89 }, { "CWL Champions", 90 },
			{ "Excellence", 93 }, { "MindFreak", 95 }, { "Nv", 96 }, { "OrbitGG", 97 }, { "Tainted Minds", 98 }, { "Epsilon eSports", 99 }, { "Team Infused", 103 }, { "Team LDLC", 104 }, { "Millenium", 105 }, { "Splyce", 106 },
			{ "Supremacy", 107 }, { "Sloud9", 109 }, { "eLevate", 111 }, { "Team EnVy", 112 }, { "Faze Clan", 113 }, { "OpTic Gaming", 116 }, { "Rise Nation", 117 }, { "Loading...", 118 }, { "Underworld", 119 }, { "Cosmos", 121 },
			{ "Cosmic", 122 }, { "Infinitus", 123 }, { "Into the Void", 124 }, { "Universe", 125 }, { "Lucid", 126 }, { "Luck of the Irish", 131 }, { "Circuits", 132 }, { "Divinium", 133 }, { "Cherry Fizz", 134 }, { "Empire", 135 },
			{ "Permafrost", 136 }, { "Hive", 137 }, { "Watermelon", 138 }
		}
	};

	inline std::unordered_map<int, std::string> camoIndexToName = {
		{
			{ 1, "Jungle Tech" }, { 2, "Ash" }, { 3, "Flectarn" }, { 4, "Heat Stroke" }, { 5, "Snow Job" }, { 6, "Dante" }, { 7, "Integer" }, { 8, "6 Speed" }, { 9, "Policia" }, { 10, "Ardent" }, { 11, "Burnt" }, { 12, "Bliss" },
			{ 13, "Battle" }, { 14, "Chameleon" }, { 15, "Gold" }, { 16, "Diamond" }, { 17, "Dark Matter" }, { 18, "Arctic" }, { 19, "Jungle" }, { 20, "Huntsman" }, { 21, "Woodlums" }, { 22, "Contagious" }, { 23, "Fear" },
			{ 24, "Wmd" }, { 25, "Red Hex" }, { 26, "Ritual" }, { 27, "Black Ops III" }, { 28, "Weaponized 115" }, { 29, "Cyborg" }, { 30, "True Vet" }, { 31, "Snow Job" }, { 32, "6 Speed" }, { 33, "Take Out" },
			{ 34, "Urban Digital" }, { 35, "Nuk3Town" }, { 36, "Transgression" }, { 38, "Storm" }, { 39, "Wartorn" }, { 40, "Prestige" }, { 41, "Mindful" }, { 42, "Etching" }, { 43, "Ice" }, { 44, "Dust" }, { 45, "Jungle Cat" },
			{ 46, "Jungle Party" }, { 47, "Contrast" }, { 48, "Verde" }, { 49, "Firebrand" }, { 50, "Field" }, { 51, "Stealth" }, { 52, "Light" }, { 53, "Spark" }, { 54, "Timber" }, { 55, "Inferno" }, { 56, "Hallucination" },
			{ 57, "Pixel" }, { 58, "Royal" }, { 59, "Infrared" }, { 60, "Heat" }, { 61, "Violet" }, { 62, "Halcyon" }, { 63, "Gem" }, { 64, "Monochrome" }, { 65, "Sunshine" }, { 66, "Swindler" }, { 67, "C.O.D.E. Warriors" },
			{ 68, "Intensity" }, { 69, "Underadar" }, { 71, "Vintage" }, { 72, "Walker" }, { 73, "WW3" }, { 74, "Energeon" }, { 75, "Topaz" }, { 76, "Garnet" }, { 77, "Sapphire" }, { 78, "Emerald" }, { 79, "Amethyst" },
			{ 80, "Opal" }, { 81, "Overgrowth" }, { 82, "Bloody Valentine" }, { 83, "Haptic" }, { 84, "Dragon Fire" }, { 85, "Glacies Fire" }, { 86, "Atomic Fire" }, { 87, "Everlasting Fire" }, { 88, "Arcane Fire" },
			{ 89, "COD XP" }, { 90, "CWL Champions" }, { 93, "Excellence" }, { 95, "MindFreak" }, { 96, "Nv" }, { 97, "OrbitGG" }, { 98, "Tainted Minds" }, { 99, "Epsilon eSports" }, { 103, "Team Infused" }, { 104, "Team LDLC" },
			{ 105, "Millenium" }, { 106, "Splyce" }, { 107, "Supremacy" }, { 109, "Sloud9" }, { 111, "eLevate" }, { 112, "Team EnVy" }, { 113, "Faze Clan" }, { 116, "OpTic Gaming" }, { 117, "Rise Nation" }, { 118, "Loading..." },
			{ 119, "Underworld" }, { 121, "Cosmos" }, { 122, "Cosmic" }, { 123, "Infinitus" }, { 124, "Into the Void" }, { 125, "Universe" }, { 126, "Lucid" }, { 131, "Luck of the Irish" }, { 132, "Circuits" },
			{ 133, "Divinium" }, { 134, "Cherry Fizz" }, { 135, "Empire" }, { 136, "Permafrost" }, { 137, "Hive" }, { 138, "Watermelon" }
		}
	};

	struct WeaponItem {
		int16_t Optic;
		std::vector<size_t> Attachments;
		int16_t Camo;
	};

	class WeaponPreset {
	public:
		WeaponPreset(std::string_view name) : m_Name(name) { }

		std::string GetPresetName() const { return m_Name; }
		WeaponItem& GetWeaponItem(std::string_view type, size_t gun) { return m_Items[type.data()][gun]; }

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
	inline bool weaponCamoSelection = false;
	inline size_t weaponCamoMenu = 0;

	inline ImGui::FileEditor weaponFiles(ImGui::FileEditor{ "Weapon Preset", std::vector<ImGui::TextEditSelectable>{  } });
	inline ImGui::ImageSelection weaponMainWeapons;
	inline ImGui::ImageSelection weaponOpticSelection(-1);
	inline ImGui::ImageMultiSelection weaponAttachmentsSelection;

	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> weaponIconsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> opticsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> attachmentsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> camosImgList;

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