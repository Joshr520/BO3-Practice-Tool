#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "imgui.h"

namespace BO3PT
{
#pragma region Blockers
	inline int zodDoorIndex = 0;
	inline int zodPowerIndex = 0;
	inline int zodSmashablesIndex = 0;
	inline int factoryDoorIndex = 0;
	inline int castleDoorIndex = 0;
	inline int castleLanderIndex = 0;
	inline int islandDoorIndex = 0;
	inline int stalingradDoorIndex = 0;
	inline int genesisDoorIndex = 0;
	inline int genesisGenIndex = 0;
	inline int prototypeDoorIndex = 0;
	inline int asylumDoorIndex = 0;
	inline int sumpfDoorIndex = 0;
	inline int theaterDoorIndex = 0;
	inline int cosmodromeDoorIndex = 0;
	inline int templeDoorIndex = 0;
	inline int moonDoorIndex = 0;
	inline int tombDoorIndex = 0;
	inline int tombGenIndex = 0;
	inline std::vector<std::string> zodDoorItems = { "Start-Junction", "Canals-Junction", "Canals-Tram", "Canals High-Tram", "Theater-Junction", "Theater Alley-Square", "Theater High-Square",
		"Slums-Junction", "Slums Waterfront-Alley", "Slums High-Train", "Canals Dropdown", "Footlight Dropdown", "Slums Alley Dropdown", "Slums Waterfront Dropdown" };
	inline std::vector<std::string> zodPowerItems = { "Quick Revive", "Canals Perk", "Footlight Perk", "Waterfront Perk", "Stamin Up", "Mule Kick", "Widows Wine",
		"Start Stairs", "Canals Stairs", "Ruby Rabbit Stairs", "Theater Stairs", "Burlesque Door", "Slums Stairs", "Rift Door", "Junction Crane", "Canals Grate" };
	inline std::vector<std::string> zodSmashablesItems = { "Canals Statue", "Theater Statue", "Slums Statue", "Rift Statue", "Summoning Key", "Detective Badge", "Femme Hairpiece",
		"Boxer Belt", "Boxing Ring", "Start Powerup Door", "Canals Portal", "Theater Portal", "Slums Portal" };
	inline std::vector<std::string> factoryDoorItems = { "Outside West", "Warehouse Building", "Warehouse Second Floor", "West TP", "Outside East", "East Building", "East Loading Dock", "East TP", "South TP" };
	inline std::vector<std::string> castleDoorItems = { "Start-Sub Clocktower", "Sub Clocktower-Courtyard", "Courtyard-Great Hall Upper", "Great Hall-Armory", "Start-Gatehouse",
		"Gatehouse-Lower Courtyard", "Lower Courtyard-Livingquarters", "Livingquarters-Armory", "Undercroft-Undercroft Lab", "Gatehouse Dropdown", "Great Hall Dropdown M8", "Great Hall Dropdown Speed Cola",
		"Lower Courtyard Dropdown Dragon", "Lower Courtyard Dropdown Landing Pad" };
	inline std::vector<std::string> castleLanderItems = { "Landing Pad Clocktower", "Landing Pad Rocket", "Landing Pad Gatehouse", "Landing Pad Start" };
	inline std::vector<std::string> islandDoorItems = { "Start-Jungle", "Jungle-Crash Site", "Jungle-Jungle Lab", "Jungle Lab-Jungle Lab Upper", "Jungle Lab-Bunker Exterior", "Start-Swamp", "Swamp-Ruins", "Swamp-Swamp Lab",
		"Swamp Lab-Swamp Lab Upper", "Swamp Lab-Meteor Site", "Swamp Lab-Bunker Exterior", "Bunker Exterior-Bunker Interior", "Bunker Interior-Bunker Upper", "Bunker Interior-Bunker Left", "Bunker Left-Cliffside",
		"Bunker Interior-Bunker Right", "Bunker Interior-Operating Rooms", "Operating Rooms-Flooded Bunker", "Bunker Exterior Dropdown" };
	inline std::vector<std::string> stalingradDoorItems = { "Start-Department", "Department-Alley", "Department-Department Upper", "Bunker-Armory", "Bunker-Infirmary", "Department-Armory", "Department-Infirmary", "Dragon Command",
		"Supply", "Tank", "Supply Tram Dropdown", "Tank Bridge Dropdown" };
	inline std::vector<std::string> genesisDoorItems = { "Start-Left", "Start-Right", "Temple-Temple Stairs", "Temple-Undercroft", "Temple-Theater", "Undercroft-Theater", "Asylum Downstairs-Upstairs", "Asylum Upstairs-Kitchen",
		"Cellblock-Messhall", "Ruins-Mound", "Generator-Trenches", "Prototype Start-Upstairs", "Prison Dropdown Generator", "Prison Dropdown Poster" };
	inline std::vector<std::string> genesisGenItems = { "Mob", "Spawn", "DE", "Verruckt" };
	inline std::vector<std::string> prototypeDoorItems = { "Start-Box", "Start-Upstairs", "Box-Upstairs" };
	inline std::vector<std::string> asylumDoorItems = { "Start Right Door", "Start Left Door", "Right Upstairs Blocker", "Right Upstairs Door", "Right Upstairs Door 2", "Box Right Door", "Left Upstairs Door", "Box Left Door" };
	inline std::vector<std::string> sumpfDoorItems = { "Start-Downstairs", "Start-Upstairs", "Comm Room Area", "Comm Room Hut", "Storage Area", "Storage Hut", "Doc Quarters Area", "Doc Quarters Hut", "Fishing Hut Area", "Fishing Hut" };
	inline std::vector<std::string> theaterDoorItems = { "Start-Crematorium", "Crematorium-Alleyway", "Alleyway-Trap Room", "Trap Room-Stairwell", "Start-Foyer", "Foyer-Dining", "Dining-Dressing", "Dressing-Stage" };
	inline std::vector<std::string> cosmodromeDoorItems = { "Centrifuge-Power", "Power-Centrifuge", "Centrifuge-Alley", "Base Entry-Power", "Power-Roof", "Roof-Catwalk", "Base Entry-North Path", "Catwalk-Lander", "Base Entry-Storage", "Storage-Lander" };
	inline std::vector<std::string> templeDoorItems = { "Start-Waterfall Upper", "Waterfall-Tunnel", "Waterfall-Cave Water", "Cave Water-Power", "Start-Pressure", "Pressure-Cave 1", "Cave 1-Cave 2", "Cave 3-Power" };
	inline std::vector<std::string> moonDoorItems = { "Start Exit", "Catacombs West", "Tunnel 6 Enter", "Tunnel 6 Exit", "Catacombs East", "Tunnel 11 Enter 1", "Tunnel 11 Enter 2", "Tunnel 11 Exit", "Power Exit",
		"Dig Enter East", "Biodome Enter East", "Dig Exit East", "Biodome Enter West" };
	inline std::vector<std::string> tombDoorItems = { "Start-Gen 3 Trench", "Gen 3 Trench-Gen 3", "Gen 3-Workshop", "Start-Gen 2 Trench", "Gen 2 Trench-Gen 2", "Gen 2-Workshop", "Workshop-NML", "NML-Mound", "NML-Gen 5", "NML-Tank Station", "Tank Station-Church" };
	inline std::vector<std::string> tombGenItems = { "Gen 1", "Gen 2", "Gen 3", "Gen 4", "Gen 5", "Gen 6" };
#pragma endregion

#pragma region Craftables
	inline int craftListIndex = 0;
	inline int craftStaff = 0;
	inline std::unordered_map<std::string, std::unordered_map<int, std::vector<std::string>>> craftList;
	inline std::unordered_map<std::string, std::unordered_map<int, std::string>> craftNames;
	inline std::unordered_map<std::string, std::unordered_map<int, int>> craftComboIndexes;

	void InitCraftablesList();
#pragma endregion

#pragma region EggStepOptions
	inline std::vector<std::string> zodRitualSteps = { "Magician Ritual", "Femme Ritual", "Detective Ritual", "Boxer Ritual", "PAP Ritual" };
	inline std::vector<std::string> zodEggSteps = { "Canals Egg", "Footlight Egg", "Waterfront Egg", "Rift Egg" };
	inline std::vector<std::string> zodOvumSteps = { "Magician Ovum", "Femme Ovum", "Detective Ovum", "Boxer Ovum" };
	inline std::vector<std::string> zodFlagSteps = { "Flag 1", "Flag 2", "Flag 3", "Flag 4" };
	inline int zodRitualIndex = 0;
	inline int zodEggIndex = 0;
	inline int zodOvumIndex = 0;
	inline int zodFlagIndex = 0;
#pragma endregion

#pragma region GUIFunctions
	void InitVariables();
	void InitImages();
	void HelpMarker(const std::string& text);
	void DummySpace(float x, float y);
	void SetToggleButtonColor(bool active);
	void FakeButton(const std::string& name, const ImVec2& size, const ImVec4& color);
	void SwapGumSelection(int newGum, int gumSlot);
	void VerifyFileStructure();
	void WritePracticePatches(const int patch[8]);
	void NotifyGame(const std::vector<int>& passList);
	void InjectTool(bool enable, bool& injectResponse);
	void ResetToggles();

	bool CreateButton(const std::string& name, const ImVec2& size, bool* value = NULL, bool toggle = false, const ImVec4& color = { 25, 100, 128, 255 }, bool inGame = false);
	bool CreateListBox(const std::string& name, const std::vector<std::string>& items, int& currentItem, const ImVec2& boxSize);
	bool CreateGumImages(const std::vector<int>& gumArr, const ImVec2& imgSize, int numOnLine, const std::string& type, const std::function<void(int input)>& funcOnPress);
	bool DoesPathExist(const std::string_view& s);
	bool CheckVersions(const std::string& newVersion, const std::string& oldVersion);
	bool DownloadAndExtractZip(const std::unordered_set<std::string_view>& wantedFiles);

	std::vector<int> GumSearch(const std::vector<int>& inGumArr, const std::string& searchText);
	std::vector<int> GetWeaponIndex(const std::string& currentMap, const std::string& weaponSelectName);
#pragma endregion

#pragma region MapOptions
	inline int mapListIndex = 0;
	inline std::vector<std::string> mapList = { "Shadows of Evil", "The Giant", "Der Eisendrache", "Zetsubou No Shima", "Gorod Krovi", "Revelations", "Nacht Der Untoten", "Verruckt", "Shi No Numa", "Kino Der Toten", "Ascension", "Shangri-La", "Moon", "Origins" };
#pragma endregion

#pragma region Points
	inline int pointInput = 0;
#pragma endregion

#pragma region Weapons
	inline bool upgradedWeapon = false;
	inline std::unordered_map<std::string, std::vector<std::string>> weaponList;

	void InitWeaponsList();
#pragma endregion

#pragma region BGB
	struct BGBPreset
	{
		std::string presetName;
		std::vector<int> presetGums{ 0, 0, 0, 0, 0 };
	};

	inline int currentGumPreset = 0;
	inline int gumSelectIndex = 0;
	inline int gumContextIndex = 0;
	inline bool showGumSelection = false;
	inline bool writeGums = false;
	inline std::vector<std::string> classicList;
	inline std::vector<std::string> megaList;
	inline std::vector<std::string> gumDescriptions;
	inline std::vector<BGBPreset> gumPresets;
	inline BGBPreset inactiveGumPreset = { "No Presets Available", { -1, -1, -1, -1, -1 } };

	void InitBGBDescriptions();
	void InitClassicGumsList();
	void InitMegaGumsList();
	void LoadGumProfiles();
	void DeleteGumPreset(const std::string& preset);
	void CreateNewGumPreset(const std::string& presetName);
	void WriteGumPreset(const std::vector<int>& gumPreset);
	void WritePresetToGame(BGBPreset& gumPreset, const std::string& file);
	bool CheckPresetExists(const std::string& inPreset);
	std::string GetCurrentPresetName();
#pragma endregion

#pragma region Perks
	inline std::unordered_map<std::string, std::vector<std::string>> perksList;

	void InitPerksList();
#pragma endregion

#pragma region PlayerOptions
	inline int timescaleInt = 1;
	inline bool godActive = false;
	inline bool infAmmoActive = false;
#pragma endregion

#pragma region PowerupOptions
	inline int powerupListIndex = 0;
	inline bool instaGrab = false;
	inline std::unordered_map<std::string, std::vector<std::string>> powerupList = { };

	void InitPowerupList();


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

	void InitValveSolutions();
	void CalcValveProbabilities();
	void CalcRemainingGreen();
	void CalcExcludedValves();
#pragma endregion

#pragma region IceCodeGuide
	struct IceCodePair
	{
		Walnut::Image* digitImage;
		Walnut::Image* symbolImage;
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

	void InitIceCodePairs();
	void RandomizeCodes();
	void ProgressGame(bool success, int numCode);
#pragma endregion

#pragma region Autosplits
	struct SplitPreset
	{
		std::string presetName;
		std::vector<std::pair<std::string, int>> splits;
		bool igt = false;
		bool igrt = false;
		int numSplits = 0;
		int map = 0;
		int splitType = 0;
		int roundInterval = 1;
	};

	inline int currentSplitPreset = 0;
	inline bool writeSplits = false;
	inline std::vector<SplitPreset> splitPresets;
	inline SplitPreset inactiveSplitPreset = { "No Presets Available", { { "", 0} } };

	void LoadSplitPresets();
	void WriteAutosplitPreset(const SplitPreset& preset);
	void CreateNewAutosplitPreset(const std::string& presetName);
	void DeleteAutosplitPreset(const std::string& preset);
	void WriteSplitXML(const std::string& preset, const std::vector<std::pair<std::string, int>>& splits);
	void WriteLayoutXML(const std::string& preset, int numSplits);
	SplitPreset ParseSplitJson(const std::string& filePath);

	inline std::vector<std::string> generalSplitData = { "Egg Autosplit", "Split on every X round ends", "Song Autosplit", "PAP Autosplit" };

	inline int soeSplits[5] = { 0 };
	inline std::vector<std::string> soeRitualSplits = { "Magician Ritual", "Femme Ritual", "Detective Ritual", "Boxer Ritual", "PAP Ritual" };
	inline std::vector<std::string> soeEggSplits = { "Canals Egg", "Footlight Egg", "Waterfront Egg", "Rift Egg", "Pickup Sword" };
	inline std::vector<std::string> soeOvumSplits = { "Junction Ovum", "Canals Ovum", "Footlight Ovum", "Waterfront Ovum" };
	inline std::vector<std::string> soeFlagSplits = { "Pickup Upgraded Sword", "Activate Book", "Flag 1", "Flag 2", "Flag 3", "Flag 4" };
	inline std::vector<std::string> soeRiftSplits = { "Canals - Rift", "Footlight - Rift", "Waterfront - Rift", "Rift - Canals", "Rift - Footlight", "Rift - Waterfront" };

	inline int deSplits[9] = { 0 };
	inline std::vector<std::string> deDragonSplits = { "Dragon 1", "Dragon 2", "Dragon 3", "Pickup Bow" };
	inline std::vector<std::string> deLightningBowSplits = { "Pickup Arrow", "Bonfires Shot", "Wall Ride", "Crackle", "Upgrade Lightning" };
	inline std::vector<std::string> deFireBowSplits = { "Pickup Arrow", "Obelisk Shot", "Circles Filled", "Golf", "Upgrade Fire " };
	inline std::vector<std::string> deVoidBowSplits = { "Pickup Arrow", "Activate Urn", "Pickup Skulls", "Crawler Kills", "Runes", "Upgrade Void" };
	inline std::vector<std::string> deWolfBowSplits = { "Pickup Arrow", "Shrine Shot", "Wolf Souls Filled", "Arrow Forged", "Upgrade Wolf" };
	inline std::vector<std::string> deWispSplits = { "Time Travel 1", "Time Travel 2" };
	inline std::vector<std::string> deSimonSplits = { "Safe Code Entered", "Simon 1", "Simon 2" };
	inline std::vector<std::string> deKeeperSplits = { "Keeper Spawned", "Keeper 1", "Keeper 2", "Keeper 3", "Keeper 4", "Keeper Trapped" };
	inline std::vector<std::string> deBossSplits = { "Boss Enter", "Boss Exit" };

	inline int znsSplits[4] = { 0 };
	inline std::vector<std::string> znsSkullSplits = { "Skull 1", "Skull 2", "Skull 3", "Skull 4", "Skull Ritual" };
	inline std::vector<std::string> znsBlockerSplits = { "Bunker Open", "Power On" };
	inline std::vector<std::string> znsWWSplits = { "KT-4", "Masamune" };
	inline std::vector<std::string> znsEESplits = { "Poster", "Bullet", "Plane Shot", "Elevator On", "Boss Enter" };

	inline int gkSplits[5] = { 0 };
	inline std::vector<std::string> gkPAPSplits = { "Groph 1", "Groph 2", "Groph 3" };
	inline std::vector<std::string> gkGauntletSplits = { "Pickup Egg", "Egg On Fire", "Incubation Start", "Pickup Gauntlet" };
	inline std::vector<std::string> gkDragonSplits = { "Fly 1", "Fly 2", "Fly 3" };
	inline std::vector<std::string> gkLockdownSplits = { "Lockdown Start", "Lockdown End" };
	inline std::vector<std::string> gkChallengeSplits = { "Start Challenges", "Start Download", "Boss Enter" };

	inline int revSplits[5] = { 0 };
	inline std::vector<std::string> revStartSplits = { "Gen 1", "Gen 2", "Gen 3", "Gen 4", "Keeper Start" };
	inline std::vector<std::string> revReelSplits = { "Pickup Reel 1", "Place Reel 1", "Pickup Reel 2", "Place Reel 2", "Pickup Reel 3", "Place Reel 3" };
	inline std::vector<std::string> revEggSplits = { "Sophia TP", "Egg 1 Fill", "Egg 2 Fill", "Egg 3 Fill", "Egg 4 Fill" };
	inline std::vector<std::string> revRuneSplits = { "Rune 1 Grab", "Rune 2 Grab", "Rune 3 Grab", "Rune 4 Grab", "Boss 1 Enter" };
	inline std::vector<std::string> revEndSplits = { "Symbols", "Boss Rush Finish", "Basketball Finish" };

	inline int tombSplits[2] = { 0 };
	inline std::vector<std::string> tombStaffSplits = { "Ice Craft", "Wind Craft", "Fire Craft", "Lightning Craft" };
	inline std::vector<std::string> tombEndSplits = { "Upgrade Leave", "4 Boxes Done", "Fists Done" };
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