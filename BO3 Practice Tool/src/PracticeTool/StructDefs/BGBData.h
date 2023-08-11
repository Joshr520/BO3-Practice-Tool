#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "ImGui/ImGuiExtendedWidgets.h"
#include "Walnut/Image.h"

namespace BO3PracticeTool
{
	typedef enum BGBRarities : uint8_t {
		Classic, Chronicles, DLC3, DLC4, Mega, RareMega, UltraRareMega
	} BGBRarity;

	typedef enum BGBColors : uint8_t {
		Blue, Yellow, Green, Purple
	} BGBColor;

	struct BGB {
		std::string Name;
		BGBRarity Rarity;
		BGBColor Color;
		std::string Category;
		std::string Description;
	};

	typedef enum BGBFilters : uint8_t {
		Alphabetical, Rarity, Color
	} BGBFilter;

	inline BGBFilter bgbSelectionFilter = Alphabetical;

	struct BGBImage {
		BGB BGBData;
		std::shared_ptr<Walnut::Image> Image;

		bool operator<(const BGBImage& bgbImage) const
		{
			if (bgbSelectionFilter == Alphabetical) {
				return BGBData.Name < bgbImage.BGBData.Name;
			}
			else if (bgbSelectionFilter == Rarity) {
				return BGBData.Rarity <= bgbImage.BGBData.Rarity && BGBData.Name < bgbImage.BGBData.Name;
			}
			else {
				return BGBData.Color <= bgbImage.BGBData.Color && BGBData.Name < bgbImage.BGBData.Name;
			}
		}
	};

	inline bool CompareImagePointer(const std::shared_ptr<BGBImage>& first, const std::shared_ptr<BGBImage>& second)
	{
		if (bgbSelectionFilter == Alphabetical) {
			return first->BGBData.Name < second->BGBData.Name;
		}
		else if (bgbSelectionFilter == Rarity) {
			if (first->BGBData.Rarity != second->BGBData.Rarity) {
				return first->BGBData.Rarity < second->BGBData.Rarity;
			}
			return first->BGBData.Name < second->BGBData.Name;
		}
		else {
			if (first->BGBData.Color != second->BGBData.Color) {
				return first->BGBData.Color < second->BGBData.Color;
			}
			return first->BGBData.Name < second->BGBData.Name;
		}
	}

	inline std::vector<BGB> bgbs = {
		{
			{ "Alchemical Antithesis", 		Classic,		Purple,		"Classic",	"Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon." },
			{ "Always Done Swiftly", 		Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nWalk faster while aiming. Raise and lower your weapon to aim more quickly." },
			{ "Anywhere But Here!", 		Classic,		Purple,		"Classic",	"Activated (2x Activations)\nInstantly teleport to a random location. A concussive blast knocks away any nearby zombies, keeping you safe." },
			{ "Armamental Accomplishment", 	Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nSwitch weapons and recover from performing melee attacks faster. Reload and use items more quickly." },
			{ "Arms Grace", 				Classic,		Yellow,		"Classic",	"Activates Immediately (Lasts until next respawn)\nRespawn with the guns you had when you bled out." },
			{ "Arsenal Accelerator", 		Classic,		Green,		"Classic",	"Activates Immediately (Lasts 10 minutes)\nCharge your special weapon faster." },
			{ "Coagulant", 					Classic,		Green,		"Classic",	"Activates Immediately (Lasts 20 minutes)\nLonger bleedout time." },
			{ "Danger Closest", 			Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nZero explosive damage." },
			{ "Eye Candy", 					Chronicles,		Purple,		"Classic",	"Activated (4x Activations)\nOverrides the colors you see." },
			{ "Firing On All Cylinders", 	Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nCan fire while sprinting." },
			{ "Impatient", 					Classic,		Yellow,		"Classic",	"Activates Immediately (Lasts until bleedout)\nRespawn near the end of the current round instead of at the start of the next round." },
			{ "In Plain Sight", 			Classic,		Purple,		"Classic",	"Activated (2x Activations, 10 seconds each)\nYou are ignored by zombies for 10 seconds." },
			{ "Lucky Crit", 				Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 1 full round)\nImproves your chances of activating an Alternate Ammo Type." },
			{ "Newtonian Negation", 		DLC4,			Green,		"Classic",	"Activates Immediately (Lasts 25 minutes)\nZombies killed fall straight up." },
			{ "Now You See Me", 			Classic,		Purple,		"Classic",	"Activated (2x Activations)\nAll zombies will chase you for 10 seconds." },
			{ "Projectile Vomiting", 		DLC3,			Blue,		"Classic",	"Activates Immediately (Lasts 5 full rounds)\nZombies you kill with grenades and large projectiles vomit uncontrollably." },
			{ "Stock Option", 				Classic,		Green,		"Classic",	"Activates Immediately (Lasts 3 minutes)\nAmmo is taken from your stockpile instead of your weapon's magazine." },
			{ "Sword Flay", 				Classic,		Green,		"Classic",	"Activates Immediately (Lasts 2.5 minutes)\nMelee attacks deal zombies 5x as much damage." },
			{ "Tone Death", 				Chronicles,		Yellow,		"Classic",	"Auto-activates when killing a zombie (25x Activations)\nSilly sounds play when zombies are killed." },
			{ "Aftertaste", 				Mega,			Blue,		"Mega",		"Activates Immediately (Lasts 3 full rounds)\nKeep all perks after being revived." },
			{ "Board Games", 				Mega,			Blue,		"Mega",		"Activates Immediately (Lasts 5 full rounds)\nRepairing a board immediately repairs all boards at that window." },
			{ "Board To Death", 			Mega,			Green,		"Mega",		"Activates Immediately (Lasts for 5 minutes)\nRepairing a board will kill all nearby zombies." },
			{ "Bullet Boost", 				RareMega,		Purple,		"Mega",		"Activated (2x Activations)\nRe-Pack your current Pack-a-Punched gun(if supported)." },
			{ "Burned Out", 				Mega,			Yellow,		"Mega",		"Activates Immediately (Lasts two hits)\nThe next time you take melee damage, nearby zombies burst into fire." },
			{ "Cache Back", 				RareMega,		Purple,		"Mega",		"Activated (1x Activation)\nSpawns a max ammo power up." },
			{ "Crate Power", 				RareMega,		Yellow,		"Mega",		"Auto-activates next time you take a gun from the magic box\nThe next gun taken from the magic box comes Pack-a-Punched." },
			{ "Crawl Space", 				Mega,			Purple,		"Mega",		"Activated (5x Activations)\nAll nearby zombies become crawlers." },
			{ "Dead Of Nuclear Winter",		Mega,			Purple,		"Mega",		"Activated (2x Activations)\nSpawns a nuke power up." },
			{ "Disorderly Combat", 			Mega,			Green,		"Mega",		"Activates Immediately (Lasts for 5 minutes)\nGives a random gun every 10 seconds." },
			{ "Ephemeral Enhancement", 		Mega,			Purple,		"Mega",		"Activated (2x Activations, 60 seconds each)\nTurns the weapon in your hands into Pack-a-Punched weapon." },
			{ "Extra Credit", 				RareMega,		Purple,		"Mega",		"Activated (4x Activations)\nSpawns a personal 1250 point power up." },
			{ "Fatal Contraption", 			Mega,			Purple,		"Mega",		"Activated (2x Activations)\nSpawn a Death Machine power up." },
			{ "Fear In Headlights", 		RareMega,		Purple,		"Mega",		"Activated (1x Activation, 2 minutes)\nZombies seen by players will not move." },
			{ "Flavor Hexed", 				Mega,			Yellow,		"Mega",		"Activates Immediately (2x Activations)\nTransforms into a random Mega GobbleGum not in your Pack." },
			{ "Head Drama", 				UltraRareMega,	Blue,		"Mega",		"Activates Immediately (Lasts for the remainder of the round)\nAny bullet which hits a zombie will damage its head." },
			{ "Idle Eyes", 					Mega,			Purple,		"Mega",		"Activated(3x Activations, 30 seconds each)\nAll zombies will ignore all players." },
			{ "I'm Feelin' Lucky", 			Mega,			Purple,		"Mega",		"Activated(2x Activations)\nSpawn a random power up." },
			{ "Immolation Liquidation", 	Mega,			Purple,		"Mega",		"Activated (3x Activations)\nSpawns a fire sale power up." },
			{ "Kill Joy", 					RareMega,		Purple,		"Mega",		"Activated (2x Activations)\nSpawns an Instakill power up." },
			{ "Killing Time", 				UltraRareMega,	Purple,		"Mega",		"Activated (1x Activation)\nAll zombies freeze in place for 20 seconds.If they are shot, they will be annihilated when the time is up." },
			{ "Licensed Contractor", 		Mega,			Purple,		"Mega",		"Activated (3x Activations)\nSpawns a Carpenter power up." },
			{ "Mind Blown", 				Mega,			Purple,		"Mega",		"Activated (3x Activations)\nGib the heads of all zombies you can see, killing them." },
			{ "Near Death Experience", 		UltraRareMega,	Blue,		"Mega",		"Activates Immediately (Lasts 3 full rounds)\nRevive, or be revived, simply by being near other players. Revived players keep all of their perks." },
			{ "On The House", 				RareMega,		Purple,		"Mega",		"Activated (1x Activation)\nSpawns a free perk power up." },
			{ "Perkaholic", 				UltraRareMega,	Yellow,		"Mega",		"Activates Immediately\nGives you all perks in the map." },
			{ "Phoenix Up", 				Mega,			Purple,		"Mega",		"Activated (1x Activation)\nRevives all teammates. Teammates keep all of their perks." },
			{ "Pop Shocks", 				Mega,			Yellow,		"Mega",		"Auto-activates when melee attacking zombies (5x Activations)\nMelee attacks trigger an electrostatic discharge, electrocuting nearby zombies." },
			{ "Power Vacuum", 				UltraRareMega,	Blue,		"Mega",		"Activates Immediately (Lasts for 4 full rounds)\nMore power ups can drop each round." },
			{ "Profit Sharing", 			UltraRareMega,	Green,		"Mega",		"Activates Immediately (Lasts 10 minutes)\nPoints you earn are also received by nearby players, and vice versa." },
			{ "Reign Drops", 				UltraRareMega,	Purple,		"Mega",		"Activated (2x Activations)\nSpawns one of each of the nine core power ups." },
			{ "Respin Cycle", 				Mega,			Purple,		"Mega",		"Activated(2x Activations)\nRe - spins the weapons in a magic box after it has been activated." },
			{ "Round Robbin'", 				UltraRareMega,	Purple,		"Mega",		"Activated (1x Activation)\nEnds the current round. All players gain 1600 points." },
			{ "Secret Shopper", 			UltraRareMega,	Green,		"Mega",		"Activates Immediately (Lasts 10 minutes)\nAny gun wall-buy can be used to buy ammo for any gun." },
			{ "Self Medication", 			UltraRareMega,	Yellow,		"Mega",		"Auto-activates by getting a kill in last stand (3x Activations)\nAuto revive yourself. Keep all of your perks." },
			{ "Shopping Free", 				UltraRareMega,	Green,		"Mega",		"Activates Immediately(Lasts 1 minute)\nAll purchases are free." },
			{ "Slaughter Slide", 			Mega,			Yellow,		"Mega",		"Auto-activates when sliding (6x Activations)\nCreate 2 lethal explosions by sliding." },
			{ "Soda Fountain", 				RareMega,		Yellow,		"Mega",		"Auto-activates when you buy a perk (5x Activations)\nCan buy an extra perk. Gives you a free perk after you buy one." },
			{ "Temporal Gift", 				RareMega,		Blue,		"Mega",		"Activates Immediately (Lasts 1 full round)\nPower ups last longer." },
			{ "Unbearable", 				Mega,			Yellow,		"Mega",		"Auto-activates when a teddy bear appears in the magic box.\nMagic box re-spins automatically. Magic box will not move for several uses." },
			{ "Undead Man Walking", 		RareMega,		Green,		"Mega",		"Activates Immediately (Lasts 4 minutes)\nSlow down all zombies to shambling speed." },
			{ "Unquenchable", 				Mega,			Yellow,		"Mega",		"Auto-activates when you have maximum perks.\nCan buy an extra perk." },
			{ "Wall Power", 				RareMega,		Yellow,		"Mega",		"Auto-activates on your next wall-buy gun purchase\nThe next gun bought off a wall comes Pack-a-Punched." },
			{ "Who's Keeping Score", 		Mega,			Purple,		"Mega",		"Activated (2x Activations)\nSpawns a double points power up." }
		}
	};

	class BGBPreset {
	public:
		BGBPreset(std::string_view name) : m_Name(name) { }

		BGB*& GetBGB(size_t index) { return m_BGBs[index]; }
		std::string GetPresetName() const { return m_Name; }

		void SetBGB(size_t index, BGB* bgb) { m_BGBs[index] = bgb; }
		void SetPresetName(std::string_view name) { m_Name = name; }

		void SavePreset();
		static void SavePreset(std::string_view name);

		bool operator==(const BGBPreset& preset) { return m_Name == preset.m_Name; }
	private:
		std::string m_Name;
		BGB* m_BGBs[5] = { &bgbs[0], &bgbs[1], &bgbs[2], &bgbs[3], &bgbs[4] };
	};

	inline std::vector<BGBPreset> bgbPresets;
	inline size_t bgbPresetsIndex = 0;
	inline size_t bgbContextGum = 0;
	inline bool bgbClassicSelection = true;

	inline ImGui::FileEditor bgbFiles(ImGui::FileEditor{ "BGB Preset", std::vector<ImGui::TextEditSelectable>{  } });
	inline ImGui::ImageSelection bgbMainGums;

	inline std::vector<BGBImage> bgbClassicImages;
	inline std::vector<BGBImage> bgbMegaImages;
	inline std::vector<std::shared_ptr<BGBImage>> bgbCombinedImages;
	inline std::vector<std::shared_ptr<BGBImage>> bgbDisplayImages;

	void LoadBGBPresets();
	void AddBGBPreset(std::string_view name);
	void DeleteBGBPreset(std::string_view name);
	inline void BGBPresetIndexChanged(size_t index) { bgbPresetsIndex = index; }
	void EditedBGBPreset(std::string_view oldName, std::string_view newName);

	void LoadBGBImages();
	static void UnloadBGBImages() { bgbClassicImages.clear(); bgbMegaImages.clear(); bgbCombinedImages.clear(); bgbDisplayImages.clear(); }
	inline void CopyToDisplayImages(const std::vector<BGBImage>& images)
	{
		bgbDisplayImages.clear();
		for (const BGBImage& image : images) {
			bgbDisplayImages.emplace_back(std::make_shared<BGBImage>(image));
		}
	}

	std::vector<std::shared_ptr<BGBImage>> BGBSearch(const std::vector<std::shared_ptr<BGBImage>>& images, std::string_view text);

	inline void SafeEraseBGBPreset(std::vector<BGBPreset>& presets, std::string_view presetName)
	{
		presets.erase(std::remove_if(presets.begin(), presets.end(), [&](const BGBPreset& preset) {
			return preset.GetPresetName() == presetName;
			}), presets.end());
	}

	inline std::unordered_map<std::string, int> bgbNameToIndex = {
		{ "Alchemical Antithesis", 0 },
		{ "Always Done Swiftly", 1 },
		{ "Anywhere But Here!", 2 },
		{ "Armamental Accomplishment", 3 },
		{ "Arms Grace", 4 },
		{ "Coagulant", 5 },
		{ "Arsenal Accelerator", 6 },
		{ "Danger Closest", 7 },
		{ "Eye Candy", 8 },
		{ "Firing On All Cylinders", 9 },
		{ "Impatient", 10 },
		{ "In Plain Sight", 11 },
		{ "Lucky Crit", 12 },
		{ "Newtonian Negation", 13 },
		{ "Now You See Me", 14 },
		{ "Projectile Vomiting", 15 },
		{ "Stock Option", 16 },
		{ "Sword Flay", 17 },
		{ "Tone Death", 18 },
		{ "Aftertaste", 19 },
		{ "Board Games", 20 },
		{ "Board To Death", 21 },
		{ "Bullet Boost", 22 },
		{ "Burned Out", 23 },
		{ "Cache Back", 24 },
		{ "Crate Power", 25 },
		{ "Crawl Space", 26 },
		{ "Dead Of Nuclear Winter", 27 },
		{ "Disorderly Combat", 28 },
		{ "Ephemeral Enhancement", 29 },
		{ "Extra Credit", 30 },
		{ "Fatal Contraption", 31 },
		{ "Fear In Headlights", 32 },
		{ "Flavor Hexed", 33 },
		{ "Head Drama", 34 },
		{ "Idle Eyes", 35 },
		{ "I'm Feelin' Lucky", 36 },
		{ "Immolation Liquidation", 37 },
		{ "Kill Joy", 38 },
		{ "Killing Time", 39 },
		{ "Licensed Contractor", 40 },
		{ "Mind Blown", 41 },
		{ "Near Death Experience", 42 },
		{ "On The House", 43 },
		{ "Perkaholic", 44 },
		{ "Phoenix Up", 45 },
		{ "Pop Shocks", 46 },
		{ "Power Vacuum", 47 },
		{ "Profit Sharing", 48 },
		{ "Reign Drops", 49 },
		{ "Respin Cycle", 50 },
		{ "Round Robbin'", 51 },
		{ "Secret Shopper", 52 },
		{ "Self Medication", 53 },
		{ "Shopping Free", 54 },
		{ "Slaughter Slide", 55 },
		{ "Soda Fountain", 56 },
		{ "Temporal Gift", 57 },
		{ "Unbearable", 58 },
		{ "Undead Man Walking", 59 },
		{ "Unquenchable", 60 },
		{ "Wall Power", 61 },
		{ "Who's Keeping Score", 62 }
	};
}