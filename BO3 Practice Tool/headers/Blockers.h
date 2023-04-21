#pragma once

#include <string>
#include <vector>

namespace Blockers
{
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
	inline std::vector<std::string> templeDoorItems = { "Start-Waterfall Upper", "Waterfall-Tunnel", "Waterfall-Cave Water", "Cave Water-Power", "Start-Pressure", "Pressure-Cave 1", "Cave 1-Cave 2", "Cave 3-Power"};
	inline std::vector<std::string> moonDoorItems = { "Start Exit", "Catacombs West", "Tunnel 6 Enter", "Tunnel 6 Exit", "Catacombs East", "Tunnel 11 Enter 1", "Tunnel 11 Enter 2", "Tunnel 11 Exit", "Power Exit", 
		"Dig Enter East", "Biodome Enter East", "Dig Exit East", "Biodome Enter West"};
	inline std::vector<std::string> tombDoorItems = { "Start-Gen 3 Trench", "Gen 3 Trench-Gen 3", "Gen 3-Workshop", "Start-Gen 2 Trench", "Gen 2 Trench-Gen 2", "Gen 2-Workshop", "Workshop-NML", "NML-Mound", "NML-Gen 5", "NML-Tank Station", "Tank Station-Church"};
	inline std::vector<std::string> tombGenItems = { "Gen 1", "Gen 2", "Gen 3", "Gen 4", "Gen 5", "Gen 6" };
}