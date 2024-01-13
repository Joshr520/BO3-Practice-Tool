#pragma once
#include "ImGui/ImGuiExtendedWidgets.h"
#include "Walnut/Image.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace BO3PracticeTool
{
	struct Attachment {
		std::string Name;
		std::string InternalName;

		bool operator==(std::string_view name) const
		{
			return Name == name || InternalName == name;
		}
	};

	struct Weapon {
		std::string Name;
		std::string InternalName;
		size_t NumAttachments;
		std::vector<Attachment> Optics;
		std::vector<Attachment> Attachments;
	};

	struct WeaponItem {
		int8_t Optic = -1;
		std::vector<size_t> Attachments;
		int16_t Camo = -1;
	};

	class WeaponPreset {
	public:
		WeaponPreset(std::string_view name) : m_Name(name) { }

		std::string_view GetPresetName() const { return m_Name; }
		WeaponItem& GetWeaponItem(std::string_view type, size_t gun) { return m_Items[type.data()][gun]; }
		static WeaponPreset* GetCurrentPreset() { return s_CurrentPreset; }

		void SetPresetName(std::string_view name) { m_Name = name; }

		void SavePreset();
		static void SavePreset(std::string_view name);
		static void WritePresetToGame(bool reset = false);

		static void LoadWeaponPresets();
		static void AddWeaponPreset(std::string_view name);
		static void DeleteWeaponPreset(std::string_view name);
		static void WeaponPresetIndexChanged(size_t index) { s_CurrentPreset = &s_Presets[index]; }
		static void EditedWeaponPreset(std::string_view oldName, std::string_view newName);

		static void ResetWeaponSelections();

		bool operator==(const WeaponPreset& preset) { return m_Name == preset.m_Name; }
	private:
		void AddWeapons(std::string_view type, std::vector<WeaponItem> items) { m_Items.insert({ type.data(), items }); }
	private:
		std::string m_Name;
		std::unordered_map<std::string, std::vector<WeaponItem>> m_Items = { };

		static std::vector<WeaponPreset> s_Presets;
		static WeaponPreset* s_CurrentPreset;
	};

	void LoadWeaponImages();
	void UnloadWeaponImages();
	void LoadWeaponSelections();

	// Attachments
	inline Attachment reflex = { "Reflex", "reflex" };
	inline Attachment elo = { "ELO", "reddot" };
	inline Attachment thermal = { "Thermal", "ir" };
	inline Attachment varix = { "Varix 3", "dualoptic" };
	inline Attachment boa = { "BOA 3", "holo" };
	inline Attachment recon = { "Recon", "acog" };
	inline Attachment longBarrel = { "Long Barrel", "extbarrel" };
	inline Attachment fastMags = { "Fast Mags", "fastreload" };
	inline Attachment quickdraw = { "Quickdraw", "quickdraw" };
	inline Attachment laserSight = { "Laser Sight", "steadyaim" };
	inline Attachment stock = { "Stock", "stalker" };
	inline Attachment rapidFire = { "Rapid Fire", "rf" };
	inline Attachment grip = { "Grip", "grip" };
	inline Attachment ballisticsCPU = { "Ballistics CPU", "swayreduc" };

	inline std::unordered_map<std::string, size_t> weaponAttachmentToIndex = {
		{ "reflex", 0 }, { "reddot", 1}, { "ir", 2}, { "dualoptic", 3}, { "holo", 4}, { "acog", 5}, { "extbarrel", 6}, { "fastreload", 7},
		{ "quickdraw", 8 }, { "steadyaim", 9 }, { "stalker", 10 }, { "rf", 11 }, { "grip", 12 }, { "swayreduc", 13 }
	};

	inline std::unordered_map<size_t, std::string> weaponIndexToAttachment = {
		{ 0, "reflex" }, { 1, "reddot" }, { 2, "ir" }, { 3, "dualoptic" }, { 4, "holo" }, { 5, "acog" }, { 6, "extbarrel" }, { 7, "fastreload" },
		{ 8, "quickdraw" }, { 9, "steadyaim" }, { 10, "stalker" }, { 11, "rf" }, { 12, "grip" }, { 13, "swayreduc" }
	};


	// SMGs
	inline Weapon vesper = { "Vesper", "smg_fastfire", 5, { reflex, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon vmp = { "VMP", "smg_versatile", 5, { reflex, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon kuda = { "Kuda", "smg_standard", 5, { reflex, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon pharo = { "Pharo", "smg_burst", 5, { reflex, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon weevil = { "Weevil", "smg_capacity", 5, { reflex, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon bootlegger = { "Bootlegger", "smg_sten", 0 };
	inline Weapon hg40 = { "HG 40", "smg_mp40", 0 };
	inline Weapon razorback = { "Razorback", "smg_longrange", 0 };
	inline Weapon ppsh = { "PPSh-41", "smg_ppsh", 0 };
	inline Weapon thompson = { "M1927", "smg_thompson", 0 };
	inline Weapon ak74 = { "AK-74u", "smg_ak74u", 0 };
	inline Weapon mp40 = { "MP40", "smg_mp40_1940", 0 };
	// ARs
	inline Weapon icr = { "ICR-1", "ar_accurate", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon kn44 = { "KN-44", "ar_standard", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon m8 = { "M8A7", "ar_longburst", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon sheiva = { "Sheiva", "ar_marksman", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon hvk = { "HVK-30", "ar_cqb", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon manOWar = { "Man-O-War", "ar_damage", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, longBarrel, fastMags } };
	inline Weapon mxGarand = { "MX Garand", "ar_garand", 0 };
	inline Weapon ffar = { "FFAR", "ar_famas", 0 };
	inline Weapon peacekepper = { "Peacekeeper MK2", "ar_peacekeeper", 0 };
	inline Weapon stg = { "StG-44", "ar_stg44", 0 };
	inline Weapon m14 = { "M14", "ar_m14", 0 };
	inline Weapon m16 = { "M16", "ar_m16", 0 };
	inline Weapon galil = { "Galil", "ar_galil", 0 };
	// Shotguns
	inline Weapon krm = { "KRM-262", "shotgun_pump", 5, { reflex, elo, boa }, { quickdraw, stock, laserSight, rapidFire, longBarrel, fastMags } };
	inline Weapon argus = { "Argus", "shotgun_precision", 5, { reflex, elo, boa }, { quickdraw, stock, laserSight, rapidFire, longBarrel, fastMags } };
	inline Weapon brecci = { "205 Brecci", "shotgun_semiauto", 5, { reflex, elo, boa }, { quickdraw, stock, laserSight, rapidFire, longBarrel, fastMags } };
	inline Weapon haymaker = { "Haymaker 12", "shotgun_fullauto", 5, { reflex, elo, boa }, { quickdraw, stock, laserSight, rapidFire, longBarrel, fastMags } };
	inline Weapon banshii = { "Banshii", "shotgun_energy", 0 };
	// LMGs
	inline Weapon brm = { "BRM", "lmg_light", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, fastMags } };
	inline Weapon dingo = { "Dingo", "lmg_cqb", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, fastMags } };
	inline Weapon dredge = { "48 Dredge", "lmg_heavy", 5, { reflex, thermal, varix, elo, boa, recon }, { quickdraw, stock, laserSight, rapidFire, grip, fastMags } };
	inline Weapon gorgon = { "Gorgon", "lmg_slowfire", 5, {reflex, thermal, varix, elo, boa, recon}, {quickdraw, stock, laserSight, rapidFire, grip, fastMags} };
	inline Weapon rpk = { "RPK", "lmg_rpk", 0 };
	inline Weapon mg08 = { "MG-08/15", "lmg_mg08", 0 };
	// Snipers
	inline Weapon locus = { "Locus", "sniper_fastbolt", 4, { elo, thermal, varix, recon }, { ballisticsCPU, stock, rapidFire, fastMags } };
	inline Weapon drakon = { "Drakon", "sniper_fastsemi", 4, { elo, thermal, varix, recon }, { ballisticsCPU, stock, rapidFire, fastMags } };
	inline Weapon svg = { "SVG-100", "sniper_powerbolt", 4, { elo, thermal, varix, recon }, { ballisticsCPU, stock, rapidFire, fastMags } };
	// Secondaries
	inline Weapon rk5 = { "RK5", "pistol_burst", 2, { reflex, elo }, { quickdraw, laserSight, longBarrel, fastMags } };
	inline Weapon lcar = { "L-CAR 9", "pistol_fullauto", 2, { reflex, elo }, { quickdraw, laserSight, longBarrel, fastMags } };
	inline Weapon bloodhound = { "Bloodhound", "pistol_revolver38", 0 };
	inline Weapon mr6 = { "MR6", "pistol_standard", 0 };
	inline Weapon marshal = { "Marshal 16 Dual-Wield", "pistol_shotgun_dw", 0 };
	inline Weapon crossbow = { "NX ShadowClaw Dual Wield", "special_crossbow_dw", 0 };
	inline Weapon riftE9 = { "Rift E9", "pistol_energy", 0 };
	inline Weapon m1911 = { "1911", "pistol_m1911", 0 };
	inline Weapon mauser = { "Mauser C96", "pistol_c96", 0 };
	inline Weapon xm53 = { "XM-53", "launcher_standard", 0 };
	inline Weapon l4Siege = { "L4 Siege", "launcher_multi", 0 };
	inline Weapon bowieKnife = { "Bowie Knife", "bowie_knife", 0 };
	inline Weapon sickle = { "Sickle", "sickle_knife", 0 };
	inline Weapon wrench = { "Wrench", "mselee_wrench", 0 };
	inline Weapon sword = { "Fury's Song", "melee_sword", 0 };
	inline Weapon fireaxe = { "Slash N' Burn", "melee_fireaxe", 0 };
	inline Weapon dagger = { "Malice", "melee_dagger", 0 };
	inline Weapon katana = { "Path of Sorrows", "melee_katana", 0 };
	inline Weapon mace = { "Skull Splitter", "melee_mace", 0 };
	inline Weapon nunchuks = { "Nunchucks", "melee_nunchuks", 0 };
	inline Weapon improvise = { "Buzz Cut", "melee_improvise", 0 };
	inline Weapon boneglass = { "Nightbreaker", "melee_boneglass", 0 };
	inline Weapon fists = { "One Inch Punch", "one_inch_punch", 0 };
	// Special Weapons
	inline Weapon raygun = { "Ray Gun", "ray_gun", 0 };
	inline Weapon raygunmk2 = { "Ray Gun Mark II", "raygun_mark2", 0 };
	inline Weapon raygunmk3 = { "GKZ-45 Mk3", "raygun_mark3", 0 };
	inline Weapon wunderwaffe = { "Wunderwaffe DG-2", "tesla_gun", 0 };
	inline Weapon apothicon = { "Apothicon Servant", "idgun_", 0 };
	inline Weapon elementalBow = { "Wrath of the Ancients", "elemental_bow", 0 };
	inline Weapon stormBow = { "Storm Bow", "elemental_bow_storm", 0 };
	inline Weapon fireBow = { "Fire Bow", "elemental_bow_rune_prison", 0 };
	inline Weapon voidBow = { "Void Bow", "elemental_bow_demongate", 0 };
	inline Weapon wolfBow = { "Wolf Bow", "elemental_bow_wolf_howl", 0 };
	inline Weapon kt4 = { "KT-4", "hero_mirg2000", 0 };
	inline Weapon thundergun = { "Thundergun", "thundergun", 0 };
	inline Weapon babygun = { "31-79 JGb215", "shrink_ray", 0 };
	inline Weapon wavegun = { "Zap Gun Dual Wield", "microwavegundw", 0 };
	inline Weapon iceStaff = { "Ice Staff", "staff_water", 0 };
	inline Weapon fireStaff = { "Fire Staff", "staff_fire", 0 };
	inline Weapon lightningStaff = { "Lightning Staff", "staff_lightning", 0 };
	inline Weapon windStaff = { "Wind Staff", "staff_air", 0 };
	// Equipment
	inline Weapon lilArnie = { "Li'l Arnie", "octobomb", 0 };
	inline Weapon tripMine = { "Trip Mine", "bouncingbetty", 0 };
	inline Weapon grenades = { "Grenades", "frag_grenade", 0 };
	inline Weapon shield = { "Shield", "shield", 0 };
	inline Weapon monkey = { "Cymbal Monkey", "cymbal_monkey", 0 };
	inline Weapon dragonStrike = { "Dragon Strike", "launcher_dragon_strike", 0 };
	inline Weapon gersh = { "Gersh Device", "black_hole_bomb", 0 };
	inline Weapon dolls = { "Matryoshka Doll", "nesting_dolls", 0 };
	inline Weapon qed = { "Quantum Entanglement Device", "quantum_bomb", 0 };
	inline Weapon gstrike = { "G Strikes", "beacon", 0 };
	inline Weapon keeperSword = { "Keeper Sword", "keeper_sword", 0 };
	inline Weapon annihilator = { "Annihilator", "hero_annihilator", 0 };
	inline Weapon rags = { "Ragnarok DG-4", "hero_gravityspikes_melee", 0 };
	inline Weapon skull = { "Skull of Nan Sapwe", "skull_gun", 0 };
	inline Weapon gauntlet = { "Gauntlet of Siegfried", "dragon_gauntlet_flamethrower", 0 };
	inline Weapon pes = { "P.E.S.", "equip_gasmask", 0 };
	inline Weapon hacker = { "Hacker", "equip_hacker", 0 };
	// Wearables
	inline Weapon margwaHeadSoe = { "Margwa Head", "margwa_head_zod", 0 };
	inline Weapon clawHelmet = { "Claw Helmet", "claw_helmet", 0 };
	inline Weapon dragonWings = { "Dragon Wings", "dragon_wings", 0 };
	inline Weapon manglerHelmet = { "Mangler Helmet", "raz_hat", 0 };
	inline Weapon valkyrieHat = { "Valkyrie Hat", "sentinel_hat", 0 };
	inline Weapon alsHat = { "Al's Hat", "s_weasels_hat", 0 };
	inline Weapon helmOfSiegfried = { "Helm of Siegfried", "s_helm_of_siegfried", 0 };
	inline Weapon helmOfTheKing = { "Helm of the King", "s_helm_of_the_king", 0 };
	inline Weapon direWolfHead = { "Dire Wolf Head", "s_dire_wolf_head", 0 };
	inline Weapon keeperSkull = { "Keeper Skull Head", "s_keeper_skull_head", 0 };
	inline Weapon margwHeadRev = { "Margwa Head", "s_margwa_head", 0 };
	inline Weapon apothiconMask = { "Apothicon Mask", "s_apothicon_mask", 0 };
	inline Weapon furyHead = { "Fury Head", "s_fury_head", 0 };
	inline Weapon goldenShovel = { "Golden Shovel", "golden_shovel", 0 };
	inline Weapon goldenHelmet = { "Golden Helmet", "golden_helmet", 0 };

	inline size_t weaponPresetsMenu = 0;
	inline size_t weaponBeingEdited = 0;
	inline size_t weaponCamoMenu = 0;
	inline bool weaponCamoSelection = false;
	inline bool weaponPresetsActive = false;
	inline std::vector<std::string> weaponTypes = { "Submachine Guns", "Assault Rifles", "Shotguns", "Light Machine Guns", "Sniper Rifles", "Secondaries", "Special Weapons", "Equipment", "Wearables" };
	inline std::vector<std::vector<Weapon>> menuWeapons = {
		{ vesper, vmp, kuda, pharo, weevil },
		{ icr, kn44, m8, sheiva, hvk, manOWar },
		{ krm, argus, brecci, haymaker },
		{ brm, dingo, dredge, gorgon },
		{ locus, drakon, svg },
		{ bowieKnife, rk5, lcar, xm53 }
	};
	inline std::unordered_map<std::string, std::vector<std::vector<Weapon>>> mapWeapons = {
		{
			{ "zm_zod", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, apothicon, keeperSword },
				{ grenades, tripMine, lilArnie, shield },
				{ margwaHeadSoe }
			}},
			{ "zm_factory", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, wunderwaffe, annihilator },
				{ grenades, tripMine, monkey }
			}},
			{ "zm_castle", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, elementalBow, stormBow, fireBow, voidBow, wolfBow, rags },
				{ grenades, tripMine, monkey, shield },
				{ clawHelmet }
			}},
			{ "zm_island", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, kt4, skull },
				{ grenades, tripMine, monkey, shield }
			}},
			{ "zm_stalingrad", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, crossbow, bowieKnife, dagger, fireaxe, sword, wrench },
				{ raygun, raygunmk3, gauntlet },
				{ grenades, tripMine, monkey, shield },
				{ dragonWings, manglerHelmet, valkyrieHat }
			}},
			{ "zm_genesis", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife, boneglass, improvise, katana, mace, nunchuks },
				{ raygun, apothicon, thundergun, rags },
				{ grenades, tripMine, lilArnie, shield },
				{ alsHat, helmOfSiegfried, helmOfTheKing, direWolfHead, margwHeadRev, keeperSkull, apothiconMask, furyHead }
			}},
			{ "zm_prototype", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, raygunmk2, thundergun, annihilator },
				{ grenades, monkey }
			}},
			{ "zm_asylum", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, raygunmk2, wunderwaffe, annihilator },
				{ grenades, tripMine, monkey }
			}},
			{ "zm_sumpf", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, raygunmk2, wunderwaffe, annihilator },
				{ grenades, tripMine, monkey }
			}},
			{ "zm_theater", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, raygunmk2, thundergun, annihilator },
				{ grenades, tripMine, monkey }
			}},
			{ "zm_cosmodrome", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, sickle },
				{ raygun, raygunmk2, thundergun, annihilator },
				{ grenades, tripMine, gersh, dolls }
			}},
			{ "zm_temple", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon, svg },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, raygunmk2, babygun, annihilator },
				{ grenades, tripMine, monkey }
			}},
			{ "zm_moon", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon },
				{ rk5, lcar, xm53, bowieKnife },
				{ raygun, raygunmk2, wavegun, annihilator },
				{ grenades, tripMine, gersh, qed, pes, hacker }
			}},
			{ "zm_tomb", {
				{ vesper, vmp, kuda, pharo, weevil },
				{ icr, kn44, m8, sheiva, hvk, manOWar },
				{ krm, argus, brecci, haymaker },
				{ brm, dingo, dredge, gorgon },
				{ locus, drakon },
				{ rk5, lcar, xm53, bowieKnife, fists },
				{ raygun, raygunmk2, iceStaff, fireStaff, lightningStaff, windStaff, annihilator },
				{ grenades, tripMine, gstrike, monkey, shield },
				{ goldenShovel, goldenHelmet }
			}}
		}
	};
	inline std::vector<std::string> camoTypes = { "Campaign", "Multiplayer", "Zombies", "Black Market", "Extras" };
	inline std::vector<std::vector<std::string>> camoList = {
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
	};
	inline ImGui::FileEditor weaponFiles(ImGui::FileEditor{ "Weapon Preset", std::vector<ImGui::TextEditSelectable>{  } });
	inline ImGui::ImageSelection weaponMainWeapons;
	inline ImGui::ImageSelection weaponOpticSelection(-1);
	inline ImGui::ImageMultiSelection weaponAttachmentsSelection;
	inline std::unordered_map<std::string, ImGui::MultiSelection> mapWeaponsSelection;
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
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> weaponIconsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> opticsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> attachmentsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> camosImgList;
}