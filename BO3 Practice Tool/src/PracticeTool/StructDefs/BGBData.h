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
        std::string InternalName;
		BGBRarity Rarity = Classic;
		BGBColor Color = Blue;
		std::string Category;
		std::string Description;

        bool operator==(const BGB& bgb) const
        {
            return Name == bgb.Name || InternalName == bgb.InternalName;
        }
        bool operator==(const BGB* bgb) const
        {
            return Name == bgb->Name || InternalName == bgb->InternalName;
        }
        bool operator==(const std::string& name) const
        {
            return Name == name || InternalName == name;
        }
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
            { "Alchemical Antithesis", 		"zm_bgb_alchemical_antithesis",     Classic,		Purple,		"Classic",	"Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon." },
            { "Always Done Swiftly", 		"zm_bgb_always_done_swiftly",       Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nWalk faster while aiming. Raise and lower your weapon to aim more quickly." },
            { "Anywhere But Here!", 		"zm_bgb_anywhere_but_here",         Classic,		Purple,		"Classic",	"Activated (2x Activations)\nInstantly teleport to a random location. A concussive blast knocks away any nearby zombies, keeping you safe." },
            { "Armamental Accomplishment", 	"zm_bgb_armamental_accomplishment", Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nSwitch weapons and recover from performing melee attacks faster. Reload and use items more quickly." },
            { "Arms Grace", 				"zm_bgb_arms_grace",                Classic,		Yellow,		"Classic",	"Activates Immediately (Lasts until next respawn)\nRespawn with the guns you had when you bled out." },
            { "Arsenal Accelerator", 		"zm_bgb_arsenal_accelerator",       Classic,		Green,		"Classic",	"Activates Immediately (Lasts 10 minutes)\nCharge your special weapon faster." },
            { "Coagulant", 					"zm_bgb_coagulant",                 Classic,		Green,		"Classic",	"Activates Immediately (Lasts 20 minutes)\nLonger bleedout time." },
            { "Danger Closest", 			"zm_bgb_danger_closest",            Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nZero explosive damage." },
            { "Eye Candy", 					"zm_bgb_eye_candy",                 Chronicles,		Purple,		"Classic",	"Activated (4x Activations)\nOverrides the colors you see." },
            { "Firing On All Cylinders", 	"zm_bgb_firing_on_all_cylinders",   Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 3 full rounds)\nCan fire while sprinting." },
            { "Impatient", 					"zm_bgb_impatient",                 Classic,		Yellow,		"Classic",	"Activates Immediately (Lasts until bleedout)\nRespawn near the end of the current round instead of at the start of the next round." },
            { "In Plain Sight", 			"zm_bgb_in_plain_sight",            Classic,		Purple,		"Classic",	"Activated (2x Activations, 10 seconds each)\nYou are ignored by zombies for 10 seconds." },
            { "Lucky Crit", 				"zm_bgb_lucky_crit",                Classic,		Blue,		"Classic",	"Activates Immediately (Lasts 1 full round)\nImproves your chances of activating an Alternate Ammo Type." },
            { "Newtonian Negation", 		"zm_bgb_newtonian_negation",        DLC4,			Green,		"Classic",	"Activates Immediately (Lasts 25 minutes)\nZombies killed fall straight up." },
            { "Now You See Me", 			"zm_bgb_now_you_see_me",            Classic,		Purple,		"Classic",	"Activated (2x Activations)\nAll zombies will chase you for 10 seconds." },
            { "Projectile Vomiting", 		"zm_bgb_projectile_vomiting",       DLC3,			Blue,		"Classic",	"Activates Immediately (Lasts 5 full rounds)\nZombies you kill with grenades and large projectiles vomit uncontrollably." },
            { "Stock Option", 				"zm_bgb_stock_option",              Classic,		Green,		"Classic",	"Activates Immediately (Lasts 3 minutes)\nAmmo is taken from your stockpile instead of your weapon's magazine." },
            { "Sword Flay", 				"zm_bgb_sword_flay",                Classic,		Green,		"Classic",	"Activates Immediately (Lasts 2.5 minutes)\nMelee attacks deal zombies 5x as much damage." },
            { "Tone Death", 				"zm_bgb_tone_death",                Chronicles,		Yellow,		"Classic",	"Auto-activates when killing a zombie (25x Activations)\nSilly sounds play when zombies are killed." },
            { "Aftertaste", 				"zm_bgb_aftertaste",                Mega,			Blue,		"Mega",		"Activates Immediately (Lasts 3 full rounds)\nKeep all perks after being revived." },
            { "Board Games", 				"zm_bgb_board_games",               Mega,			Blue,		"Mega",		"Activates Immediately (Lasts 5 full rounds)\nRepairing a board immediately repairs all boards at that window." },
            { "Board To Death", 			"zm_bgb_board_to_death",            Mega,			Green,		"Mega",		"Activates Immediately (Lasts for 5 minutes)\nRepairing a board will kill all nearby zombies." },
            { "Bullet Boost", 				"zm_bgb_bullet_boost",              RareMega,		Purple,		"Mega",		"Activated (2x Activations)\nRe-Pack your current Pack-a-Punched gun(if supported)." },
            { "Burned Out", 				"zm_bgb_burned_out",                Mega,			Yellow,		"Mega",		"Activates Immediately (Lasts two hits)\nThe next time you take melee damage, nearby zombies burst into fire." },
            { "Cache Back", 				"zm_bgb_cache_back",                RareMega,		Purple,		"Mega",		"Activated (1x Activation)\nSpawns a max ammo power up." },
            { "Crate Power", 				"zm_bgb_crate_power",               RareMega,		Yellow,		"Mega",		"Auto-activates next time you take a gun from the magic box\nThe next gun taken from the magic box comes Pack-a-Punched." },
            { "Crawl Space", 				"zm_bgb_crawl_space",               Mega,			Purple,		"Mega",		"Activated (5x Activations)\nAll nearby zombies become crawlers." },
            { "Dead Of Nuclear Winter",		"zm_bgb_dead_of_nuclear_winter",    Mega,			Purple,		"Mega",		"Activated (2x Activations)\nSpawns a nuke power up." },
            { "Disorderly Combat", 			"zm_bgb_disorderly_combat",         Mega,			Green,		"Mega",		"Activates Immediately (Lasts for 5 minutes)\nGives a random gun every 10 seconds." },
            { "Ephemeral Enhancement", 		"zm_bgb_ephemeral_enhancement",     Mega,			Purple,		"Mega",		"Activated (2x Activations, 60 seconds each)\nTurns the weapon in your hands into Pack-a-Punched weapon." },
            { "Extra Credit", 				"zm_bgb_extra_credit",              RareMega,		Purple,		"Mega",		"Activated (4x Activations)\nSpawns a personal 1250 point power up." },
            { "Fatal Contraption", 			"zm_bgb_fatal_contraption",         Mega,			Purple,		"Mega",		"Activated (2x Activations)\nSpawn a Death Machine power up." },
            { "Fear In Headlights", 		"zm_bgb_fear_in_headlights",        RareMega,		Purple,		"Mega",		"Activated (1x Activation, 2 minutes)\nZombies seen by players will not move." },
            { "Flavor Hexed", 				"zm_bgb_flavor_hexed",              Mega,			Yellow,		"Mega",		"Activates Immediately (2x Activations)\nTransforms into a random Mega GobbleGum not in your Pack." },
            { "Head Drama", 				"zm_bgb_head_drama",                UltraRareMega,	Blue,		"Mega",		"Activates Immediately (Lasts for the remainder of the round)\nAny bullet which hits a zombie will damage its head." },
            { "Idle Eyes", 					"zm_bgb_idle_eyes",                 Mega,			Purple,		"Mega",		"Activated(3x Activations, 30 seconds each)\nAll zombies will ignore all players." },
            { "I'm Feelin' Lucky", 			"zm_bgb_im_feelin_lucky",           Mega,			Purple,		"Mega",		"Activated(2x Activations)\nSpawn a random power up." },
            { "Immolation Liquidation", 	"zm_bgb_immolation_liquidation",    Mega,			Purple,		"Mega",		"Activated (3x Activations)\nSpawns a fire sale power up." },
            { "Kill Joy", 					"zm_bgb_kill_joy",                  RareMega,		Purple,		"Mega",		"Activated (2x Activations)\nSpawns an Instakill power up." },
            { "Killing Time", 				"zm_bgb_killing_time",              UltraRareMega,	Purple,		"Mega",		"Activated (1x Activation)\nAll zombies freeze in place for 20 seconds.If they are shot, they will be annihilated when the time is up." },
            { "Licensed Contractor", 		"zm_bgb_licensed_contractor",       Mega,			Purple,		"Mega",		"Activated (3x Activations)\nSpawns a Carpenter power up." },
            { "Mind Blown", 				"zm_bgb_mind_blown",                Mega,			Purple,		"Mega",		"Activated (3x Activations)\nGib the heads of all zombies you can see, killing them." },
            { "Near Death Experience", 		"zm_bgb_near_death_experience",     UltraRareMega,	Blue,		"Mega",		"Activates Immediately (Lasts 3 full rounds)\nRevive, or be revived, simply by being near other players. Revived players keep all of their perks." },
            { "On The House", 				"zm_bgb_on_the_house",              RareMega,		Purple,		"Mega",		"Activated (1x Activation)\nSpawns a free perk power up." },
            { "Perkaholic", 				"zm_bgb_perkaholic",                UltraRareMega,	Yellow,		"Mega",		"Activates Immediately\nGives you all perks in the map." },
            { "Phoenix Up", 				"zm_bgb_phoenix_up",                Mega,			Purple,		"Mega",		"Activated (1x Activation)\nRevives all teammates. Teammates keep all of their perks." },
            { "Pop Shocks", 				"zm_bgb_pop_shocks",                Mega,			Yellow,		"Mega",		"Auto-activates when melee attacking zombies (5x Activations)\nMelee attacks trigger an electrostatic discharge, electrocuting nearby zombies." },
            { "Power Vacuum", 				"zm_bgb_power_vacuum",              UltraRareMega,	Blue,		"Mega",		"Activates Immediately (Lasts for 4 full rounds)\nMore power ups can drop each round." },
            { "Profit Sharing", 			"zm_bgb_profit_sharing",            UltraRareMega,	Green,		"Mega",		"Activates Immediately (Lasts 10 minutes)\nPoints you earn are also received by nearby players, and vice versa." },
            { "Reign Drops", 				"zm_bgb_reign_drops",               UltraRareMega,	Purple,		"Mega",		"Activated (2x Activations)\nSpawns one of each of the nine core power ups." },
            { "Respin Cycle", 				"zm_bgb_respin_cycle",              Mega,			Purple,		"Mega",		"Activated(2x Activations)\nRe - spins the weapons in a magic box after it has been activated." },
            { "Round Robbin'", 				"zm_bgb_round_robbin",              UltraRareMega,	Purple,		"Mega",		"Activated (1x Activation)\nEnds the current round. All players gain 1600 points." },
            { "Secret Shopper", 			"zm_bgb_secret_shopper",            UltraRareMega,	Green,		"Mega",		"Activates Immediately (Lasts 10 minutes)\nAny gun wall-buy can be used to buy ammo for any gun." },
            { "Self Medication", 			"zm_bgb_self_medication",           UltraRareMega,	Yellow,		"Mega",		"Auto-activates by getting a kill in last stand (3x Activations)\nAuto revive yourself. Keep all of your perks." },
            { "Shopping Free", 				"zm_bgb_shopping_free",             UltraRareMega,	Green,		"Mega",		"Activates Immediately(Lasts 1 minute)\nAll purchases are free." },
            { "Slaughter Slide", 			"zm_bgb_slaughter_slide",           Mega,			Yellow,		"Mega",		"Auto-activates when sliding (6x Activations)\nCreate 2 lethal explosions by sliding." },
            { "Soda Fountain", 				"zm_bgb_soda_fountain",             RareMega,		Yellow,		"Mega",		"Auto-activates when you buy a perk (5x Activations)\nCan buy an extra perk. Gives you a free perk after you buy one." },
            { "Temporal Gift", 				"zm_bgb_temporal_gift",             RareMega,		Blue,		"Mega",		"Activates Immediately (Lasts 1 full round)\nPower ups last longer." },
            { "Unbearable", 				"zm_bgb_unbearable",                Mega,			Yellow,		"Mega",		"Auto-activates when a teddy bear appears in the magic box.\nMagic box re-spins automatically. Magic box will not move for several uses." },
            { "Undead Man Walking", 		"zm_bgb_undead_man_walking",        RareMega,		Green,		"Mega",		"Activates Immediately (Lasts 4 minutes)\nSlow down all zombies to shambling speed." },
            { "Unquenchable", 				"zm_bgb_unquenchable",              Mega,			Yellow,		"Mega",		"Auto-activates when you have maximum perks.\nCan buy an extra perk." },
            { "Wall Power", 				"zm_bgb_wall_power",                RareMega,		Yellow,		"Mega",		"Auto-activates on your next wall-buy gun purchase\nThe next gun bought off a wall comes Pack-a-Punched." },
            { "Who's Keeping Score", 		"zm_bgb_whos_keeping_score",        Mega,			Purple,		"Mega",		"Activated (2x Activations)\nSpawns a double points power up." }
        }
    };
    inline std::vector<std::string> bgbClassics = { "Alchemical Antithesis", "Always Done Swiftly", "Anywhere But Here!", "Armamental Accomplishment", "Arms Grace", "Arsenal Accelerator", "Coagulant", "Danger Closest", "Eye Candy", "Firing On All Cylinders",
        "Impatient", "In Plain Sight", "Lucky Crit", "Newtonian Negation", "Now You See Me", "Projectile Vomiting", "Stock Option", "Sword Flay", "Tone Death" };
    inline std::vector<std::string> bgbMegas = { "Aftertaste", "Board Games", "Board To Death", "Bullet Boost", "Burned Out", "Cache Back", "Crate Power", "Crawl Space", "Dead Of Nuclear Winter", "Disorderly Combat", "Ephemeral Enhancement", "Extra Credit",
        "Fatal Contraption", "Fear In Headlights", "Flavor Hexed", "Head Drama", "Idle Eyes", "I'm Feelin' Lucky", "Immolation Liquidation", "Kill Joy", "Killing Time", "Licensed Contractor", "Mind Blown", "Near Death Experience", "On The House", "Perkaholic", "Phoenix Up",
        "Pop Shocks", "Power Vacuum", "Profit Sharing", "Reign Drops", "Respin Cycle", "Round Robbin'", "Secret Shopper", "Self Medication", "Shopping Free", "Slaughter Slide", "Soda Fountain", "Temporal Gift", "Unbearable", "Undead Man Walking", "Unquenchable", "Wall Power", "Who's Keeping Score" };

	class BGBPreset {
	public:
		BGBPreset(std::string_view name) : m_Name(name) { }

		BGB* GetBGB(size_t index) const { return m_BGBs[index]; }
		std::string_view GetPresetName() const { return m_Name; }
		static BGBPreset* GetCurrentPreset() { return s_CurrentPreset; }

        void SetBGB(size_t index, BGB* oldBGB, const BGB& newBGB);
		void SetPresetName(std::string_view name) { m_Name = name; }

		void SavePreset();
		static void SavePreset(std::string_view name);
        static void WritePresetToGame(bool reset = false);

		static void LoadBGBPresets();
		static void AddBGBPreset(std::string_view name);
		static void DeleteBGBPreset(std::string_view name);
		static void BGBPresetIndexChanged(size_t index) { s_CurrentPreset = &s_Presets[index]; }
		static void EditedBGBPreset(std::string_view oldName, std::string_view newName);

		bool operator==(const BGBPreset& preset) { return m_Name == preset.m_Name; }
	private:
		std::string m_Name;
		BGB* m_BGBs[5] = { &bgbs[0], &bgbs[1], &bgbs[2], &bgbs[3], &bgbs[4] };

		static std::vector<BGBPreset> s_Presets;
		static BGBPreset* s_CurrentPreset;
	};

	void LoadBGBImages();
    void LoadSingleBGBImage(const std::string& name);
	void UnloadBGBImages();

	std::vector<std::shared_ptr<BGBImage>> BGBSearch(const std::vector<std::shared_ptr<BGBImage>>& images, std::string_view text);

    inline bool bgbPresetsActive = false;
    inline bool bgbClassicSelection = true;
    inline ImGui::FileEditor bgbFiles(ImGui::FileEditor{ "BGB Preset", std::vector<ImGui::TextEditSelectable>{  } });
    inline ImGui::MultiSelection bgbSelections({ ImGui::SelectionData(bgbClassics), ImGui::SelectionData(bgbMegas) });
    inline std::unordered_map<std::string, BGBImage> bgbImages;
    inline std::vector<std::shared_ptr<BGBImage>> bgbCombinedImages;
    inline std::vector<std::shared_ptr<BGBImage>> bgbDisplayImages;
}