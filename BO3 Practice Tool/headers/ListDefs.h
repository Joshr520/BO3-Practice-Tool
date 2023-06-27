#pragma once

#include <string>
#include <vector>
#include <unordered_map>

inline std::vector<std::string> t7Maps = { "zm_zod", "zm_factory", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_prototype", "zm_asylum", "zm_sumpf", "zm_theater", "zm_cosmodrome", "zm_tmeple", "zm_moon", "zm_tomb"};
inline std::vector<std::string> t7MapsVerbose = { { "Shadows of Evil", "The Giant", "Der Eisendrache", "Zetsubou No Shima", "Gorod Krovi", "Revelations", "Nacht Der Untoten", "Verruckt", "Shi No Numa", "Kino Der Toten", "Ascension", "Shangri-La", "Moon", "Origins" } };

struct MapCraftables {
	int m_CraftableIndex;
	std::string m_CraftableName;
	std::vector<std::string> m_PieceNames;
};

struct CraftablesLists {
	std::unordered_map<std::string, std::vector<MapCraftables>> m_Craftables;
};

struct MapWeapons {
	std::vector<std::string> m_AR;
	std::vector<std::string> m_SMG;
	std::vector<std::string> m_LMG;
	std::vector<std::string> m_Shotgun;
	std::vector<std::string> m_Sniper;
	std::vector<std::string> m_Pistol;
	std::vector<std::string> m_Launcher;
	std::vector<std::string> m_Melee;
	std::vector<std::string> m_Special;
	std::vector<std::string> m_Equipment;
	std::vector<std::string> m_Hero;
};

struct WeaponsLists {
	std::unordered_map<std::string, MapWeapons> m_Weapons;
};

struct PerksLists {
	std::unordered_map<std::string, std::vector<std::string>> m_Perks;
};

struct PowerupLists {
	std::unordered_map<std::string, std::vector<std::string>> m_Powerups;
};

struct MapBlockers {
	int m_Index;
	std::string m_Name;
	std::vector<std::string> m_Blockers;
};

struct BlockersLists {
	std::unordered_map<std::string, std::vector<MapBlockers>> m_BlockersLists;
};

struct MapBlockersButton {
	int m_NotifyNum1;
	int m_NotifyNum2;
	int m_ListIndex;
	std::string m_Name;
};

struct BlockersButtonLists {
	std::unordered_map<std::string, std::vector<MapBlockersButton>> m_Buttons;
};

struct BGB {
	std::string m_Index;
	std::string m_Name;
	std::string m_Description;
};

struct BGBLists {
	std::vector<BGB> m_Classics;
	std::vector<BGB> m_Megas;
};

struct BGBPreset {
	std::string m_Name;
	std::vector<BGB> m_BGBs;
};

struct ValvePosition {
	std::string m_Department;
	std::string m_DragonCommand;
	std::string m_Armory;
	std::string m_Supply;
	std::string m_Infirmary;
	std::string m_Tank;
};

struct ValveSolutions {
	std::unordered_map<std::string, std::unordered_map<std::string, ValvePosition>> m_ValveSolutions1;
	std::unordered_map<std::string, std::unordered_map<std::string, ValvePosition>> m_ValveSolutions2;
};

struct AutosplitGroup {
	std::string m_Name;
	std::vector<std::string> m_Splits;
};

struct AutosplitLists {
	std::unordered_map<std::string, std::vector<AutosplitGroup>> m_AutosplitLists;
};

struct AutosplitPreset
{
	std::string m_Name;
	std::vector<std::pair<std::string, int>> m_Splits;
	bool m_IGT = false;
	bool m_IGRT = false;
	int m_NumSplits = 0;
	int m_Map = 0;
	int m_SplitType = 0;
};

struct MapPracticePatch {
	int m_Index;
	std::string m_Map;
	std::vector<std::string> m_Items;
};

struct PracticePatchLists {
	std::vector<MapPracticePatch> m_PracticePatches;
};

struct MenuWeapon {
	std::string m_Name;
	int m_NumAttachments;
	std::vector<std::string> m_Optics;
	std::vector<std::string> m_Attachments;
};

struct MenuWeaponLists {
	std::vector<std::string> m_WeaponTypes;
	std::vector<std::vector<MenuWeapon>> m_Weapons;
};

struct MenuWeaponPresetItem {
	int m_EquippedOptic;
	std::vector<int> m_EquippedAttachments;
	int m_Camo[2] = { -1, -1 };
};
struct MenuWeaponPreset {
	std::string m_Name;
	std::unordered_map<std::string, std::vector<MenuWeaponPresetItem>> m_PresetItems;
};

inline MenuWeaponLists menuWeaponLists = {
	{
		{ "Submachine Guns", "Assault Rifles", "Shotguns", "Light Machine Guns", "Sniper Rifles", "Secondaries", }
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

inline CraftablesLists craftables = {
	{
		{ "zm_zod", {
			{ 0, "Shield", { "Dolly", "Door", "Clamp" } },
			{ 1, "Civil Protector", { "Fuse 1", "Fuse 2", "Fuse 3" } },
			{ 2, "ID Gun", { "Heart", "Tentacle", "Xenomatter" } },
			{ 3, "Mementos", { "Boxer", "Detective", "Femme", "Magician" } },
			{ 4, "Gateworm", { "Boxer", "Detective", "Femme", "Magician" } }
		}},
		{ "zm_castle", {
			{ 0, "Shield", { "Dolly", "Door", "Clamp" } },
			{ 1, "Rags", { "Body", "Guards", "Handle" } }
		}},
		{ "zm_island", {
			{ 0, "Shield", { "Dolly", "Door", "Clamp" } },
			{ 1, "Gasmask", { "Visor", "Filter", "Strap" } }
		}},
		{ "zm_stalingrad", {
			{ 0, "Shield", { "Dolly", "Door", "Clamp" } }
		}},
		{ "zm_genesis", {
			{ 0, "Shield", { "Dolly", "Door", "Clamp" } },
			{ 1, "Keeper", { "Totem", "Head", "Gem" } }
		}},
		{ "zm_tomb", {
			{ 0, "Shield", { "Dolly", "Door", "Clamp" } },
			{ 1, "Maxis Drone", { "Body", "Brain", "Engine" } },
			{ 2, "Gramophone", { "Vinyl Player", "Vinyl Master", "Vinyl Wind", "Vinyl Ice", "Vinyl Fire", "Vinyl Lightning" } },
			{ 3, "Wind Staff", { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } },
			{ 4, "Fire Staff", { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } },
			{ 5, "Lightning Staff", { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } },
			{ 6, "Ice Staff", { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } }
		}}
	}
};

inline WeaponsLists weapons = {
	{ 
		{ "zm_zod", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP", "Bootlegger" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" },
			{ "Bloodhound", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife" },
			{ "Apothicon Servant", "Ray Gun" },
			{ "Li'l Arnie", "Trip Mine", "Frag Grenades", "Shield" },
			{ "Keeper Sword" }
		}},
		{ "zm_factory", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" }, { "MR6", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife" },
			{ "WunderWaffe DG-2", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_castle", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" },
			{ "MR6", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife", "Plunger"},
			{ "Wrath of the Ancients", "Lightning Bow", "Fire Bow", "Void Bow", "Wolf Bow", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades", "Shield" },
			{ "Ragnarok DG-4" }
		}},
		{ "zm_island", {
			{ "ICR-1", "HVK-30", "Man-O-War", "MX Garand", "M8A7", "Sheiva", "KN-44" },
			{ "Pharo", "Weevil", "Vesper", "Razorback", "HG 40", "Kuda", "VMP" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" },
			{ "MR6", "RK5", "L-Car 9", "Marshal 16 Dual-Wield" },
			{ "XM-53" },
			{ "Bowie Knife" },
			{ "KT-4", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades", "Shield" },
			{ "Skull of Nan Sapwe" }
		}},
		{ "zm_stalingrad", {
			{ "ICR-1", "HVK-30", "Man-O-War", "FFAR", "MX Garand", "M8A7", "Sheiva", "KN-44" },
			{ "Pharo", "Weevil", "Vesper", "HG 40", "PPSH-41", "Kuda", "VMP" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" },
			{ "MR6", "RK5", "L-Car 9" },
			{ "L4 Siege", "XM-53" },
			{ "Bowie Knife", "Malic", "Slash N' Burn", "Fury's Song", "Wrench" },
			{ "GKZ-45 Mk3", "Ray Gun", "NX ShadowClaw Dual Wield" },
			{ "Dragon Strike", "Cymbal Monkey", "Trip Mine", "Frag Grenades", "Shield" },
			{ "Gauntlet of Siegfried" }
		}},
		{ "zm_genesis", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44", "Peacekeeper MK2" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "M1927", "VMP" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon" },
			{ "Banshii", "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{"Locus", "Drakon", "SVG-100"},
			{"MR6", "Rift E9", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife", "Nightbreaker", "Buzz Cut", "Nunchucks", "Skull Splitter", "Path of Sorrows" },
			{ "Thundergun", "Apothicon Servant", "Ray Gun" },
			{ "Li'l Arnie", "Trip Mine", "Frag Grenades", "Shield" },
			{ "Ragnarok DG-4" }
		}},
		{ "zm_prototype", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44", "MX Garand", "STG-44" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP", "MP40", "Bootlegger", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "SVG-100" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife" },
			{ "Thundergun", "Ray Gun Mark II", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_asylum", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44", "MX Garand", "STG-44" },
			{ "Pharo", "Weevil", "Vesper", "PPSH-41", "Kuda", "Bootlegger", "VMP", "MP40", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "SVG-100" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife" },
			{ "Wunderwaffe DG-2", "Ray Gun Mark II", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_sumpf", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44", "MX Garand", "STG-44" },
			{ "Pharo", "Weevil", "Vesper", "PPSH-41", "Kuda", "Bootlegger", "VMP", "MP40", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "SVG-100" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Bowie Knife" },
			{ "Wunderwaffe DG-2", "Ray Gun Mark II", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_theater", {
			{ "ICR-1", "HVK-30", "Man-O-War", "FFAR", "Galil", "M8A7", "M16", "Sheiva", "KN-44", "M14" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP", "MP40", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Drakon", "SVG-100" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "XM-53" },
			{ "Sickle" },
			{ "Thundergun", "Ray Gun Mark II", "Ray Gun" },
			{ "Gersh Device", "Matryoshka Doll", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_cosmodrome", {
			{ "ICR-1", "HVK-30", "Man-O-War", "FFAR", "Galil", "M8A7", "M16", "Sheiva", "KN-44", "M14" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "L4 Siege", "XM-53" },
			{ "Bowie Knife" },
			{ "Thundergun", "Ray Gun Mark II", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_temple", {
			{ "ICR-1", "HVK-30", "Man-O-War", "FFAR", "Galil", "M8A7", "M16", "Sheiva", "KN-44", "M14" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon", "SVG-100" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "L4 Siege", "XM-53" },
			{ "Bowie Knife" },
			{ "31-79 JGb215", "Ray Gun Mark II", "Ray Gun" },
			{ "Cymbal Monkey", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_moon", {
			{ "ICR-1", "HVK-30", "Man-O-War", "FFAR", "Galil", "M8A7", "M16", "Sheiva", "KN-44", "M14" },
			{ "Pharo", "Weevil", "Vesper", "Kuda", "VMP", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon" },
			{ "MR6", "1911", "RK5", "L-Car 9" },
			{ "L4 Siege", "XM-53" },
			{ "Bowie Knife" },
			{ "Zap Gun Dual Wield", "Ray Gun Mark II", "Ray Gun" },
			{ "P.E.S.", "Hacker", "Gersh Device", "Quantum Entanglement Device", "Trip Mine", "Frag Grenades" },
			{ "Annihilator" }
		}},
		{ "zm_tomb", {
			{ "ICR-1", "HVK-30", "Man-O-War", "M8A7", "Sheiva", "KN-44", "STG-44", "M14" },
			{ "Pharo", "Weevil", "Vesper", "MP40", "Kuda", "M1927", "VMP", "AK-74U" },
			{ "Dingo", "48 Dredge", "BRM", "MG-08/15", "Gorgon", "RPK" },
			{ "Haymaker 12", "Argus", "Krm-262", "205 Brecci" },
			{ "Locus", "Drakon" },
			{ "RK5", "Mauser C96", "L-Car 9" },
			{ "L4 Siege", "XM-53" },
			{ "Bowie Knife" },
			{ "Staff of Ice", "Staff of Lightning", "Staff of Fire", "Staff of Wind", "Ray Gun Mark II", "Ray Gun" },
			{ "CymbaL Monkey", "G Strikes", "Trip Mine", "Frag Grenades", "Shield" },
			{ "Annihilator" }
		}}
	}
};

inline PerksLists perks = {
	{
		{ "zm_zod", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Widow's Wine" }
		}},
		{ "zm_factory", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri" }
		}},
		{ "zm_castle", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine", "Electric Cherry" }
		}},
		{ "zm_island", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine", "Electric Cherry" }
		}},
		{ "zm_stalingrad", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine", "Electric Cherry" }
		}},
		{ "zm_genesis", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine", "Electric Cherry" }
		}},
		{ "zm_prototype", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_asylum", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_sumpf", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_theater", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_cosmodrome", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_temple", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_moon", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine" }
		}},
		{ "zm_tomb", {
			{ "Juggernog", "Speed Cola", "Double Tap", "Quick Revive", "Stamin-Up", "Mule Kick", "Deadshot Daiquiri", "Widow's Wine", "Electric Cherry" }
		}}
	}
};

inline PowerupLists powerups = {
	{
		{ "zm_zod", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Bonus Points Team", "Widows Grenade", "Shield Charge" }
		}},
		{ "zm_factory", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player" }
		}},
		{ "zm_castle", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Bonus Points Team", "Widows Grenade", "Shield Charge", "Tram Token", "Void Symbol Tears", "Void Symbol Rib", "Void Symbol Triangles", "Void Symbol Claw", "Void Symbol Tear", "Void Symbol Ribbon" }
		}},
		{ "zm_island", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Bonus Points Team", "Widows Grenade", "Shield Charge", "Perk Slot", "Seed" }
		}},
		{ "zm_stalingrad", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade", "Shield Charge", "Tank Cylinder", "Supply Cylinder", "DC Cylinder" }
		}},
		{ "zm_genesis", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade", "Shield Charge", "Random Weapon" }
		}},
		{ "zm_prototype", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_asylum", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_sumpf", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_theater", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_cosmodrome", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_temple", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_moon", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade" }
		}},
		{ "zm_tomb", {
			{ "Max Ammo", "Double Points", "Insta Kill", "Nuke", "Death Machine", "Carpenter", "Fire Sale", "Free Perk", "Bonus Points Player", "Widows Grenade", "Shield Charge", "Zombie Blood" }
		}}
	}
};

inline BGBLists bgbs = {
	{
		{ "0", "Alchemical Antithesis", "Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon." },
		{ "1", "Always Done Swiftly", "Activates Immediately (Lasts 3 full rounds)\nWalk faster while aiming. Raise and lower your weapon to aim more quickly." },
		{ "2", "Anywhere But Here!", "Activated (2x Activations)\nInstantly teleport to a random location. A concussive blast knocks away any nearby zombies, keeping you safe." },
		{ "3", "Armamental Accomplishment", "Activates Immediately (Lasts 3 full rounds)\nSwitch weapons and recover from performing melee attacks faster. Reload and use items more quickly." },
		{ "4", "Arms Grace", "Activates Immediately (Lasts until next respawn)\nRespawn with the guns you had when you bled out." },
		{ "5", "Coagulant", "Activates Immediately (Lasts 20 minutes)\nLonger bleedout time." },
		{ "6", "Arsenal Accelerator", "Activates Immediately (Lasts 10 minutes)\nCharge your special weapon faster." },
		{ "7", "Danger Closest", "Activates Immediately (Lasts 3 full rounds)\nZero explosive damage." },
		{ "8", "Eye Candy", "Activated (4x Activations)\nOverrides the colors you see." },
		{ "9", "Firing On All Cylinders", "Activates Immediately (Lasts 3 full rounds)\nCan fire while sprinting." },
		{ "10", "Impatient", "Activates Immediately (Lasts until bleedout)\nRespawn near the end of the current round instead of at the start of the next round." },
		{ "11", "In Plain Sight", "Activated (2x Activations, 10 seconds each)\nYou are ignored by zombies for 10 seconds." },
		{ "12", "Lucky Crit", "Activates Immediately (Lasts 1 full round)\nImproves your chances of activating an Alternate Ammo Type." },
		{ "13", "Newtonian Negation", "Activates Immediately (Lasts 25 minutes)\nZombies killed fall straight up." },
		{ "14", "Now You See Me", "Activated (2x Activations)\nAll zombies will chase you for 10 seconds." },
		{ "15", "Projectile Vomiting", "Activates Immediately (Lasts 5 full rounds)\nZombies you kill with grenades and large projectiles vomit uncontrollably." },
		{ "16", "Stock Option", "Activates Immediately (Lasts 3 minutes)\nAmmo is taken from your stockpile instead of your weapon's magazine." },
		{ "17", "Sword Flay", "Activates Immediately (Lasts 2.5 minutes)\nMelee attacks deal zombies 5x as much damage." },
		{ "18", "Tone Death", "Auto-activates when killing a zombie (25x Activations)\nSilly sounds play when zombies are killed." },
	},
	{
		{ "19", "Aftertaste", "Activates Immediately (Lasts 3 full rounds)\nKeep all perks after being revived." },
		{ "20", "Board Games", "Activates Immediately (Lasts 5 full rounds)\nRepairing a board immediately repairs all boards at that window." },
		{ "21", "Board To Death", "Activates Immediately (Lasts for 5 minutes)\nRepairing a board will kill all nearby zombies." },
		{ "22", "Bullet Boost", "Activated (2x Activations)\nRe-Pack your current Pack-a-Punched gun(if supported)." },
		{ "23", "Burned Out", "Activates Immediately (Lasts two hits)\nThe next time you take melee damage, nearby zombies burst into fire." },
		{ "24", "Cache Back", "Activated (1x Activation)\nSpawns a max ammo power up." },
		{ "25", "Crate Power", "Auto-activates next time you take a gun from the magic box\nThe next gun taken from the magic box comes Pack-a-Punched." },
		{ "26", "Crawl Space", "Activated (5x Activations)\nAll nearby zombies become crawlers." },
		{ "27", "Dead Of Nuclear Winter", "Activated (2x Activations)\nSpawns a nuke power up." },
		{ "28", "Disorderly Combat", "Activates Immediately (Lasts for 5 minutes)\nGives a random gun every 10 seconds." },
		{ "29", "Ephemeral Enhancement", "Activated (2x Activations, 60 seconds each)\nTurns the weapon in your hands into Pack-a-Punched weapon." },
		{ "30", "Extra Credit", "Activated (4x Activations)\nSpawns a personal 1250 point power up." },
		{ "31", "Fatal Contraption", "Activated (2x Activations)\nSpawn a Death Machine power up." },
		{ "32", "Fear In Headlights", "Activated (1x Activation, 2 minutes)\nZombies seen by players will not move." },
		{ "33", "Flavor Hexed", "Activates Immediately (2x Activations)\nTransforms into a random Mega GobbleGum not in your Pack." },
		{ "34", "Head Drama", "Activates Immediately (Lasts for the remainder of the round)\nAny bullet which hits a zombie will damage its head." },
		{ "35", "Idle Eyes", "Activated(3x Activations, 30 seconds each)\nAll zombies will ignore all players." },
		{ "36", "I'm Feelin' Lucky", "Activated(2x Activations)\nSpawn a random power up." },
		{ "37", "Immolation Liquidation", "Activated (3x Activations)\nSpawns a fire sale power up." },
		{ "38", "Kill Joy", "Activated (2x Activations)\nSpawns an Instakill power up." },
		{ "39", "Killing Time", "Activated (1x Activation)\nAll zombies freeze in place for 20 seconds.If they are shot, they will be annihilated when the time is up." },
		{ "40", "Licensed Contractor", "Activated (3x Activations)\nSpawns a Carpenter power up." },
		{ "41", "Mind Blown", "Activated (3x Activations)\nGib the heads of all zombies you can see, killing them." },
		{ "42", "Near Death Experience", "Activates Immediately (Lasts 3 full rounds)\nRevive, or be revived, simply by being near other players. Revived players keep all of their perks." },
		{ "43", "On The House", "Activated (1x Activation)\nSpawns a free perk power up." },
		{ "44", "Perkaholic", "Activates Immediately\nGives you all perks in the map." },
		{ "45", "Phoenix Up", "Activated (1x Activation)\nRevives all teammates. Teammates keep all of their perks." },
		{ "46", "Pop Shocks", "Auto-activates when melee attacking zombies (5x Activations)\nMelee attacks trigger an electrostatic discharge, electrocuting nearby zombies." },
		{ "47", "Power Vacuum", "Activates Immediately (Lasts for 4 full rounds)\nMore power ups can drop each round." },
		{ "48", "Profit Sharing", "Activates Immediately (Lasts 10 minutes)\nPoints you earn are also received by nearby players, and vice versa." },
		{ "49", "Reign Drops", "Activated (2x Activations)\nSpawns one of each of the nine core power ups." },
		{ "50", "Respin Cycle", "Activated (2x Activations)\nRe-spins the weapons in a magic box after it has been activated." },
		{ "51", "Round Robbin'", "Activated (1x Activation)\nEnds the current round. All players gain 1600 points." },
		{ "52", "Secret Shopper", "Activates Immediately (Lasts 10 minutes)\nAny gun wall-buy can be used to buy ammo for any gun." },
		{ "53", "Self Medication", "Auto-activates by getting a kill in last stand (3x Activations)\nAuto revive yourself. Keep all of your perks." },
		{ "54", "Shopping Free", "Activates Immediately(Lasts 1 minute)\nAll purchases are free." },
		{ "55", "Slaughter Slide", "Auto-activates when sliding (6x Activations)\nCreate 2 lethal explosions by sliding." },
		{ "56", "Soda Fountain", "Auto-activates when you buy a perk (5x Activations)\nCan buy an extra perk. Gives you a free perk after you buy one." },
		{ "57", "Temporal Gift", "Activates Immediately (Lasts 1 full round)\nPower ups last longer." },
		{ "58", "Unbearable", "Auto-activates when a teddy bear appears in the magic box.\nMagic box re-spins automatically. Magic box will not move for several uses." },
		{ "59", "Undead Man Walking", "Activates Immediately (Lasts 4 minutes)\nSlow down all zombies to shambling speed." },
		{ "60", "Unquenchable", "Auto-activates when you have maximum perks.\nCan buy an extra perk." },
		{ "61", "Wall Power", "Auto-activates on your next wall-buy gun purchase\nThe next gun bought off a wall comes Pack-a-Punched." },
		{ "62", "Who's Keeping Score", "Activated (2x Activations)\nSpawns a double points power up." }
	}
};

inline ValveSolutions valves = {
	{
		{ "Department", {
			{ "Dragon Command", { "P", "1", "2", "2", "1", "1" } },
			{ "Armory", { "P", "2", "1", "3", "3", "1" } },
			{ "Supply", { "P", "3", "3", "1", "2", "3" } },
			{ "Infirmary", { "P", "2", "1", "1", "2", "3" } },
			{ "Tank", { "P", "1", "3", "2", "3", "1" } }
		}},
		{ "Dragon Command", {
			{ "Department", { "1", "P", "1", "3", "3", "1" } },
			{ "Armory", { "3", "P", "1", "3", "1", "1" } },
			{ "Supply", { "3", "P", "2", "2", "1", "1" } },
			{ "Infirmary", { "1", "P", "2", "1", "1", "2" } },
			{ "Tank", { "1", "P", "1", "1", "1", "1" } }
		}},
		{ "Armory", {
			{ "Department", { "2", "1", "P", "3", "3", "3" } },
			{ "Dragon Command", { "1", "1", "P", "3", "1", "1" } },
			{ "Supply", { "2", "2", "P", "1", "2", "3" } },
			{ "Infirmary", { "3", "1", "P", "3", "1", "3" } },
			{ "Tank", { "3", "1", "P", "2", "1", "1" } }
		}},
		{ "Supply", {
			{ "Department", { "1", "1", "2", "P", "3", "1" } },
			{ "Dragon Command", { "2", "2", "1", "P", "2", "3" } },
			{ "Armory", { "3", "1", "2", "P", "1", "1" } },
			{ "Infirmary", { "3", "1", "3", "P", "2", "3" } },
			{ "Tank", { "1", "2", "1", "P", "3", "1" } }
		}},
		{ "Infirmary", {
			{ "Department", { "1", "3", "2", "1", "P", "2" } },
			{ "Dragon Command", { "2", "1", "3", "3", "P", "3" } },
			{ "Armory", { "2", "2", "2", "1", "P", "2" } },
			{ "Supply", { "1", "2", "2", "1", "P", "1" } },
			{ "Tank", { "3", "3", "3", "2", "P", "2" } }
		}},
		{ "Tank", {
			{ "Department", { "1", "3", "1", "1", "2", "P" } },
			{ "Dragon Command", { "2", "1", "3", "2", "2", "P" } },
			{ "Armory", { "2", "2", "1", "1", "2", "P" } },
			{ "Supply", { "1", "3", "2", "1", "1", "P" } },
			{ "Infirmary", { "3", "1", "2", "2", "1", "P" } }
		}}
	},
	{
		{ "Department", {
			{ "Dragon Command", { "P", "3", "3", "2", "2", "2" } },
			{ "Armory", { "P", "3", "2", "1", "1", "2" } },
			{ "Supply", { "P", "2", "2", "2", "3", "1" } },
			{ "Infirmary", { "P", "1", "3", "3", "3", "3" } },
			{ "Tank", { "P", "3", "1", "1", "1", "3" } }
		}},
		{ "Dragon Command", {
			{ "Department", { "2", "P", "1", "1", "2", "3" } },
			{ "Armory", { "2", "P", "3", "1", "2", "2" } },
			{ "Supply", { "2", "P", "3", "3", "3", "3" } },
			{ "Infirmary", { "3", "P", "3", "2", "2", "2" } },
			{ "Tank", { "2", "P", "3", "2", "3", "2" } }
		}},
		{ "Armory", {
			{ "Department", { "3", "3", "P", "2", "2", "2" } },
			{ "Dragon Command", { "2", "2", "P", "2", "2", "2" } },
			{ "Supply", { "1", "2", "P", "3", "3", "1" } },
			{ "Infirmary", { "1", "2", "P", "1", "2", "2" } },
			{ "Tank", { "1", "3", "P", "1", "1", "2" } }
		}},
		{ "Supply", {
			{ "Department", { "3", "3", "1", "P", "2", "3" } },
			{ "Dragon Command", { "1", "3", "2", "P", "1", "2" } },
			{ "Armory", { "3", "3", "3", "P", "2", "2" } },
			{ "Infirmary", { "1", "2", "2", "P", "3", "2" } },
			{ "Tank", { "2", "1", "3", "P", "3", "3" } }
		}},
		{ "Infirmary", {
			{ "Department", { "3", "1", "2", "2", "P", "1" } },
			{ "Dragon Command", { "1", "2", "1", "3", "P", "1" } },
			{ "Armory", { "3", "1", "3", "3", "P", "1" } },
			{ "Supply", { "3", "3", "3", "3", "P", "3" } },
			{ "Tank", { "2", "2", "1", "1", "P", "3" } }
		}},
		{ "Tank", {
			{ "Department", { "2", "1", "2", "2", "3", "P" } },
			{ "Dragon Command", { "1", "3", "1", "3", "1", "P" } },
			{ "Armory", { "2", "1", "3", "3", "3", "P" } },
			{ "Supply", { "3", "3", "3", "2", "2", "P" } },
			{ "Infirmary", { "1", "2", "1", "3", "3", "P" } }
		}},
	}
};

inline BlockersLists blockers = {
	{
		{ "zm_zod", {
			{ 0, "Doors List", { "Start-Junction", "Canals-Junction", "Canals-Tram", "Canals High-Tram", "Theater-Junction", "Theater Alley-Square", "Theater High-Square", "Slums-Junction", "Slums Waterfront-Alley", "Slums High-Train", "Canals Dropdown", "Footlight Dropdown", "Slums Alley Dropdown", "Slums Waterfront Dropdown" } },
			{ 0, "Power List", { "Quick Revive", "Canals Perk", "Footlight Perk", "Waterfront Perk", "Stamin Up", "Mule Kick", "Widows Wine", "Start Stairs", "Canals Stairs", "Ruby Rabbit Stairs", "Theater Stairs", "Burlesque Door", "Slums Stairs", "Rift Door", "Junction Crane", "Canals Grate" } },
			{ 0, "Smashables List", { "Canals Statue", "Theater Statue", "Slums Statue", "Rift Statue", "Summoning Key", "Detective Badge", "Femme Hairpiece", "Boxer Belt", "Boxing Ring", "Start Powerup Door", "Canals Portal", "Theater Portal", "Slums Portal" } }
		}},
		{ "zm_factory", {
			{ 0, "Doors List", { "Outside West", "Warehouse Building", "Warehouse Second Floor", "West TP", "Outside East", "East Building", "East Loading Dock", "East TP", "South TP" } }
		}},
		{ "zm_castle", {
			{ 0, "Doors List", { "Start-Sub Clocktower", "Sub Clocktower-Courtyard", "Courtyard-Great Hall Upper", "Great Hall-Armory", "Start-Gatehouse", "Gatehouse-Lower Courtyard", "Lower Courtyard-Livingquarters", "Livingquarters-Armory", "Undercroft-Undercroft Lab", "Gatehouse Dropdown", "Great Hall Dropdown M8", "Great Hall Dropdown Speed Cola",
				"Lower Courtyard Dropdown Dragon", "Lower Courtyard Dropdown Landing Pad" } },
			{ 0, "Landing Pads", { "Landing Pad Clocktower", "Landing Pad Rocket", "Landing Pad Gatehouse", "Landing Pad Start" } }
		}},
		{ "zm_island", {
			{ 0, "Doors List", { "Start-Jungle", "Jungle-Crash Site", "Jungle-Jungle Lab", "Jungle Lab-Jungle Lab Upper", "Jungle Lab-Bunker Exterior", "Start-Swamp", "Swamp-Ruins", "Swamp-Swamp Lab", "Swamp Lab-Swamp Lab Upper", "Swamp Lab-Meteor Site", "Swamp Lab-Bunker Exterior", "Bunker Exterior-Bunker Interior", "Bunker Interior-Bunker Upper",
				"Bunker Interior-Bunker Left", "Bunker Left-Cliffside", "Bunker Interior-Bunker Right", "Bunker Interior-Operating Rooms", "Operating Rooms-Flooded Bunker", "Bunker Exterior Dropdown" } }
		}},
		{ "zm_stalingrad", {
			{ 0, "Doors List", { "Start-Department", "Department-Alley", "Department-Department Upper", "Bunker-Armory", "Bunker-Infirmary", "Department-Armory", "Department-Infirmary", "Dragon Command", "Supply", "Tank", "Supply Tram Dropdown", "Tank Bridge Dropdown" } }
		}},
		{ "zm_genesis", {
			{ 0, "Doors List", { "Start-Left", "Start-Right", "Temple-Temple Stairs", "Temple-Undercroft", "Temple-Theater", "Undercroft-Theater", "Asylum Downstairs-Upstairs", "Asylum Upstairs-Kitchen", "Cellblock-Messhall", "Ruins-Mound", "Generator-Trenches", "Prototype Start-Upstairs", "Prison Dropdown Generator", "Prison Dropdown Poster" } },
			{ 0, "Generators List", { "Mob", "Spawn", "DE", "Verruckt" } }
		}},
		{ "zm_prototype", {
			{ 0, "Doors List", { "Start-Box", "Start-Upstairs", "Box-Upstairs" } }
		}},
		{ "zm_asylum", {
			{ 0, "Doors List", { "Start Right Door", "Start Left Door", "Right Upstairs Blocker", "Right Upstairs Door", "Right Upstairs Door 2", "Box Right Door", "Left Upstairs Door", "Box Left Door" } }
		}},
		{ "zm_sumpf", {
			{ 0, "Doors List", { "Start-Downstairs", "Start-Upstairs", "Comm Room Area", "Comm Room Hut", "Storage Area", "Storage Hut", "Doc Quarters Area", "Doc Quarters Hut", "Fishing Hut Area", "Fishing Hut" } }
		}},
		{ "zm_theater", {
			{ 0, "Doors List", { "Start-Crematorium", "Crematorium-Alleyway", "Alleyway-Trap Room", "Trap Room-Stairwell", "Start-Foyer", "Foyer-Dining", "Dining-Dressing", "Dressing-Stage" } }
		}},
		{ "zm_cosmodrome", {
			{ 0, "Doors List", { "Centrifuge-Power", "Power-Centrifuge", "Centrifuge-Alley", "Base Entry-Power", "Power-Roof", "Roof-Catwalk", "Base Entry-North Path", "Catwalk-Lander", "Base Entry-Storage", "Storage-Lander" } }
		}},
		{ "zm_temple", {
			{ 0, "Doors List", { "Start-Waterfall Upper", "Waterfall-Tunnel", "Waterfall-Cave Water", "Cave Water-Power", "Start-Pressure", "Pressure-Cave 1", "Cave 1-Cave 2", "Cave 3-Power" } }
		}},
		{ "zm_moon", {
			{ 0, "Doors List", { "Start Exit", "Catacombs West", "Tunnel 6 Enter", "Tunnel 6 Exit", "Catacombs East", "Tunnel 11 Enter 1", "Tunnel 11 Enter 2", "Tunnel 11 Exit", "Power Exit", "Dig Enter East", "Biodome Enter East", "Dig Exit East", "Biodome Enter West" } }
		}},
		{ "zm_tomb", {
			{ 0, "Doors List", { "Start-Gen 3 Trench", "Gen 3 Trench-Gen 3", "Gen 3-Workshop", "Start-Gen 2 Trench", "Gen 2 Trench-Gen 2", "Gen 2-Workshop", "Workshop-NML", "NML-Mound", "NML-Gen 5", "NML-Tank Station", "Tank Station-Church" } },
			{ 0, "Generators List", { "Gen 1", "Gen 2", "Gen 3", "Gen 4", "Gen 5", "Gen 6" } }
		}}
	}
};

inline BlockersButtonLists blockersButtons = {
	{
		{ "zm_zod", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 3, -1, 1, "Activate Selected Power" },
			{ 4, -1, 2, "Smash Selected Smashable" }
		}},
		{ "zm_factory", {
			{ 1, -1, 0, "Open Selected Door" }
		}},
		{ "zm_castle", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 4, -1, 1, "Activate Selected Landing Pad" }
		}},
		{ "zm_island", {
			{ 1, -1, 0, "Open Selected Door" }
		}},
		{ "zm_stalingrad", {
			{ 1, -1, 0, "Open Selected Door" }
		}},
		{ "zm_genesis", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 3, -1, 1, "Activate Selected Gen" },
			{ 4, 0, -1, "Trap Apothicon" }
		}},
		{ "zm_prototype", {
			{ 1, -1, 0, "Open Selected Door" }
		}},
		{ "zm_asylum", {
			{ 1, -1, 0, "Open Selected Door" }
		}},
		{ "zm_sumpf", {
			{ 1, -1, 0, "Open Selected Door" }
		}},
		{ "zm_theater", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 4, 0, -1, "Link TP" }
		}},
		{ "zm_cosmodrome", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 4, 0, -1, "Open PAP" }
		}},
		{ "zm_temple", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 4, 0, -1, "Open PAP" }
		}},
		{ "zm_moon", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 4, 0, -1, "Open TP Cage" }
		}},
		{ "zm_tomb", {
			{ 1, -1, 0, "Open Selected Door" },
			{ 3, -1, 1, "Activate Selected Gen" }
		}}
	}
};

inline AutosplitLists autosplits = {
	{
		{ "zm_zod", {
			{ "Ritual", { "Magician Ritual", "Femme Ritual", "Detective Ritual", "Boxer Ritual", "PAP Ritual" } },
			{ "Rift", { "Canals - Rift", "Footlight - Rift", "Waterfront - Rift", "Rift - Canals", "Rift - Footlight", "Rift - Waterfront" } },
			{ "SOE Egg", { "Canals Egg", "Footlight Egg", "Waterfront Egg", "Rift Egg", "Pickup Sword" } },
			{ "Ovum", { "Junction Ovum", "Canals Ovum", "Footlight Ovum", "Waterfront Ovum" } },
			{ "Flag", { "Pickup Upgraded Sword", "Activate Book", "Flag 1", "Flag 2", "Flag 3", "Flag 4" } }
		}},
		{ "zm_factory", {

		}},
		{ "zm_castle", {
			{ "Dragon", { "Church Dragon", "Courtyard Dragon", "Undercroft Dragon", "Pickup Bow" } },
			{ "Lightning Bow", { "Start Lightning", "Bonfires Shot", "Wall Ride", "Crackle", "Upgrade Lightning" } },
			{ "Fire Bow", { "Start Fire", "Obelisk Shot", "Circles Filled", "Golf", "Upgrade Fire" } },
			{ "Void Bow", { "Start Void", "Activate Urn", "Pickup Skulls", "Crawler Kills", "Runes", "Upgrade Void" } },
			{ "Wolf Bow", { "Start Wolf", "Shrine Shot", "Start Escort", "Wolf Souls Filled", "Arrow Forged", "Upgrade Wolf" } },
			{ "Wisp", { "Normal TP", "Time Travel 1", "Time Travel 2" } },
			{ "Simon", { "Safe Code Entered", "Simon 1", "Simon 2" } },
			{ "Keeper", { "Keeper Spawned", "Keeper 1", "Keeper 2", "Keeper 3", "Keeper 4", "Keeper Trapped" } },
			{ "DE Boss", { "DE Boss Enter", "Boss Exit" } }
		}},
		{ "zm_island", {
			{ "Skull", { "Skull 1", "Skull 2", "Skull 3", "Skull 4", "Skull Ritual" } },
			{ "Blockers", { "Bunker Open", "Power On" } },
			{ "WW", { "KT-4", "Masamune" } },
			{ "ZNS Main EE", { "Poster", "Bullet", "Plane Shot", "Elevator On", "ZNS Boss Enter" } }
		}},
		{ "zm_stalingrad", {
			{ "PAP", { "Groph 1", "Groph 2", "Groph 3" } },
			{ "Gauntlet", { "Pickup Egg", "Egg On Fire", "Incubation Start", "Pickup Gauntlet" } },
			{ "Dragon", { "Fly Supply", "Fly Tank", "Fly DC" } },
			{ "Lockdown", { "Lockdown Start", "Lockdown End" } },
			{ "Challenge", { "Start Challenges", "Start Download", "GK Boss Enter" } }
		}},
		{ "zm_genesis", {
			{ "Rev EE Start", { "Spawn Gen", "DE Gen", "Verruckt Gen", "Mob Gen", "Keeper Start" } },
			{ "Apothicon", { "Enter Beast", "Exit Beast Mob", "Exit Beast Verruckt", "Exit Beast DE", "Exit Beast Spawn" } },
			{ "Reel", { "Pickup Reel 1", "Place Reel 1", "Pickup Reel 2", "Place Reel 2", "Pickup Reel 3", "Place Reel 3" } },
			{ "Rev Egg", { "Sophia TP", "Egg 1", "Egg 2", "Egg 3", "Egg 4" } },
			{ "Rune", { "Rune 1", "Rune 2", "Rune 3", "Rune 4", "Boss 1 Enter" } },
			{ "Rev EE End", { "Symbols", "Basketball", "Boss 2" } }
		}},
		{ "zm_prototype", {

		}},
		{ "zm_asylum", {

		}},
		{ "zm_sumpf", {

		}},
		{ "zm_theater", {

		}},
		{ "zm_cosmodrome", {

		}},
		{ "zm_temple", {

		}},
		{ "zm_moon", {

		}},
		{ "zm_tomb", {
			{ "Staff", { "Ice Craft", "Wind Craft", "Fire Craft", "Lightning Craft" } },
			{ "Origins EE End", { "Upgrade Leave", "4 Boxes Done", "Fists Done" } }
		}}
	}
};

inline PracticePatchLists practicePatches = {
	{
		{ 0, " Shadows of Evil", { "None", "Soft Patch", "Infinite Beast", "Ovums Practice", "Flags Practice" } },
		{ 0, " The Giant", { "None", "Flytrap Practice" } },
		{ 0, " Der Eisendrache", { "None", "Soft Patch", "Wisp 2 Practice", "Keeper Practice", "Lightning Bow Shots", "Fire Bow Shots" } },
		{ 0, " Zetsubou No Shima", { "None", "Soft Patch" } },
		{ 0, " Gorod Krovi", { "None", "Soft Patch", "Lockdown Practice", "Challenges Practice", "Gersh Quote Skips", "Boss Quote Skip" } },
		{ 0, " Revelations", { "None", "Soft Patch", "Bones Practice", "Boss 1 Practice", "Boss 2 Practice", "Basketball Practice", "Squid Shards Practice" } },
		{ 0, " Moon", { "None" } },
		{ 0, " Origins", { "None", "Soft Patch", "Wind Orb Practice", "Puzzle Practice", "Temp Lineups", "Rain Fire Lineups", "Rain Fire Panzers" } },
		{ 0, " General", { "None", "Spawner Debug"} }
	}
};