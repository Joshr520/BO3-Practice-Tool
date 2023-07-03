#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Walnut/FileFormats/json.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "ImGuiHelper.h"
#include "ListDefs.h"

namespace BO3PT
{
	typedef int RenderWindow;

	enum RenderWindow_ {
		BGBLoadout,
		WeaponLoadout,
		Autosplits,
		PracticePatches,
		Settings,
		PlayerOptions,
		ZombieOptions,
		RoundOptions,
		PowerupOptions,
		EggStepOptions,
		CraftableOptions,
		BlockerOptions,
		MapOptions,
		GumTracker,
		ZombieCalculator,
		CodeGuides,
		GKValveSolver
	};

#pragma region Craftables
	inline int craftListIndex = 0;
	inline int craftStaff = 0;
	inline std::unordered_map<std::string, std::unordered_map<int, int>> craftComboIndexes;
#pragma endregion

#pragma region EggStepOptions
	inline std::vector<std::string> zodRitualSteps = { "Magician Ritual", "Femme Ritual", "Detective Ritual", "Boxer Ritual", "PAP Ritual" };
	inline std::vector<std::string> zodEggSteps = { "Canals Egg", "Footlight Egg", "Waterfront Egg", "Rift Egg" };
	inline std::vector<std::string> zodOvumSteps = { "Magician Ovum", "Detective Ovum", "Femme Ovum", "Boxer Ovum" };
	inline std::vector<std::string> zodFlagSteps = { "Flag 1", "Flag 2", "Flag 3", "Flag 4" };
	inline int zodRitualIndex = 0;
	inline int zodEggIndex = 0;
	inline int zodOvumIndex = 0;
	inline int zodFlagIndex = 0;

	inline std::vector<std::string> castleDragonSteps = { "Courtyard Dragon", "Church Dragon", "Undercroft Dragon" };
	inline std::vector<std::string> castleLighntingSteps = { "Start Quest", "Shoot Bonfires", "Wallrun", "Fill Urns", "Forge Arrow", "Upgrade Bow" };
	inline std::vector<std::string> castleFireSteps = { "Start Quest", "Shoot Obelisk", "Fill Circles", "Finish Golf", "Forge Arrow", "Upgrade Bow" };
	inline std::vector<std::string> castleVoidSteps = { "Start Quest",  "Activate Urn", "Collect Skulls", "Fill Crawlers", "Forge Arrow", "Upgrade Bow" };
	inline std::vector<std::string> castleWolfSteps = { "Start Quest",  "Shoot Shrine", "Escort Wolf", "Forge Arrow", "Upgrade Bow" };
	inline std::vector<std::string> castleTimeTravelSteps = { "Time Travel 1", "Time Travel 2" };
	inline std::vector<std::string> castleSimonSteps = { "Complete Simon", "Call Dempsey" };
	inline std::vector<std::string> castleKeeperSteps = { "Activate Keeper", "Complete Keeper" };
	inline int castleDragonIndex = 0;
	inline int castleLighntingIndex = 0;
	inline int castleFireIndex = 0;
	inline int castleVoidIndex = 0;
	inline int castleWolfIndex = 0;
	inline int castleTimeTravelIndex = 0;
	inline int castleSimonIndex = 0;
	inline int castleKeeperIndex = 0;

	inline std::vector<std::string> islandSkullSteps = { "Skull 1", "Skull 2", "Skull 3", "Skull 4", "Skull Ritual" };
	inline std::vector<std::string> islandEESteps = { "Finish Challenges", "Poster", "Bullet", "Shoot Plane", "Pickup Gears", "Open Elevator", "Start Boss" };
	inline int islandSkullIndex = 0;
	inline int islandEEIndex = 0;

	inline std::vector<std::string> stalingradPAPSteps = { "Finish Grophs" };
	inline std::vector<std::string> stalingradGauntletSteps = { "Pickup Egg", "Awaken Egg", "Napalm Kills", "Multi Kills", "Melee Kills", "Incubation Lockdown", "Incubation", "Pickup Gauntlet" };
	inline std::vector<std::string> stalingradEESteps = { "Tube Puzzle", "Password", "Pickup Trophies", "Challenges", "Deliver Power Core", "Start Nikolai Fight" };
	inline int stalingradPAPIndex = 0;
	inline int stalingradGauntletIndex = 0;
	inline int stalingradEEIndex = 0;
#pragma endregion

#pragma region GUIFunctions
	void InitVariables();
	void LoadImages(RenderWindow sidebarIndex);
	void VerifyInternalFiles();
	void VerifyExternalFiles();
	void WritePracticePatches();
	void NotifyGame(const std::vector<int>& passList);
	void InjectTool(bool enable);
	void ResetToggles();

	bool DoesPathExist(std::string_view s);
	bool CheckVersions(const std::string& newVersion, std::string_view currentVersion);
	bool DownloadAndExtractZip(const std::unordered_set<std::string_view>& wantedFiles);
#pragma endregion

#pragma region MapOptions
	inline int mapListIndex = 0;
#pragma endregion

#pragma region Points
	inline int pointInput = 0;
#pragma endregion

#pragma region Weapons
	inline bool upgradedWeapon = false;
#pragma endregion

#pragma region BGB
	inline int currentBGBPreset = 0;
	inline bool showBGBSelection = false;
	inline bool writeBGBs = false;
	inline BGB bgbSelect;
	inline BGB bgbContext = { "0", "Alchemical Antithesis", "Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon." };
	inline BGB bgbToSwap;
	inline std::vector<BGBPreset> bgbPresets;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> bgbImgList;

	void LoadBGBProfiles();
	void CreateBGBPreset(std::string_view presetName);
	void DeleteBGBPreset(const BGBPreset& preset);
	void SaveBGBPresets();
	void WriteBGBPresetToGame();
	void SwapBGBPreset(BGB bgbOld, BGB bgbNew);
	void SwapBGBTrack(BGB bgbOld, BGB bgbNew);

	bool BGBPresetExists(std::string_view presetName);

	std::vector<BGB> BGBSearch(int type, std::string_view searchText);
#pragma endregion

#pragma region WeaponLoadouts
	struct WeaponCamoGroup {
		std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> m_Camos;
	};

	inline bool writeWeaponPresets = false;
	inline bool camoSelection = false;
	inline bool weaponTypesLoaded[6] = { false };
	inline int currentWeaponPreset = 0;
	inline int currentWeaponPresetMenu = -1;
	inline int currentWeaponEdit = 0;
	inline const ImVec2 weaponKitSize = ImVec2(256, 256);
	inline const ImVec2 camoSelectSize = ImVec2(145, 145);
	inline const ImVec2 camoPreviewSize = ImVec2(168, 168);
	inline const ImVec2 buttonPromptSize = ImVec2(40, 40);
	inline const ImVec2 attachmentSize = ImVec2(120, 120);
	inline const ImVec2 gunSize = ImVec2(128, 128);
	inline std::vector<MenuWeaponPreset> weaponPresets;
	inline std::vector<std::unique_ptr<Walnut::Image>> buildKitImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> weaponIconsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> opticsImgList;
	inline std::unordered_map<std::string, std::unique_ptr<Walnut::Image>> attachmentsImgList;
	inline std::unordered_map<std::string, WeaponCamoGroup> camosImgList;

	void LoadWeaponProfiles();
	void CreateWeaponPreset(std::string_view presetName);
	void DeleteWeaponPreset(const MenuWeaponPreset& preset);
	void SaveWeaponLoadout(const MenuWeaponPreset& preset);
	void WriteWeaponLoadoutToGame(Walnut::JSONBuilder* json = nullptr);

	MenuWeaponPreset ParseWeaponLoadout(std::string_view filename);
#pragma endregion

#pragma region PlayerOptions
	inline int timescaleInt = 1;
	inline bool godActive = false;
	inline bool infAmmoActive = false;
#pragma endregion

#pragma region PowerupOptions
	inline int powerupListIndex = 0;
	inline bool instaGrab = false;
#pragma endregion

#pragma region Gum Tracker
	inline bool gumTrackChosen[5] = { false };
	inline std::vector<BGB> gumTrackBGBs = { { "0", "Alchemical Antithesis", "Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon." },
		{ "1", "Always Done Swiftly", "Activates Immediately (Lasts 3 full rounds)\nWalk faster while aiming. Raise and lower your weapon to aim more quickly." },
		{ "2", "Anywhere But Here!", "Activated (2x Activations)\nInstantly teleport to a random location. A concussive blast knocks away any nearby zombies, keeping you safe." },
		{ "3", "Armamental Accomplishment", "Activates Immediately (Lasts 3 full rounds)\nSwitch weapons and recover from performing melee attacks faster. Reload and use items more quickly." },
		{ "4", "Arms Grace", "Activates Immediately (Lasts until next respawn)\nRespawn with the guns you had when you bled out." } };
	inline int gumTrackCurrentIndex = 0;
#pragma endregion

#pragma region ZombieCalc
	inline int roundNumber = 1;
	inline int zombiesForRound = 0;
	inline int specialZombiesForRound = 0;
	inline int zombiesUpToRound = 0;
	inline int playerCount = 1;
	inline int zombieHealthForRound = 0;
	inline int zombieSpawnRate = 0;
	inline int specialEnemySpawnRate = 0;
	inline int currentSpecialEnemyMap = 0;
	inline int meatballCount = 0;
	inline int dogCount = 0;
	inline int islandSpidersCount = 0;
	inline int manglersCount = 0;
	inline int valksCount = 0;
	inline int genesisSpidersCount = 0;
	inline int furysCount = 0;
	inline int keepersCount = 0;
	inline int specialEnemyCount_1 = 0;
	inline int specialEnemyCount_2 = 0;
	inline int specialEnemyCount_3 = 0;
	inline int specialEnemyPlayerCount = 1;
	inline int specialEnemyRound = 1;
	inline int numDogRounds = 1;
	inline int soePlayerCount = 1;
	inline int soeRound = 1;
	inline int moonPlayerCount = 1;
	inline int moonRound = 1;
	inline int moonEarthTravels = 0;
	inline int moonRoundSkips = 0;
	inline int customZombiesLeft = 1;
	inline int customZombiesLeftRound = 1;
	inline int customZombiesLeftPlayerCount = 1;
	inline int gkLockdownRound = 1;
	inline int gkLockdownPlayerCount = 1;
	inline float hordesForRound = 0.0f;
	inline bool moonRoundSkip = false;
	inline std::string zombieSpawnRateStr = "";
	inline std::string roundTime = "";
	inline std::string expectedRoundTime = "";
	inline std::string soeExpectedRoundTime = "";
	inline std::string moonExpectedRoundTime = "";
	inline std::string customCalcExpectedRoundTime = "";
	inline std::string specialEnemiesRoundTime = "";
	inline std::string waveTime_1 = "";
	inline std::string waveTime_2 = "";
	inline std::string waveTime_3 = "";
	inline std::string waveTime_4 = "";
	inline std::string specialEnemy_1 = "Meatballs";
	inline std::string specialEnemy_2 = "";
	inline std::string specialEnemy_3 = "";
	inline std::string lastEditedSpecialEnemy = "Meatballs";
	inline std::string meatballDelay = "";
	inline std::string dogDelay = "";
	inline std::string spiderDelay = "";
	inline std::string manglerDelay = "";
	inline std::string valkDelay = "";
	inline std::string furyDelay = "";
	inline std::string keeperDelay = "";
	inline std::vector<std::string> specialEnemiesMapCombo = { "SOE", "Giant + DE", "ZNS", "GK", "Rev" };
	inline std::vector<std::string> dogRoundCountCombo = { "1", "2", "3", "4+" };
	inline int corpseDelay[4][255] = { {0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 3, 3, 2, 3, 3, 4, 3, 4, 4, 4, 5, 5,
		5, 5, 5, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 15, 16, 16, 17, 17, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23,
		24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 33, 33, 34, 35, 36, 37, 37, 39, 39, 40, 41, 42, 43, 44, 44, 45, 46, 47, 48, 49, 49,
		50, 52, 52, 53, 55, 55, 56, 58, 58, 59, 61, 61, 62, 63, 64, 66, 66, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86,
		88, 89, 90, 91, 93, 94, 95, 97, 98, 99, 100, 102, 103, 105, 106, 107, 108, 110, 111, 113, 114, 116, 117, 119, 120, 121, 122, 124, 125,
		128, 129, 130, 132, 133, 135, 137, 138, 140, 141, 143, 144, 146, 148, 149, 150, 152, 153, 155, 157, 159, 160, 163, 163, 165, 167, 170,
		171, 173, 174, 176, 178, 180, 181, 182, 185, 187, 189, 190, 192, 194, 196, 198, 200, 202, 203, 206, 207, 210, 211, 213, 214, 217, 219,
		221, 224, 225, 227, 229, 231, 233, 235, 237, 240, 241, 244, 246, 248, 250, 251, 253, 256, 258, 260, 262, 264, 267, 269, 271, 274, 276,
		278, 280, 282, 285, 287, 289},
		{0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 8, 7, 8, 9, 10, 10, 11, 11, 12, 13, 13, 14,
		16, 16, 17, 18, 19, 19, 20, 23, 24, 25, 26, 26, 28, 28, 29, 30, 31, 32, 34, 35, 36, 37, 38, 39, 40, 42, 43, 44, 45, 47, 48, 49, 50, 52,
		53, 54, 56, 57, 59, 60, 62, 63, 64, 66, 68, 69, 71, 73, 74, 76, 77, 79, 81, 82, 84, 86, 87, 89, 92, 93, 94, 97, 99, 100, 102, 104, 106,
		108, 110, 112, 114, 116, 118, 120, 122, 124, 127, 128, 131, 132, 135, 137, 139, 141, 144, 145, 148, 151, 153, 155, 158, 160, 162, 165,
		167, 170, 172, 173, 177, 179, 181, 185, 187, 190, 192, 195, 198, 200, 203, 206, 209, 211, 214, 216, 219, 221, 225, 228, 231, 233, 237,
		240, 243, 245, 248, 251, 253, 258, 260, 263, 266, 269, 272, 275, 279, 282, 285, 288, 291, 294, 297, 301, 304, 307, 310, 314, 317, 321,
		325, 327, 331, 334, 338, 341, 344, 349, 351, 356, 359, 363, 365, 370, 373, 377, 381, 384, 388, 392, 396, 399, 404, 407, 410, 415, 419,
		422, 426, 430, 434, 438, 441, 446, 450, 453, 458, 461, 466, 470, 473, 478, 481, 486, 490, 494, 498, 503, 507, 512, 515, 520, 525, 528,
		532, 537, 541, 545, 550, 554, 558, 565, 568, 572, 576},
		{0, 0, 1, 0, 1, 2, 1, 2, 1, 2, 3, 2, 3, 4, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 10, 10, 12, 12, 13, 14, 15, 16, 18, 19, 20, 22, 23, 24, 25, 29,
		30, 32, 33, 35, 37, 38, 40, 41, 43, 45, 47, 49, 50, 52, 54, 57, 58, 61, 62, 64, 67, 69, 71, 73, 76, 78, 80, 83, 85, 88, 90, 93, 95, 98,
		101, 103, 106, 109, 111, 114, 117, 120, 123, 126, 129, 132, 135, 138, 141, 144, 148, 151, 154, 157, 160, 164, 168, 171, 175, 178, 182,
		185, 189, 192, 196, 200, 204, 207, 211, 215, 219, 224, 227, 232, 235, 239, 243, 247, 252, 255, 260, 265, 268, 273, 278, 282, 286, 291,
		295, 300, 305, 309, 313, 318, 323, 328, 334, 338, 344, 348, 353, 358, 363, 368, 373, 379, 383, 389, 395, 399, 405, 410, 416, 421, 426,
		433, 438, 443, 449, 454, 459, 466, 472, 477, 483, 488, 495, 500, 507, 513, 519, 526, 532, 537, 543, 550, 556, 562, 569, 575, 581, 588,
		594, 600, 606, 613, 621, 627, 633, 639, 647, 653, 660, 667, 675, 682, 689, 696, 702, 710, 718, 722, 730, 737, 745, 752, 760, 767, 775,
		784, 791, 797, 807, 811, 821, 827, 836, 844, 850, 858, 867, 874, 881, 892, 899, 904, 915, 923, 931, 939, 948, 955, 962, 972, 978, 988,
		996, 1004, 1013, 1022, 1031, 1039, 1048, 1055, 1064, 1072, 1083, 1092, 1101, 1109, 1118, 1126, 1135, 1144, 1153},
		{0, 0, 1, 1, 1, 2, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 8, 10, 10, 11, 13, 13, 15, 16, 17, 20, 21, 23, 24, 26, 27, 31, 33, 35, 37, 39, 41,
		43, 46, 48, 50, 52, 55, 57, 59, 62, 65, 67, 70, 73, 76, 79, 81, 84, 87, 90, 94, 97, 100, 103, 106, 110, 113, 117, 120, 124, 128, 131, 135,
		139, 143, 146, 151, 154, 159, 163, 166, 171, 175, 180, 184, 189, 193, 197, 202, 206, 212, 216, 221, 226, 230, 236, 241, 246, 251, 257, 262,
		268, 272, 278, 283, 289, 295, 300, 306, 311, 317, 322, 329, 334, 341, 345, 353, 359, 365, 372, 378, 384, 391, 397, 403, 410, 417, 423, 430,
		437, 443, 451, 457, 465, 471, 479, 486, 493, 500, 509, 515, 523, 530, 538, 545, 552, 560, 568, 576, 584, 591, 599, 609, 616, 624, 631, 639,
		649, 656, 667, 674, 684, 691, 699, 709, 716, 726, 734, 744, 752, 761, 771, 779, 788, 798, 806, 815, 825, 835, 843, 854, 864, 873, 883, 893,
		903, 911, 923, 932, 942, 952, 962, 972, 982, 993, 1001, 1012, 1023, 1034, 1044, 1053, 1066, 1077, 1087, 1097, 1109, 1119, 1129, 1142, 1152,
		1164, 1175, 1185, 1196, 1207, 1219, 1230, 1244, 1255, 1265, 1277, 1290, 1301, 1313, 1324, 1337, 1350, 1361, 1374, 1385, 1397, 1409, 1420,1433,
		1446, 1458, 1469, 1484, 1496, 1509, 1520, 1533, 1547, 1559, 1572, 1587, 1598, 1613, 1624, 1640, 1651, 1664, 1677, 1691, 1705, 1718, 1732} };

	void CalcLockdownTime(int round, int playerCount);
	int GetZombieCountForRound(int round, int playerCount);
	int MaxZombieFunc(int maxZombies, int round);
	int GetZombiesUpToRound(int round, int playerCount);
	int GetZombieHealthForRound(int round);
	int GetZombieSpawnRateForRound(int round, int playerCount);
	int GetSpecialEnemySpawnRate(int playerCount, const std::string& specialEnemy);
	int RoundTimeNearest50(int time);
	float RawSpawnRateForRound(int round, int playerCount);
	std::string CalcRoundTime(int round, int playerCount, int corpseDelay = 0, bool specialEnemies = false);
	std::string SpecialRoundTime(int round, int playerCount, int corpseDelay, bool soe);
	std::string CustomRoundTime(int round, int playerCount, int zombieCount);
	
#pragma endregion

#pragma region SOECodeGuide
	inline int codeIndex_1 = 0;
	inline int codeIndex_2 = 1;
	inline std::vector<std::string> soeCodeCombo;
	inline std::vector<std::shared_ptr<Walnut::Image>> soeCodeImgList;
#pragma endregion

#pragma region GKValveSolver
	inline bool greenChosen = false;
	inline bool passwordChosen = false;
	inline bool noGreenChoice = false;
	inline bool valveComboSet = false;
	inline bool showEvaluation = false;
	inline bool checkedGreenArray[6] = { false };
	inline std::string greenLocation = "";
	inline std::string passwordLocation = "";
	inline std::vector<std::vector<int>> valveDirectionCounts_1 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	inline std::vector<std::vector<int>> valveDirectionCounts_2 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
	inline std::vector<std::string> valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
	inline std::vector<std::string> valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
	inline std::vector<std::string> valveLocations = { "Department", "Dragon Command", "Armory", "Supply", "Infirmary", "Tank" };
	inline std::vector<bool> valveGreen = { false, false, false, false, false, false };
	inline std::vector<bool> valvePassword = { false, false, false, false, false, false };
	inline std::vector<std::vector<bool>> valveDirections = { { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false } };
	inline std::vector<std::vector<std::string>> valveDirectionOdds_1 = { { "", "", "" }, { "", "", "" }, { "", "", "" }, { "", "", "" }, { "", "", "" }, { "", "", "" } };
	inline std::vector<std::vector<std::string>> valveDirectionOdds_2 = { { "", "", "" }, { "", "", "" }, { "", "", "" }, { "", "", "" }, { "", "", "" }, { "", "", "" } };
	inline std::vector<std::vector<std::string>> excludedValves = { };
	inline std::unordered_map<std::string, int> locationToInt = { { "Department", 0 }, { "Dragon Command", 1 }, { "Armory", 2 }, { "Supply", 3}, {"Infirmary", 4}, { "Tank", 5 } };
	inline std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> valveSolutions_1 = { };
	inline std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> valveSolutions_2 = { };
	inline std::unordered_map<std::string, std::vector<std::string>> possibleValves_1 = { };
	inline std::unordered_map<std::string, std::vector<std::string>> possibleValves_2 = { };
	inline std::unordered_map<std::string, std::string> valveLocationsAbbr = { { "Department", "Dept"}, { "Dragon Command", "DC"}, { "Armory", "Arm"}, { "Supply", "Supply"}, { "Infirmary", "Inf"}, { "Tank", "Tank"} };
	inline std::vector<std::unique_ptr<Walnut::Image>> valveSolverImgList;

	void CalcValveProbabilities();
	void CalcRemainingGreen();
	void CalcExcludedValves();
#pragma endregion

#pragma region IceCodeGuide
	struct IceCodePair
	{
		std::shared_ptr<Walnut::Image> digitImage;
		std::shared_ptr<Walnut::Image> symbolImage;
	};

	inline int gameProgress = 0;
	inline int timesMissed = 0;
	inline int timesGuessed = 0;
	inline bool showSolution = false;
	inline bool gameStarted = false;
	inline bool gameChecked[12] = { false };
	inline int randomList[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	inline std::string gameTime = "Time: ";
	inline std::string accuracy = "Accuracy: ";
	inline std::vector<IceCodePair> iceCodePairs;
	inline std::vector<IceCodePair> randomIceCodePairs;
	inline Walnut::Timer gameTimer;
	inline std::unordered_map<std::string, std::shared_ptr<Walnut::Image>> iceCodeImgList;

	void LoadIceCodePairs();
	void RandomizeCodes();
	void ProgressGame(bool success, int numCode);
#pragma endregion

#pragma region Autosplits
	inline int currentAutosplitPreset = 0;
	inline bool writeSplits = false;
	inline std::vector<AutosplitPreset> autosplitPresets;

	void LoadAutosplitPresets();
	void CreateAutosplitPreset(std::string_view presetName);
	void DeleteAutosplitPreset(const AutosplitPreset& preset);
	void SaveAutosplitPreset(const AutosplitPreset& preset);
	void WriteAutosplitPresetToGame(Walnut::JSONBuilder* json = nullptr);
	void WriteSplitXML(std::string_view preset, const std::vector<std::pair<std::string, int>>& splits);
	void WriteLayoutXML(std::string_view preset, int numSplits);
	AutosplitPreset ParseSplitJson(std::string_view filename);

	inline std::vector<std::string> generalSplitData = { "Egg Autosplit", "Song Autosplit", "PAP Autosplit" };
#pragma endregion

#pragma region ZombieOptions
	inline bool zombiesIgnore = false;
	inline bool zombiesFreeze = false;
	inline bool zombiesSpawn = false;
	inline bool zombieSpeedWalk = false;
	inline bool zombieSpeedRun = false;
	inline bool zombieSpeedSprint = false;
#pragma endregion
}