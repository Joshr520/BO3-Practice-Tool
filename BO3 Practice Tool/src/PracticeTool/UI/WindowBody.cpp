#include "WindowBody.h"

#include <unordered_map>
#include <functional>
#include <iostream>
#include <stdexcept>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Walnut/UI/UI.h"
#include "Walnut/ApplicationGUI.h"
#include "Embed/IcoMoon.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include "PracticeTool/StructDefs/BGBData.h"
#include "PracticeTool/StructDefs/WeaponData.h"
#include "PracticeTool/StructDefs/AutosplitData.h"
#include "PracticeTool/StructDefs/GameData.h"
#include "PracticeTool/UI/KeyboardIcons.h"
#include "PracticeTool/Core/Memory.h"
#include "PracticeTool/Core/Interop.h"
#include "PracticeTool/Core/Log.h"

#include "Embed/Icons.embed"

// Map Selection Data
size_t mapSelectIndex = 0;

// BGB Preset Data
size_t bgbFilterIndex = 0;
bool bgbSelection = false;
std::string bgbContextGum = "Alchemical Antithesis";
std::vector<std::string> bgbFilters = { "Alphabetical", "Rarity", "Color" };
ImGui::ImageSelection bgbMainGums;

// Autosplit Preset Data
struct AutosplitMenuGroup {
    std::string Name;
    std::vector<std::string> Splits;
};

size_t autosplitMapChangeNum = 0;
size_t autosplitAddType = 0;
size_t autosplitAddSplit = 0;
int autosplitRoundInput = 1;
std::vector<std::string> autosplitTypes = { "Egg Speedrun", "Song Speedrun", "PAP Speedrun" };
std::vector<std::string> autosplitConditions = { "None", "Current Round", "Specific Round" };
std::vector<std::vector<AutosplitMenuGroup>> autosplitMenuSplits = {
    {	// zm_zod
        { "Round Splits" },
        { "Ritual Starts", { "Magician Ritual", "Femme Ritual", "Detective Ritual", "Boxer Ritual", "PAP Ritual" } },
        { "Rift Travels", { "Canals - Rift", "Footlight - Rift", "Waterfront - Rift", "Rift - Canals", "Rift - Footlight", "Rift - Waterfront" } },
        { "Egg Places", { "Canals Egg", "Footlight Egg", "Waterfront Egg", "Rift Egg", "Pickup Sword" } },
        { "Ovum Starts", { "Junction Ovum", "Canals Ovum", "Footlight Ovum", "Waterfront Ovum" } },
        { "Flag Steps", { "Pickup Upgraded Sword", "Activate Book", "Flag 1", "Flag 2", "Flag 3", "Flag 4" } },
    },
    {	// zm_facotry
        { "Round Splits" },
    },
    {	// zm_castle
        { "Round Splits" },
        { "Dragon Fills", { "Church Dragon", "Courtyard Dragon", "Undercroft Dragon", "Pickup Bow" } },
        { "Lightning Bow", { "Start Lightning", "Bonfires Shot", "Wall Ride", "Crackle", "Upgrade Lightning" } },
        { "Fire Bow", { "Start Fire", "Obelisk Shot", "Circles Filled", "Golf", "Upgrade Fire" } },
        { "Void Bow", { "Start Void", "Activate Urn", "Pickup Skulls", "Crawler Kills", "Runes", "Upgrade Void" } },
        { "Wolf Bow", { "Start Wolf", "Shrine Shot", "Start Escort", "Escort Finished", "Arrow Forged", "Upgrade Wolf" } },
        { "Wisps", { "Normal TP", "Time Travel 1", "Time Travel 2" } },
        { "Simon Says", { "Safe Code Entered", "Simon 1", "Simon 2" } },
        { "Keeper", { "Keeper Spawned", "Keeper 1", "Keeper 2", "Keeper 3", "Keeper 4", "Keeper Trapped" } },
        { "Boss", { "Boss Enter", "Boss Exit" } }
    },
    {	// zm_island
        { "Round Splits" },
        { "Skull Lockdowns", { "Skull 1", "Skull 2", "Skull 3", "Skull 4", "Skull Ritual" } },
        { "Blockers", { "Bunker Open", "Power On" } },
        { "Wonder Weapon", { "KT-4", "Masamune" } },
        { "Main EE", { "Poster", "Bullet", "Plane Shot", "Elevator On", "Boss Enter" } }
    },
    {	// zm_stalingrad
        { "Round Splits" },
        { "PAP", { "Groph 1", "Groph 2", "Groph 3" } },
        { "Gauntlet", { "Pickup Egg", "Egg On Fire", "Incubation Start", "Pickup Gauntlet" } },
        { "Dragon Flys", { "Fly Supply", "Fly Tank", "Fly DC" } },
        { "Lockdown", { "Lockdown Start", "Lockdown End" } },
        { "Challenges", { "Start Challenges", "Start Download", "Boss Enter" } }
    },
    {	// zm_genesis
        { "Round Splits" },
        { "EE Start", { "Spawn Gen", "DE Gen", "Verruckt Gen", "Mob Gen", "Keeper Start" } },
        { "Apothicon Entry/Exit", { "Enter Beast", "Exit Beast Mob", "Exit Beast Verruckt", "Exit Beast DE", "Exit Beast Spawn" } },
        { "Reels", { "Pickup Reel 1", "Place Reel 1", "Pickup Reel 2", "Place Reel 2", "Pickup Reel 3", "Place Reel 3" } },
        { "Egg Pickups", { "Sophia TP", "Egg 1", "Egg 2", "Egg 3", "Egg 4" } },
        { "Rune Pickups", { "Rune 1", "Rune 2", "Rune 3", "Rune 4", "Boss 1 Enter" } },
        { "EE End", { "Symbols", "Basketball", "Boss 2" } }
    },
    {	// zm_prototype
        { "Round Splits" },
    },
    {	// zm_asylum
        { "Round Splits" },
    },
    {	// zm_sumpf
        { "Round Splits" },
    },
    {	// zm_theater
        { "Round Splits" },
    },
    {	// zm_cosmodrome
        { "Round Splits" },
    },
    {	// zm_temple
        { "Round Splits" },
    },
    {	// zm_moon
        { "Round Splits" },
    },
    {	// zm_tomb
        { "Round Splits" },
        { "Staves", { "Ice Craft", "Wind Craft", "Fire Craft", "Lightning Craft" } },
        { "Portals", { "Fire Enter", "Fire Leave", "Wind Enter", "Wind Leave", "Lightning Enter", "Lightning Leave", "Ice Enter", "Ice Leave" } },
        { "EE End", { "4 Boxes Done", "Fists Done" } }
    }
};

// Practice Patches Data
bool practicePatchesActive = false;

// Player Data
size_t perkSelectIndex = 0;
int timescale = 1;
int playerSpeed = 190;
int pointInput = 0;
bool godmode = false;
bool noclip = false;
bool infiniteAmmo = false;
BO3PracticeTool::BGB* currentBGB = &BO3PracticeTool::bgbs[0];

// Weapon Data
size_t takeWeaponIndex = 0;
bool upgradedWeapon = false;
std::vector<std::string> takeWeaponChocies = { "Primary Weapon", "Hero Weapon", "Lethals", "Tacticals", "Shield Slot", "Mine", "Wearable" };

// Zombie Data
size_t zombieSpeedIndex = 0;
int zombieHealthRound = 1;
bool zombieHealthOverride = false;
bool zombieSpeedOverride = false;
bool zombieFreeze = false;
bool zombieSpawning = true;
bool zombieFreezeTP = false;
std::vector<std::string> zombieSpeedOverrides = { "Walk", "Run", "Sprint" };

// Round Data
int roundInput = 1;
int zombieCount = 0;
bool infiniteRound = false;

// Powerup Data
bool instaGrab = false;
size_t powerupIndex = 0;

struct Powerup {
    std::string Name;
    std::string InternalName;
};

Powerup maxAmmo = { "Max Ammo", "full_ammo" };
Powerup doublePoints = { "Double Points", "double_points" };
Powerup instaKill = { "Insta Kill", "insta_kill" };
Powerup nuke = { "Nuke", "nuke" };
Powerup deathMachine = { "Death Machine", "minigun" };
Powerup carpenter = { "Carpenter", "carpenter" };
Powerup fireSale = { "Fire Sale", "fire_sale" };
Powerup widowsGrenade = { "Widows Grenade", "ww_grenade" };
Powerup tramToken = { "Tram Token", "castle_tram_token" };
Powerup voidTears = { "Void Symbol Tears", "demonic_rune_lor" };
Powerup voidRib = { "Void Symbol Rib", "demonic_rune_ulla" };
Powerup voidTriangles = { "Void Symbol Triangles", "demonic_rune_uja" };
Powerup voidClaw = { "Void Symbol Claw", "demonic_rune_oth" };
Powerup voidTear = { "Void Symbol Tear", "demonic_rune_zor" };
Powerup voidRibbon = { "Void Symbol Ribbon", "demonic_rune_mar" };
Powerup perkSlot = { "Perk Slot", "empty_perk" };
Powerup seed = { "Seed", "island_seed" };
Powerup cylinderTank = { "Tank Cylinder", "code_cylinder_red" };
Powerup cylinderSupply = { "Supply Cylinder", "code_cylinder_yellow" };
Powerup cylinderDC = { "DC Cylinder", "code_cylinder_blue" };
Powerup zombieBlood = { "Zombie Blood", "zombie_blood" };

std::unordered_map<std::string, std::vector<Powerup>> powerups = {
    {
        { "zm_zod", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_factory", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale }
        }},
        { "zm_castle", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade, tramToken, voidTears, voidRib, voidTriangles, voidClaw, voidTear, voidRibbon }
        }},
        { "zm_island", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade, perkSlot, seed }
        }},
        { "zm_stalingrad", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade, cylinderTank, cylinderSupply, cylinderDC }
        }},
        { "zm_genesis", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_prototype", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_asylum", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_sumpf", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_theater", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_cosmodrome", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_temple", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_moon", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade }
        }},
        { "zm_tomb", {
            { maxAmmo, doublePoints, instaKill, nuke, deathMachine, carpenter, fireSale, widowsGrenade, zombieBlood }
        }}
    }
};

// Craftable Data
struct PieceItem {
    std::string Name;
    std::string InternalName;
};

struct TreeItem {
    std::string Name;
    std::string InternalName;
    std::vector<PieceItem> Pieces;
};

size_t treeIndex = 0;
size_t pieceIndex = 0;
PieceItem staffGem = { "Gem", "gem" };
PieceItem staffUpper = { "Staff Upper", "upper_staff" };
PieceItem staffMiddle = { "Staff Middle", "middle_staff" };
PieceItem staffLower = { "Staff Lower", "lower_staff" };
TreeItem shieldCraft = { "Shield", "craft_shield_zm", { { "Dolly", "dolly" }, { "Door", "door" }, { "Clamp", "clamp" } } };
std::unordered_map<std::string, std::vector<TreeItem>> craftables = {
    { "zm_zod", {
        shieldCraft,
        { "Civil Protector", "police_box", { { "Fuse 1", "fuse_01" }, { "Fuse 2", "fuse_02" }, { "Fuse 3", "fuse_03" } } },
        { "Apothicon Servant", "idgun", { { "Margwa Heart", "part_heart" }, { "Margwa Tentacle", "part_skeleton" }, { "Xenomatter", "part_xenomatter" } } },
        { "Magician Memento", "ritual_magician", { { "Golden Fountain Pen", "memento_magician" } } },
        { "Detective Memento", "ritual_detective", { { "Detective Badge", "memento_detective" } } },
        { "Boxer Memento", "ritual_boxer", { { "Championship Belt", "memento_boxer" } } },
        { "Femme Memento", "ritual_femme", { { "Hairpiece", "memento_femme" } } },
        { "Gateworms", "ritual_pap", { { "Magician Gateworm", "relic_magician" }, { "Detective Gateworm", "ritual_detective" }, { "Boxer Gateworm", "ritual_boxer" }, { "Femme Gateworm", "ritual_femme" } } }
    }},
    { "zm_castle", {
        shieldCraft,
        { "Ragnarok DG-4", "gravityspike", { { "Body", "part_body" }, { "Guards", "part_guards" }, { "Handle", "part_handle" } } }
    }},
    { "zm_island", {
        shieldCraft,
        { "Gasmask", "gasmask", { { "Visor", "part_visor" }, { "Filter", "part_filter" }, { "Strap", "part_strap" } } }
    }},
    { "zm_stalingrad", {
        shieldCraft,
        { "Dragon Controller", "dragonride", { { "Codes", "part_codes" }, { "Transmitter", "part_transmitter" }, { "Map", "part_map" } } }
    }},
    { "zm_genesis", {
        shieldCraft,
        { "Keeper Protector", "keeper_callbox_", { { "Totem", "totem" }, { "Head", "head" }, { "Gem", "gem" } } }
    }},
    { "zm_tomb", {
        shieldCraft,
        { "Maxis Drone", "equip_dieseldrone", { { "Body", "body" }, { "Brain", "brain" }, { "Engine", "engine" } } },
        { "Gramophone", "gramophone", { { "Record Player", "vinyl_player" }, { "Master Disc", "vinyl_master" }, { "Wind Disc", "vinyl_air" }, { "Ice Disc", "vinyl_ice" }, { "Fire Disc", "vinyl_fire" }, { "Storm Disc", "vinyl_elec" } } },
        { "Wind Staff", "elemental_staff_air", { staffGem, staffUpper, staffMiddle, staffLower } },
        { "Ice Staff", "elemental_staff_water", { staffGem, staffUpper, staffMiddle, staffLower } },
        { "Fire Staff", "elemental_staff_fire", { staffGem, staffUpper, staffMiddle, staffLower } },
        { "Lightning Staff", "elemental_staff_lightning", { staffGem, staffUpper, staffMiddle, staffLower } }
    }}
};

// Blocker Data
struct Blocker {
    std::string Notify;
    std::string Name;
};

struct MapBlockers {
    size_t Index;
    std::string Type;
    std::vector<Blocker> Blockers;
};

size_t blockerTypeIndex = 0;
size_t blockerIndex = 0;
std::vector<std::string> blockerTypes;
std::unordered_map<std::string, std::vector<MapBlockers>> blockers = {
    { "zm_zod", {
        { 0, "Doors", { { "4", "Spawn" FONT_ARROW_RIGHT "Junction" }, { "7", "Canals" FONT_ARROW_RIGHT "Junction" }, { "8", "Canals" FONT_ARROW_RIGHT "Tram" }, { "3", "Upper Canals" FONT_ARROW_RIGHT "Tram" }, { "6", "Footlight" FONT_ARROW_RIGHT "Junction" }, { "10", "Footlight" FONT_ARROW_RIGHT "Burlesque" },
            { "2", "Upper Footlight" FONT_ARROW_RIGHT "Tram" }, { "5", "Waterfront" FONT_ARROW_RIGHT "Junction" }, { "0", "Waterfront" FONT_ARROW_RIGHT "Boxing Ring" }, { "1", "Waterfront Egg" FONT_ARROW_RIGHT "Tram" }, { "15", "Canals Dropdown" }, { "17", "Footlight Dropdown" },
            { "18", "Waterfront Dropdown" }, { "19", "Boxing Ring Dropdown" } } },
        { 0, "Power", { { "1", "Quick Revive" }, { "4", "Canals Perk" }, { "5", "Footlight Perk" }, { "6", "Waterfront Perk" }, { "2", "Stamin Up" }, { "3", "Mule Kick" }, { "7", "Widows Wine" }, { "14", "Spawn Stairs" },
            { "12", "Canals Stairs" }, { "16", "Ruby Rabbit Stairs" }, { "13", "Footlight Stairs" } , { "21", "Burlesque Door" } , { "11", "Waterfront Stairs" }, { "15", "Rift Door" }, { "20", "Junction Crane" }, { "23", "Canals Grate" } } },
        { 0, "Smashables", { { "pf29459_auto3", "Canals Statue" }, { "pf29461_auto3", "Footlight Statue" }, { "pf29468_auto3", "Waterfront Statue" }, { "pf29470_auto3", "Rift Statue" },
            { "unlock_quest_key", "Summoning Key" }, { "memento_detective_drop", "Detective Badge" }, { "memento_femme_drop", "Femme Hairpiece" }, { "memento_boxer_belt", "Boxer Belt" },
            { "smash_trigger_open_slums", "Boxing Ring" }, { "smash_unnamed_0", "Spawn Powerup" }, { "canal_portal", "Canals Portal" }, { "theater_portal", "Footlight Portal" },
            { "slums_portal", "Waterfront Portal" } } }
    }},
    { "zm_factory", {
        { 0, "Doors", { { "2", "Spawn" FONT_ARROW_RIGHT "KRM" }, { "5", "KRM" FONT_ARROW_RIGHT "Kuda" }, { "6", "Kuda" FONT_ARROW_RIGHT "TP A" }, { "10", "Kuda" FONT_ARROW_RIGHT "Trip Mine" }, { "3", "Spawn" FONT_ARROW_RIGHT "L-CAR" }, { "4", "L-CAR" FONT_ARROW_RIGHT "VMP" }, { "11", "VMP" FONT_ARROW_RIGHT "1 Window" }, { "7", "1 Window" FONT_ARROW_RIGHT "TP B" }, { "0", "Power" FONT_ARROW_RIGHT "TP C" } } },
        { 0, "Teleporters", { { "0", "TP A" }, { "2", "TP B" }, { "1", "TP C" } } },
        { 0, "Power", { { "power_on", "Turn on Power" } } }
    }},
    { "zm_castle", {
        { 0, "Doors", { { "0", "Spawn" FONT_ARROW_RIGHT "Mule Kick" }, { "13", "Mule Kick" FONT_ARROW_RIGHT "Courtyard" }, { "14", "Courtyard" FONT_ARROW_RIGHT "Church" }, { "1", "Church" FONT_ARROW_RIGHT "Power" }, { "7", "Spawn" FONT_ARROW_RIGHT "Gatehouse" }, { "12", "Gatehouse" FONT_ARROW_RIGHT "Lower Courtyard" },
            { "15", "Lower Courtyard" FONT_ARROW_RIGHT "Livingquarters" }, { "3", "Livingquarters" FONT_ARROW_RIGHT "Power" }, { "4", "Undercroft" FONT_ARROW_RIGHT "TP" }, { "19", "Gatehouse Dropdown" }, { "21", "Church Dropdown M8" }, { "18", "Church Dropdown Speed Cola" },
            { "20", "Lower Courtyard Dropdown Dragon" }, { "22", "Lower Courtyard Dropdown Landing Pad" } } },
        { 0, "Landing Pads", { { "3", "Spawn" },  { "0", "Clocktower" }, { "2", "Gatehouse" },  { "1", "Rocket" } } },
        { 0, "Power", { { "power_on", "Turn on Power" } } }
    }},
    { "zm_island", {
        { 0, "Doors", { { "2", "Spawn" FONT_ARROW_RIGHT "KRM" }, { "14", "KRM" FONT_ARROW_RIGHT "Crash Site" }, { "4", "KRM" FONT_ARROW_RIGHT "Lab A" }, { "10", "Lab A" FONT_ARROW_RIGHT "Lab A Interior" }, { "6", "Lab A" FONT_ARROW_RIGHT "Bunker Exterior" }, { "1", "Spawn" FONT_ARROW_RIGHT "L-CAR" }, { "3", "L-CAR" FONT_ARROW_RIGHT "Lab B" },
            { "11", "L-CAR" FONT_ARROW_RIGHT "Altar" }, { "20", "Lab B" FONT_ARROW_RIGHT "Lab B Interior" }, { "9", "Lab B" FONT_ARROW_RIGHT "Meteor Site" }, { "5", "Lab B" FONT_ARROW_RIGHT "Bunker Exterior" }, { "12", "Bunker" FONT_ARROW_RIGHT "AA Gun" }, { "17", "Bunker" FONT_ARROW_RIGHT "Power" }, { "7", "Power" FONT_ARROW_RIGHT "Zipline" },
            { "18", "Bunker" FONT_ARROW_RIGHT "Living Quarters" }, { "8", "Bunker" FONT_ARROW_RIGHT "Operation Room" }, { "19", "Operation Room" FONT_ARROW_RIGHT "Flooded Bunker" }, { "23", "Bunker Dropdown" } } },
        { 0, "Power", { { "0", "Lab A Power" }, { "1", "Lab B Power" }, { "bunker", "Open Bunker Door" }, { "power_on", "Turn on Power" } } },
        { 0, "PAP", { { "pap", "Open PAP Gate" } } }
    }},
    { "zm_stalingrad", {
        { 0, "Doors", { { "12", "Spawn" FONT_ARROW_RIGHT "Department" }, { "10", "Department" FONT_ARROW_RIGHT "Bunker" }, { "21", "Department" FONT_ARROW_RIGHT "Department Upper" }, { "2", "Bunker" FONT_ARROW_RIGHT "Armory" }, { "4", "Bunker" FONT_ARROW_RIGHT "Infirmary" }, { "14", "Department" FONT_ARROW_RIGHT "Armory" },
            { "19", "Department" FONT_ARROW_RIGHT "Infirmary" }, { "13", "Dragon Command" }, { "23", "Supply" }, { "7", "Tank" }, { "16", "Supply Dropdown" }, { "24", "Tank Dropdown" } } }
    }},
    { "zm_genesis", {
        { 0, "Doors", { { "5", "Spawn" FONT_ARROW_RIGHT "Shang" }, { "4", "Shang" FONT_ARROW_RIGHT "DE" }, { "8", "DE" FONT_ARROW_RIGHT "Undercroft" }, { "7", "DE" FONT_ARROW_RIGHT "Kino" }, { "6", "Undercroft" FONT_ARROW_RIGHT "Kino" }, { "9", "Verruckt" FONT_ARROW_RIGHT "Verruckt Gen" }, { "10", "Verruckt Gen" FONT_ARROW_RIGHT "Verruckt Upstairs" },
            { "13", "Spawn" FONT_ARROW_RIGHT "Origins" },  { "0", "Origins" FONT_ARROW_RIGHT "Origins Gen" }, { "2", "Origins Gen" FONT_ARROW_RIGHT "Mound" }, { "1", "Mob" FONT_ARROW_RIGHT "Cafeteria" }, { "14", "Nacht" FONT_ARROW_RIGHT "Nacht Upstairs" }, { "11", "Mob Gen Dropdown" }, { "12", "Mob Poster Dropdown" } } },
        { 0, "Engines", { { "1", "Spawn" }, { "2", "DE" }, { "3", "Verruckt" }, { "0", "Mob" } } },
        { 0, "Apothicon", { { "trap_apothicon", "Trap Apothicon" } } }
    }},
    { "zm_prototype", {
        { 0, "Doors", { { "0", "Spawn" FONT_ARROW_RIGHT "Box" }, { "1", "Spawn" FONT_ARROW_RIGHT "Upstairs" }, { "3", "Box" FONT_ARROW_RIGHT "Upstairs" } } }
    }},
    { "zm_asylum", {
        { 0, "Doors", { { "0", "QR" FONT_ARROW_RIGHT "Mule Kick" }, { "1", "QR" FONT_ARROW_RIGHT "Bootlegger" }, { "9", "Mule Kick" FONT_ARROW_RIGHT "ICR" }, { "4", "ICR" FONT_ARROW_RIGHT "Speed" }, { "5", "Speed" FONT_ARROW_RIGHT "Kitchen" }, { "6", "Kitchen" FONT_ARROW_RIGHT "Power" },
            { "10", "Jugg" FONT_ARROW_RIGHT "Double Tap" },  { "2", "Double Tap" FONT_ARROW_RIGHT "Toilets" }, { "3", "Toilets" FONT_ARROW_RIGHT "Power" } } }
    }},
    { "zm_sumpf", {
        { 0, "Doors", { { "9", "Spawn" FONT_ARROW_RIGHT "Downstairs" }, { "4", "Spawn" FONT_ARROW_RIGHT "Upstairs" }, { "0", "Downstairs" FONT_ARROW_RIGHT "Comm Room Outside" }, { "5", "Comm Room Outside" FONT_ARROW_RIGHT "Comm Room" }, { "3", "Downstairs" FONT_ARROW_RIGHT "Storage Outside" }, { "8", "Storage Outside" FONT_ARROW_RIGHT "Storage" },
            { "2", "Downstairs" FONT_ARROW_RIGHT "Doc Quarters Outside" }, { "6", "Doc Quarters Outside" FONT_ARROW_RIGHT "Doc Quarters" }, { "1", "Downstairs" FONT_ARROW_RIGHT "Fishing Hut Outside" }, { "7", "Fishing Hut Outside" FONT_ARROW_RIGHT "Fishing Hit" } } }
    }},
    { "zm_theater", {
        { 0, "Doors", { { "9", "Spawn" FONT_ARROW_RIGHT "Crematorium" }, { "8", "Crematorium" FONT_ARROW_RIGHT "Alley" }, { "7", "Alley" FONT_ARROW_RIGHT "Trap Room" }, { "0", "Trap Room" FONT_ARROW_RIGHT "Stage" }, { "2", "Spawn" FONT_ARROW_RIGHT "Balcony" },
            { "3", "Balcony" FONT_ARROW_RIGHT "Dining" }, { "6", "Dining" FONT_ARROW_RIGHT "Dressing" }, { "5", "Dressing" FONT_ARROW_RIGHT "Stage" } } },
        { 0, "Teleporter", { { "link_tp", "Link Teleporter" } } }
    }},
    { "zm_cosmodrome", {
        { 0, "Doors", { { "0", "Spawn" FONT_ARROW_RIGHT "Widows" }, { "9", "Widows" FONT_ARROW_RIGHT "Stam" }, { "10", "Stam" FONT_ARROW_RIGHT "Lander" }, { "2", "Spawn" FONT_ARROW_RIGHT "Jug" }, { "4", "Jug" FONT_ARROW_RIGHT "Argus" }, { "3", "Argus" FONT_ARROW_RIGHT "Power" }, { "7", "Argus" FONT_ARROW_RIGHT "PAP" },
            { "8", "Argus" FONT_ARROW_RIGHT "Widows " }, { "1", "Power" FONT_ARROW_RIGHT "Mule Kick" }, { "5", "Mule Kick" FONT_ARROW_RIGHT "Speed" } } },
        { 0, "PAP", { { "open_pap", "Open PAP Door" } } }
    }},
    { "zm_temple", {
        { 0, "Doors", { { "5", "Spawn" FONT_ARROW_RIGHT "L-CAR" }, { "1", "L-CAR" FONT_ARROW_RIGHT "Mule Kick" }, { "8", "Mule Kick" FONT_ARROW_RIGHT "ICR" }, { "7", "ICR" FONT_ARROW_RIGHT "Power" }, { "9", "Spawn" FONT_ARROW_RIGHT "Pharo" }, { "3", "Pharo" FONT_ARROW_RIGHT "Kuda" },
            { "0", "Kuda" FONT_ARROW_RIGHT "KN" }, { "12", "KN" FONT_ARROW_RIGHT "Power" } } },
        { 0, "PAP", { { "open_pap", "Open PAP Stairs" } } }
    }},
    { "zm_moon", {
        { 0, "Doors", { { "16", "Spawn" FONT_ARROW_RIGHT "Outside" }, { "5", "Outside" FONT_ARROW_RIGHT "Tunnel 6 Start" }, { "0", "Tunnel 6 Start" FONT_ARROW_RIGHT "Tunnel 6 End" }, { "13", "Tunnel 6 End" FONT_ARROW_RIGHT "Power" }, { "3", "Outside" FONT_ARROW_RIGHT "Tunnel 11 Starts" }, { "1", "Tunnel 11 Start" FONT_ARROW_RIGHT "Tunnel 11 Mid" }, { "2", "Tunnel 11 Mid" FONT_ARROW_RIGHT "Tunnel 11 End" },
            { "11", "Tunnel 11 End" FONT_ARROW_RIGHT "Power" }, { "9", "Power" FONT_ARROW_RIGHT "Double Tap" }, { "15", "Double Tap" FONT_ARROW_RIGHT "Lab" }, { "7", "Lab" FONT_ARROW_RIGHT "PAP" }, { "20", "Lab" FONT_ARROW_RIGHT "Biodome" }, { "22", "Biodome" FONT_ARROW_RIGHT "PAP" } } }
    }},
    { "zm_tomb", {
        { 0, "Doors", { { "0", "Spawn" FONT_ARROW_RIGHT "Gen 3 Trench" }, { "1", "Gen 3 Trench" FONT_ARROW_RIGHT "Gen 3" }, { "3", "Gen 3" FONT_ARROW_RIGHT "Workshop" }, { "4", "Spawn" FONT_ARROW_RIGHT "Gen 2 Trench" }, { "5", "Gen 2 Trench" FONT_ARROW_RIGHT "Gen 2" }, { "2", "Gen 2" FONT_ARROW_RIGHT "Workshop" },
            { "6", "Workshop" FONT_ARROW_RIGHT "NML" }, { "7", "NML" FONT_ARROW_RIGHT "Gen 5" }, { "9", "NML" FONT_ARROW_RIGHT "Mound" }, { "12", "NML" FONT_ARROW_RIGHT "Tank Station" }, { "11", "Tank Station" FONT_ARROW_RIGHT "Church" } } },
        { 0, "Generators", { { "generator_start_bunker", "Gen 1" }, { "generator_tank_trench", "Gen 2" }, { "generator_mid_trench", "Gen 3" }, { "generator_nml_right", "Gen 4" }, { "generator_nml_left", "Gen 5" }, { "generator_church", "Gen 6" } } }
    }}
};

// Egg Steps Data
struct EggStep {
    bool Completed;
    bool Unlocked;
    std::string Name;
};

struct EggCategory {
    std::string Category;
    std::vector<EggStep> Steps;
};

std::unordered_map<std::string, std::vector<EggCategory>> eggSteps = {
    { "zm_zod", {
        { "Rituals", { { 0, 1, "Magician" }, { 0, 1, "Detective" }, { 0, 1, "Boxer" }, { 0, 1, "Femme" } } },
        { "Eggs", { { 0, 1, "Open Egg" }, { 0, 0, "Rift Egg" }, { 0, 0, "Canals Egg" }, { 0, 0, "Waterfront Egg" }, { 0, 0, "Footlight Egg" } } },
        { "Ovums", { { 0, 0, "Pickup Ovum" }, { 0, 0, "Magician" }, { 0, 0, "Detective" }, { 0, 0, "Boxer" }, { 0, 0, "Femme" }, { 0, 0, "Upgrade Sword" } } },
        { "Flags", { { 0, 0, "Activate Book" }, { 0, 0, "Magician" }, { 0, 0, "Detective" }, { 0, 0, "Boxer" }, { 0, 0, "Femme" } } }
    }},
    { "zm_castle", {
        { "Dragons", { { 0, 1, "Courtyard" }, { 0, 1, "Church" }, { 0, 1, "Undercroft" } } },
        { "Lightning Bow", { { 0, 0, "Start Quest" }, { 0, 0, "Shoot Bonfires" }, { 0, 0, "Wallrun" }, { 0, 0, "Fill Urns" }, { 0, 0, "Forge Arrow" }, { 0, 0, "Upgrade Bow" } } },
        { "Fire Bow", { { 0, 0, "Start Quest" }, { 0, 0, "Shoot Obelisk" }, { 0, 0, "Fill Circles" }, { 0, 0, "Finish Golf" }, { 0, 0, "Forge Arrow" }, { 0, 0, "Upgrade Bow" } } },
        { "Void Bow", { { 0, 0, "Start Quest" }, { 0, 0, "Activate Urn" }, { 0, 0, "Collect Skulls" }, { 0, 0, "Fill Crawlers" }, { 0, 0, "Forge Arrow" }, { 0, 0, "Upgrade Bow" } } },
        { "Wolf Bow", { { 0, 0, "Start Quest" }, { 0, 0, "Collect Skull" }, { 0, 0, "Feed Wolf" }, { 0, 0, "Forge Arrow" }, { 0, 0, "Upgrade Bow" } } },
        { "Time Travel", { { 0, 0, "Time Travel 1" }, { 0, 0, "Time Travel 2" } } },
        { "Simon Says", { { 0, 0, "Simon Says 1" }, { 0, 0, "Simon Says 2" }, { 0, 0, "Call Dempsey" } } },
        { "Keeper", { { 0, 0, "Start Keeper" }, { 0, 0, "Finish Keeper" } } }
    }},
    { "zm_island", {
        { "Skull Quest", { { 0, 1, "Ritual 1" }, { 0, 1, "Ritual 2" }, { 0, 1, "Ritual 3" }, { 0, 1, "Ritual 4" }, { 0, 0, "Skull Lockdown" } } },
        { "Challenges" , { { 0, 1, "Challenge 1" }, { 0, 1, "Challenge 2" }, { 0, 1, "Challenge 3" } } },
        { "Plane", { { 0, 0, "Poster" }, { 0, 1, "Bullet" }, { 0, 0, "Shoot Plane" } } },
        { "Gears", { { 0, 0, "Lab A Gear" }, { 0, 0, "Zipline Gear" }, { 0, 0, "Plane Gear" }, { 0, 0, "Activate Elevator" } } }
    }},
    { "zm_stalingrad", {
        { "PAP Grophs", { { 0, 1, "DC Groph" }, { 0, 0, "Supply Groph" }, { 0, 0, "Tank Groph" } } },
        { "Gauntlet", { { 0, 0, "Pickup Egg" }, { 0, 0, "Awaken Egg" }, { 0, 0, "Napalm Kills" }, { 0, 0, "Multi Kills" }, { 0, 0, "Melee Kills" }, { 0, 0, "Incubation Lockdown" },
            { 0, 0, "Incubate Egg" }, { 0, 0, "Pickup Gauntlet" } } },
        { "Main EE", { { 0, 0, "Valves" }, { 0, 0, "Password" }, { 0, 0, "Pickup Trophies" }, { 0, 0, "Challenges" }, { 0, 0, "Deliver Power Core" } } }
    }},
    { "zm_genesis", {
        { "Keeper Protector", { { 0, 1, "Shoot Tombstones" }, { 0, 0, "Pickup Head" }, { 0, 0, "Pickup Totem" }, { 0, 0, "Pickup Gem" }, { 0, 0, "Keeper Ritual" } } },
        { "Apothicon Rocks", { { 0, 1, "Spawn" }, { 0, 1, "DE" }, { 0, 1, "Verruckt" }, { 0, 1, "Mob" }, { 0, 1, "Nacht" } } },
        { "Arnies", { { 0, 0, "Set 1" }, { 0, 0, "Set 2" }, { 0, 0, "Set 3" } } },
        { "Bones", { { 0, 0, "Spawn" }, { 0, 0, "DE" }, { 0, 0, "Verruckt" }, { 0, 0, "Origins" }, { 0, 0, "Nacht" }, { 0, 0, "Pickup Kronorium" } } },
        { "Eggs", { { 0, 0, "Spawn" }, { 0, 0, "DE" }, { 0, 0, "Verruckt" }, { 0, 0, "Mob" } } },
        { "Symbols", { { 0, 0, "Spawn" }, { 0, 0, "DE" }, { 0, 0, "Verruckt" }, { 0, 0, "Mob" } } },
        { "Ending", { { 0, 0, "Boss 1" }, { 0, 0, "Basketball" } } }
    }}
};

// Zombie Calculator Data
int calcRound = 1;
std::string calcBasicHealth;
std::vector<std::string> calcBasicZombieCount;
std::vector<std::string> calcBasicZombiesUpToCount;
std::vector<std::string> calcBasicHordeCount;
std::vector<std::string> calcBasicSpawnRate;
std::vector<std::string> calcBasicExpectedRoundTime;

std::string ParseMilliToTime(int milliseconds)
{
    int hours = milliseconds / 3600000;
    milliseconds %= 3600000;
    int minutes = milliseconds / 60000;
    milliseconds %= 60000;
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    std::stringstream ss;

    if (hours > 0) {
        ss << std::setw(2) << std::setfill('0') << hours << ":";
    }
    if (minutes > 0) {
        ss << std::setw(2) << std::setfill('0') << minutes << ":";
    }
    ss << std::setw(2) << std::setfill('0') << seconds << ".";
    ss << std::setw(2) << std::setfill('0') << milliseconds;

    return ss.str();
}

int MaxZombieFunc(int maxZombies, int round)
{
    if (round >= 6) {
        return maxZombies;
    }
    if (round < 2) {
        return (int)(maxZombies * 0.25f);
    }
    if (round < 3) {
        return (int)(maxZombies * 0.3f);
    }
    if (round < 4) {
        return (int)(maxZombies * 0.5f);
    }
    if (round < 5) {
        return (int)(maxZombies * 0.7f);
    }
    if (round < 6) {
        return (int)(maxZombies * 0.9f);
    }
    return maxZombies;
}

int GetZombieCountForRound(int round, size_t playerCount)
{
    int maxZombies = 24;
    float multiplier = std::max(1.0f, round / 5.0f);
    if (round >= 10) {
        multiplier *= (round * 0.15f);
    }

    if (playerCount == 1) {
        maxZombies += (int)(3 * multiplier);
    }
    else {
        maxZombies += (int)(((playerCount - 1) * 6) * multiplier);
    }

    return MaxZombieFunc(maxZombies, round);
}

int GetZombiesUpToRound(int round, size_t playerCount)
{
    if (round == 1) {
        return 0;
    }
    return GetZombieCountForRound(round - 1, playerCount) + GetZombiesUpToRound(round - 1, playerCount);
}

int GetZombieHealth(int round)
{
    if (round > 162) {
        return INT_MAX;
    }

    int zombieHealth = 150;

    if (round < 10) {
        zombieHealth += 100 * (round - 1);
        return zombieHealth;
    }

    float multiplier = 0.1f;
    int baseIncrease = 100 * (9 - 1);
    zombieHealth += baseIncrease;
    for (size_t i = 10; i <= round; i++) {
        zombieHealth += (int)(zombieHealth * multiplier);
    }

    return zombieHealth;
}

int RoundTimeNearest50(int time)
{
    int newTime = time % 50;
    if (newTime < 25) {
        newTime = time - newTime;
    }
    else {
        newTime = time + (50 - newTime);
    }
    return newTime;
}

int GetSpawnRate(int round, size_t playerCount)
{
    if (round == 1) {
        return 2100;
    }
    float startDelay = 2.0f;
    if (playerCount == 2) {
        startDelay = 1.5f;
    }
    else if (playerCount == 3) {
        startDelay = 0.89f;
    }
    else if (playerCount == 4) {
        startDelay = 0.67f;
    }
    float calcSpawnRate = std::max(0.2f, startDelay * (float)(pow(0.95f, round - 1)) + 0.1f);
    int calcRoundedTime = (int)(calcSpawnRate * 1000);
    return RoundTimeNearest50(calcRoundedTime);
}

float RawSpawnRate(int round, size_t playerCount)
{
    if (round == 1) {
        return 2.1f;
    }
    float startDelay = 2.0f;
    if (playerCount == 2) {
        startDelay = 1.5f;
    }
    else if (playerCount == 3) {
        startDelay = 0.89f;
    }
    else if (playerCount == 4) {
        startDelay = 0.67f;
    }
    return startDelay * (float)(pow(0.95f, round - 1));
}

void CalcZombieCalcData()
{

}

// Valve Solver Data
std::unique_ptr<Walnut::Image> g_ValveImage;

struct Valve {
    std::string Name;
    std::vector<std::string> Probability1;
    std::vector<std::string> Probability2;
    std::unordered_map<std::string, std::vector<size_t>> Solutions1;
    std::unordered_map<std::string, std::vector<size_t>> Solutions2;
    bool Direction1 = false;
    bool Direction2 = false;
    bool Direction3 = false;
    bool Green = false;
    bool CheckedGreen = false;
    bool Password = false;

    bool operator==(const Valve& valve)
    {
        return valve.Name == Name;
    }
};

Valve dept = {
    "Department", { "40%", "28%", "32%" }, { "32%", "36%", "32%" },
    { { "Dragon Command", { 0, 1, 2, 2, 1, 1 } }, { "Armory", { 0, 2, 1, 3, 3, 1 } }, { "Supply", { 0, 3, 3, 1, 2, 3 } }, { "Infirmary", { 0, 2, 1, 1, 2, 3 } }, { "Tank", { 0, 1, 3, 2, 3, 1 } } },
    { { "Dragon Command", { 0, 3, 3, 2, 2, 2 } }, { "Armory", { 0, 3, 2, 1, 1, 2 } }, { "Supply", { 0, 2, 2, 2, 3, 1 } }, { "Infirmary", { 0, 1, 3, 3, 3, 3 } }, { "Tank", { 0, 3, 1, 1, 1, 3 } } }
};
Valve dc = {
    "Dragon Command", { "48%", "32%", "20%" }, { "24%", "32%", "44%" },
    { { "Department", { 1, 0, 1, 3, 3, 1 } }, { "Armory", { 3, 0, 1, 3, 1, 1 } }, { "Supply", { 3, 0, 2, 2, 1, 1 } }, { "Infirmary", { 1, 0, 2, 1, 1, 2 } }, { "Tank", { 1, 0, 1, 1, 1, 1 } } },
    { { "Department", { 2, 0, 1, 1, 2, 3 } }, { "Armory", { 2, 0, 3, 1, 2, 2 } }, { "Supply", { 2, 0, 3, 3, 3, 3 } }, { "Infirmary", { 3, 0, 3, 2, 2, 2 } }, { "Tank", { 2, 0, 3, 2, 3, 2 } } }
};
Valve arm = {
    "Armory", { "36%", "40%", "24%" }, { "28%", "24%", "48%" },
    { { "Department", { 2, 1, 0, 3, 3, 3 } }, { "Dragon Command", { 1, 1, 0, 3, 1, 1 } }, { "Supply", { 2, 2, 0, 1, 2, 3 } }, { "Infirmary", { 3, 1, 0, 3, 1, 3 } }, { "Tank", { 3, 1, 0, 2, 1, 1 } } },
    { { "Department", { 3, 3, 0, 2, 2, 2 } }, { "Dragon Command", { 2, 2, 0, 2, 2, 2 } }, { "Supply", { 1, 2, 0, 3, 3, 1 } }, { "Infirmary", { 1, 2, 0, 1, 2, 2 } }, { "Tank", { 1, 3, 0, 1, 1, 2 } } }
};
Valve supply = {
    "Supply", { "44%", "28%", "28%" }, { "28%", "36%", "36%" },
    { { "Department", { 1, 1, 2, 0, 3, 1 } }, { "Dragon Command", { 2, 2, 1, 0, 2, 3 } }, { "Armory", { 3, 1, 2, 0, 1, 1 } }, { "Infirmary", { 3, 1, 3, 0, 2, 3 } }, { "Tank", { 1, 2, 1, 0, 3, 1 } } },
    { { "Department", { 3, 3, 1, 0, 2, 3 } }, { "Dragon Command", { 1, 3, 2, 0, 1, 2 } }, { "Armory", { 3, 3, 3, 0, 2, 2 } }, { "Infirmary", { 1, 2, 2, 0, 3, 2 } }, { "Tank", { 2, 1, 3, 0, 3, 3 } } }
};
Valve inf = {
    "Infirmary", { "44%", "32%", "24%" }, { "20%", "40%", "40%" },
    { { "Department", { 1, 3, 2, 1, 0, 2 } }, { "Dragon Command", { 2, 1, 3, 3, 0, 3 } }, { "Armory", { 2, 2, 2, 1, 0, 2 } }, { "Supply", { 1, 2, 2, 1, 0, 1 } }, { "Tank", { 3, 3, 3, 2, 0, 2 } } },
    { { "Department", { 3, 1, 2, 2, 0, 1 } }, { "Dragon Command", { 1, 2, 1, 3, 0, 1 } }, { "Armory", { 3, 1, 3, 3, 0, 1 } }, { "Supply", { 3, 3, 3, 3, 0, 3 } }, { "Tank", { 2, 2, 1, 1, 0, 3 } } }
};
Valve tank = {
    "Tank", { "52%", "16%", "32%" }, { "20%", "48%", "32%" },
    { { "Department", { 1, 3, 1, 1, 2, 0 } }, { "Dragon Command", { 2, 1, 3, 2, 2, 0 } }, { "Armory", { 2, 2, 1, 1, 2, 0 } }, { "Supply", { 1, 3, 2, 1, 1, 0 } }, { "Infirmary", { 3, 1, 2, 2, 1, 0 } } },
    { { "Department", { 2, 1, 2, 2, 3, 0 } }, { "Dragon Command", { 1, 3, 1, 3, 1, 0 } }, { "Armory", { 2, 1, 3, 3, 3, 0 } }, { "Supply", { 3, 3, 3, 2, 2, 0 } }, { "Infirmary", { 1, 2, 1, 3, 3, 0 } } }
};

std::vector<Valve*> valves = { &dept, &dc, &arm, &supply, &inf, &tank };
Valve* greenValve = nullptr;
Valve* passwordValve = nullptr;
std::vector<bool> notGreenValves = { false, false, false, false, false, false };
std::vector<std::string> valveLocations = { "Department", "Dragon Command", "Armory", "Supply", "Infirmary", "Tank" };
std::vector<std::pair<std::string, std::vector<std::string>>> possibleSolutions1;
std::vector<std::pair<std::string, std::vector<std::string>>> possibleSolutions2;

void CalcValveProbabilities()
{
    std::vector<std::vector<size_t>> counts1 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } ;
    std::vector<std::vector<size_t>> counts2 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

    for (const Valve* valve : valves) {
        // If password found and current valve is not password -> continue
        if (passwordValve && *valve != *passwordValve) {
            continue;
        }

        for (size_t j = 0; j < 6; j++) {
            const std::string& location = valveLocations[j];
            // Valve can't check itself for green
            // Green found and current valve not green
            // Valve already checked for green
            if (location == valve->Name || greenValve && greenValve->Name != location || notGreenValves[j]) {
                continue;
            }
            auto it = valve->Solutions1.find(location);
            if (it == valve->Solutions1.end()) {
                continue;
            }

            for (size_t k = 0; k < 6; k++) {
                if (it->second[k]) {
                    counts1[k][it->second[k] - 1]++;
                }
            }

            it = valve->Solutions2.find(location);
            if (it == valve->Solutions2.end()) {
                continue;
            }

            for (size_t k = 0; k < 6; k++) {
                if (it->second[k]) {
                    counts2[k][it->second[k] - 1]++;
                }
            }
        }
    }
    // Calc percentages + string manipulation
    for (size_t i = 0; i < 6; i++) {
        const std::string& location = valveLocations[i];
        const size_t total = counts1[i][0] + counts1[i][1] + counts1[i][2];

        for (size_t j = 0; j < 3; j++) {
            if (!counts1[i][j]) {
                valves[i]->Probability1[j] = "0%";
            }
            else {
                float data = counts1[i][j] * 100.0f / total;
                char buf[8];
                sprintf_s(buf, "%.2f", data);
                std::string percentage = buf;
                size_t dot = percentage.find('.');
                if (dot != std::string::npos && dot + 3 < percentage.size()) {
                    percentage = percentage.substr(0, dot + 3);
                }
                while (percentage.at(percentage.size() - 1) == '0') {
                    percentage.erase(percentage.size() - 1);
                }
                if (percentage.find('.') == percentage.size() - 1) {
                    percentage.erase(percentage.size() - 1);
                }
                valves[i]->Probability1[j] = percentage + "%";
            }
            if (!counts2[i][j]) {
                valves[i]->Probability2[j] = "0%";
            }
            else {
                float data = counts2[i][j] * 100.0f / total;
                char buf[8];
                sprintf_s(buf, "%.2f", data);
                std::string percentage = buf;
                size_t dot = percentage.find('.');
                if (dot != std::string::npos && dot + 3 < percentage.size() - 1) {
                    percentage = percentage.substr(0, dot + 3);
                }
                while (percentage.at(percentage.size() - 1) == '0') {
                    percentage.erase(percentage.size() - 1);
                }
                if (percentage.find('.') == percentage.size() - 1) {
                    percentage.erase(percentage.size() - 1);
                }
                valves[i]->Probability2[j] = percentage + "%";
            }
        }
    }
}

void CalcValvePossibleSolutions()
{
    possibleSolutions1.clear();
    possibleSolutions2.clear();

    if (!passwordValve) {
        return;
    }

    for (size_t i = 0; i < 6; i++) {
        const std::string& location = valveLocations[i];
        std::vector<std::string> directions1;
        std::vector<std::string> directions2;

        // Password valve is checking for green as well
        // Green found and current valve is not green
        // Current valve not green
        if (passwordValve->Name == location || greenValve && greenValve->Name != location || notGreenValves[i]) {
            continue;
        }

        for (size_t j = 0; j < 6; j++) {
            const std::vector<size_t>& combination1 = passwordValve->Solutions1[location];
            const std::vector<size_t>& combination2 = passwordValve->Solutions2[location];

            if (combination1[j]) {
                directions1.emplace_back((std::to_string(combination1[j])));
            }
            else {
                directions1.emplace_back("P");
            }
            if (combination2[j]) {
                directions2.emplace_back((std::to_string(combination2[j])));
            }
            else {
                directions2.emplace_back("P");
            }
        }

        possibleSolutions1.emplace_back(std::make_pair(location, directions1));
        possibleSolutions2.emplace_back(std::make_pair(location, directions2));
    }
}

namespace BO3PracticeTool
{
    struct TabBody {
        std::function<void()> BodyRender;
    };
    // Frontend Data
    void BGBPresetsBody();
    void WeaponPresetsBody();
    void AutosplitPresetsBody();
    void PracticePatchesBody();
    // In Game Options
    void PlayerOptionsBody();
    void ZombieOptionsBody();
    void RoundOptionsBody();
    void PowerupOptionsBody();
    void EggStepOptionsBody();
    void CraftableOptionsBody();
    void BlockerOptionsBody();
    void MapOptionsBody();
    // Resources
    void GumTrackerBody();
    void ZombieCalculatorBody();
    void CodeGuidesBody();
    void GKValveSolverBody();

    const std::vector<std::vector<TabBody>> tabBodies = {
        { { BGBPresetsBody }, { WeaponPresetsBody }, { AutosplitPresetsBody }, { PracticePatchesBody } },
        { { PlayerOptionsBody }, { ZombieOptionsBody } },
        { { GumTrackerBody }, { ZombieCalculatorBody }, { CodeGuidesBody }, { GKValveSolverBody } },
    };

    void OnInit()
    {
        uint32_t w, h;
        void* data = Walnut::Image::Decode(g_ValveGauge.data(), g_ValveGauge.size(), w, h);
        g_ValveImage = std::make_unique<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data);
    }

    void OnShutdown()
    {
        g_ValveImage.reset();
    }

    // TODO: Add logging on fail
    void RenderWindowBody(size_t TabGroup, size_t TabItem)
    {
        if (validMap && TabGroup == 1) {
            ImGui::SameLine();
            Walnut::UI::ShiftCursorX(ImGui::GetContentRegionAvail().x - 300.0f);
            if (ImGui::Button("Load Map")) {
                NotifyGame(std::format("game map-{}", mapName));
            }
            ImGui::ItemTooltip("Load the selected map");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::BeginCombo("##Map Selection", t7Maps[mapSelectIndex].c_str())) {
                ImGui::Selection(t7Maps, mapSelectIndex);
                ImGui::EndCombo();
            }
            ImGui::ItemTooltip("Choose a map to load");
        }

        tabBodies[TabGroup][TabItem].BodyRender();
    }

    // Frontend Data

    void BGBPresetsBody()
    {
        BGBPreset* currentPreset = BGBPreset::GetCurrentPreset();

        if (currentPreset && Memory::injected) {
            ImGui::SameLine();
            Walnut::UI::ShiftCursorX(ImGui::GetContentRegionAvail().x - 110.0f);
            if (ImGui::ToggleButton("BGB Presets Active", "Active", &bgbPresetsActive)) {
                BGBPreset::WritePresetToGame();
            }
        }

        float width = ImGui::GetContentRegionAvail().x;
        static float editorWidth = width * 0.2f;
        static float bodyWidth = width * 0.8f;
        ImVec2 editorSize(editorWidth, 0);
        ImGuiStyle style = ImGui::GetStyle();
        ImGui::Splitter("##Files Split", true, 8.0f, &editorWidth, &bodyWidth, 250.0f, 500.0f);

        if (ImGui::BeginChild("BGBPresets", editorSize)) {
            ImGui::HelpMarker("Right click to popup a menu. This section is essentially a file explorer.");
            bgbFiles.Render();
            ImGui::EndChild();
        }

        if (!currentPreset) {
            return;
        }

        ImGui::SameLine();
        Walnut::UI::ShiftCursorX(10.0f);
        ImGui::BeginGroup();

        float heightAvail = ImGui::GetContentRegionAvail().y - style.ItemSpacing.y * 4;
        ImVec2 imageSize(heightAvail / 5.0f, heightAvail / 5.0f);
        if (!bgbSelection) {
            for (size_t i = 0; i < 5; i++) {
                const BGB* bgb = currentPreset->GetBGB(i);
                const ImTextureID& image = bgbImages[bgb->Name].Image->GetDescriptorSet();
                if (bgbMainGums.Render(static_cast<int8_t>(i), image, imageSize).Hovered) {
                    bgbContextGum = bgb->Name;
                }
            }
        }

        ImGui::EndGroup();

        ImGui::SameLine();
        Walnut::UI::ShiftCursor(30.0f, -40.0f);
        ImGui::BeginGroup();

        float widthAvail = ImGui::GetContentRegionAvail().x - style.ItemSpacing.x * 6 - style.ScrollbarSize;
        if (ImGui::GetScrollMaxY() > 0) {
            widthAvail += style.ScrollbarSize;
        }
        imageSize = imageSize * 0.8f;
        int numItems = static_cast<int>(ImGui::GetContentRegionAvail().x / (imageSize.x + style.ItemSpacing.x));
        if (!numItems) {
            numItems = 1;
        }

        ImGui::TextBold("Currently Editing: %s", currentPreset->GetPresetName().data());
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();

        static std::string searchText;
        ImGui::Text("Gum Search");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(widthAvail * 0.4f);
        if (ImGui::InputText("##Gum Search", &searchText)) {
            bgbDisplayImages = BGBSearch(bgbDisplayImages, searchText);
        }
        ImGui::SameLine();
        ImGui::Text("Filter");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::BeginCombo("##Gum Filters", bgbFilters[bgbFilterIndex].c_str())) {
            if (ImGui::Selection(bgbFilters, bgbFilterIndex)) {
                bgbSelectionFilter = static_cast<BGBFilter>(bgbFilterIndex);
                std::sort(bgbDisplayImages.begin(), bgbDisplayImages.end(), CompareImagePointer);
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginTabBar("Gum Selection Menus")) {
            if (ImGui::TabItemButton("Classics")) {
                bgbClassicSelection = true;
                bgbDisplayImages = BGBSearch(bgbDisplayImages, searchText);
            }
            if (ImGui::TabItemButton("Megas")) {
                bgbClassicSelection = false;
                bgbDisplayImages = BGBSearch(bgbDisplayImages, searchText);
            }
            ImGui::EndTabBar();
        }

        float height = ImGui::GetContentRegionAvail().y;
        float minSelectionHeight = std::max(height * 0.6f, height - 192);
        static float previousHeight = ImGui::GetContentRegionAvail().y;
        static float selectionHeight = height * 0.8f;
        static float contextHeight = height * 0.2f;
        if (height != previousHeight) {
            selectionHeight = std::max(minSelectionHeight, selectionHeight * (height / previousHeight));
            contextHeight = height - selectionHeight;
            previousHeight = height;
        }
        ImVec2 selectionSize(-1, selectionHeight);
        ImGui::Splitter("##Selection Split", false, 8.0f, &selectionHeight, &contextHeight, minSelectionHeight, 50.0f);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_WindowBg]);
        if (ImGui::BeginChild("Scrollable Gum Selection", ImVec2(0, selectionHeight))) {
            for (size_t i = 0; i < bgbDisplayImages.size(); i++) {
                const BGBImage& image = *bgbDisplayImages[i];
                if (ImGui::ImageButton(image.Image->GetDescriptorSet(), imageSize, ImVec2(0, 0), ImVec2(1, 1), 0)) {
                    size_t index = bgbMainGums.GetSelected();
                    if (image.BGBData != currentPreset->GetBGB(index)) {
                        currentPreset->SetBGB(index, currentPreset->GetBGB(index), image.BGBData);
                        currentPreset->SavePreset();
                    }
                }
                if (bgbContextGum != image.BGBData.Name && ImGui::IsItemHovered()) {
                    bgbContextGum = image.BGBData.Name;
                }
                if ((i + 1) % numItems || !i) {
                    ImGui::SameLine();
                }
            }
            ImGui::EndChild();
        }

        Walnut::UI::ShiftCursorY(5.0f);
        ImGui::GetCursorPos();
        float contextSize = ImGui::GetContentRegionAvail().y;
        ImGui::Image(bgbImages[bgbContextGum].Image->GetDescriptorSet(), ImVec2(contextSize, contextSize));
        ImGui::SameLine();
        if (ImGui::BeginChild("Gum Context")) {
            const BGB& bgb = bgbImages[bgbContextGum].BGBData;
            ImGui::TextBold(bgb.Name.c_str());
            ImGui::TextWrapped(bgb.Description.c_str());
            ImGui::EndChild();
        }

        ImGui::PopStyleColor();
        ImGui::EndGroup();
    }

    // Weapon Body UI
    void WeaponPresetsBody()
    {
        WeaponPreset* currentPreset = WeaponPreset::GetCurrentPreset();

        if (currentPreset && Memory::injected) {
            ImGui::SameLine();
            Walnut::UI::ShiftCursorX(ImGui::GetContentRegionAvail().x - 110.0f);
            if (ImGui::ToggleButton("Weapon Presets Active", "Active", &weaponPresetsActive)) {
                WeaponPreset::WritePresetToGame();
            }
        }

        float width = ImGui::GetContentRegionAvail().x;
        static float editorWidth = width * 0.2f;
        static float bodyWidth = width * 0.8f;
        ImVec2 editorSize(editorWidth, 0);
        ImGuiStyle style = ImGui::GetStyle();
        ImGui::Splitter("##Files Split", true, 8.0f, &editorWidth, &bodyWidth, 250.0f, 750.0f);

        if (ImGui::BeginChild("WeaponPresets", editorSize)) {
            ImGui::HelpMarker("Right click to popup a menu. This section is essentially a file explorer.");
            weaponFiles.Render();
            ImGui::EndChild();
        }

        if (!currentPreset) {
            return;
        }

        ImGui::SameLine();
        Walnut::UI::ShiftCursor(30.0f, -40.0f);

        if (weaponCamoSelection) {
            ImGui::BeginGroup();

            if (ImGui::BackButton()) {
                weaponCamoSelection = false;
            }
            ImGui::SameLine();

            if (ImGui::BeginTabBar("Camo Selection Menu")) {
                for (size_t i = 0; i < camoTypes.size(); i++) {
                    const std::string& type = camoTypes[i];
                    if (ImGui::TabItemButton(type.c_str())) {
                        weaponCamoMenu = i;
                    }
                }
                ImGui::EndTabBar();
            }

            float widthAvail = ImGui::GetContentRegionAvail().x - style.ScrollbarSize;
            float imageWidth = std::min(widthAvail / 7 - style.ItemSpacing.x, 192.0f) - style.FramePadding.x * 2;
            ImVec2 imageSize(imageWidth, imageWidth);
            int numItems = static_cast<int>(widthAvail / (imageSize.x + style.FramePadding.x * 2));

            ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_WindowBg]);
            if (ImGui::BeginChild("Scrollable Camo Selection", ImVec2(0, ImGui::GetContentRegionAvail().y))) {
                for (size_t i = 0; i < camoList[weaponCamoMenu].size(); i++) {
                    const std::string& name = camoList[weaponCamoMenu][i];
                    size_t index = camoNameToIndex[name];
                    const ImTextureID& camo = camosImgList[name]->GetDescriptorSet();

                    if (ImGui::ImageButton(camo, imageSize)) {
                        weaponCamoSelection = false;
                        WeaponItem& item = currentPreset->GetWeaponItem(weaponTypes[weaponPresetsMenu], weaponBeingEdited);
                        item.Camo = static_cast<int16_t>(index);
                        currentPreset->SavePreset();
                    }
                    ImVec2 last(ImGui::GetItemRectMin().x + 10, ImGui::GetItemRectMax().y - 15);
                    last.y -= ImGui::CalcTextSize(name.c_str()).y;
                    ImGui::TextBackground(last, name);
                    if ((i + 1) % numItems || !i) {
                        ImGui::SameLine();
                    }
                }

            }
            ImGui::EndChild();
            ImGui::PopStyleColor();
            return ImGui::EndGroup();
        }

        ImGui::BeginGroup();
        ImGui::TextBold("Currently Editing: %s", currentPreset->GetPresetName().data());
        ImGui::BeginGroup();

        if (ImGui::BeginTabBar("Weapon Type Menus")) {
            for (size_t i = 0; i < 6; i++) {
                const std::string& type = weaponTypes[i];
                if (ImGui::TabItemButton(type.c_str())) {
                    weaponPresetsMenu = i;
                    weaponBeingEdited = 0;
                    weaponMainWeapons.SetSelected(0);
                    WeaponPreset::ResetWeaponSelections();
                }
            }
            ImGui::EndTabBar();
        }

        bool removePressed = ImGui::IsKeyPressed(ImGuiKey_R, false);

        const std::string& type = weaponTypes[weaponPresetsMenu];
        WeaponItem& item = currentPreset->GetWeaponItem(type, weaponBeingEdited);
        const Weapon& weapon = menuWeapons[weaponPresetsMenu][weaponBeingEdited];
        const std::string& name = weapon.Name;
        const size_t& numAttachments = weapon.NumAttachments;
        const std::vector<Attachment>& optics = weapon.Optics;
        const std::vector<Attachment>& attachments = weapon.Attachments;

        float widthAvail = ImGui::GetContentRegionAvail().x;
        float imageWidth = std::min(widthAvail / 7 - style.ItemSpacing.x, 192.0f);
        float heightAvail = ImGui::GetContentRegionAvail().y - 50.0f;
        if (heightAvail / 4 < imageWidth) {
            imageWidth = heightAvail / 4;
        }
        ImVec2 imageSize(imageWidth, imageWidth);
        // Optics selection
        for (size_t i = 0; i < optics.size(); i++) {
            const ImTextureID& image = opticsImgList[optics[i].Name]->GetDescriptorSet();
            const std::string opticName = optics[i].Name;
            if (weaponOpticSelection.Render(static_cast<int8_t>(i), image, imageSize, true).Pressed) {
                item.Optic = weaponOpticSelection.GetSelected();
                if (type == "Sniper Rifles" && std::find(item.Attachments.begin(), item.Attachments.end(), 0) != item.Attachments.end()) {
                    item.Attachments.erase(std::find(item.Attachments.begin(), item.Attachments.end(), 0));
                    weaponAttachmentsSelection.Deselect(0);
                }
                currentPreset->SavePreset();
            }
            if (ImGui::IsItemHovered() && removePressed && weaponOpticSelection.GetSelected() == i) {
                weaponOpticSelection.Deselect();
                item.Optic = -1;
                currentPreset->SavePreset();
            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(opticName.c_str()).y;
            ImGui::TextBackground(last, opticName);
            if (i < optics.size() - 1) {
                ImGui::SameLine();
            }
        }
        // Attachments selection
        for (size_t i = 0; i < attachments.size(); i++) {
            const ImTextureID& attachment = attachmentsImgList[std::format("{}_{}", name, attachments[i].Name)]->GetDescriptorSet();
            const std::string attachmentName = attachments[i].Name;
            if (weaponAttachmentsSelection.Render(i, attachment, imageSize).Pressed) {
                item.Attachments = weaponAttachmentsSelection.GetSelected();
                if (attachmentName == "Ballistics CPU") {
                    item.Optic = -1;
                    weaponOpticSelection.Deselect();
                }
                currentPreset->SavePreset();
            }
            if (ImGui::IsItemHovered() && removePressed && std::find(item.Attachments.begin(), item.Attachments.end(), i) != item.Attachments.end()) {
                weaponAttachmentsSelection.Deselect(i);
                item.Attachments = weaponAttachmentsSelection.GetSelected();
                currentPreset->SavePreset();
            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(attachmentName.c_str()).y;
            ImGui::TextBackground(last, attachmentName);
            if (i < attachments.size() - 1) {
                ImGui::SameLine();
            }
        }
        // Camo selection
        if (item.Camo != -1) {
            const std::string& name = camoIndexToName[item.Camo];
            if (ImGui::ImageButton(camosImgList[name]->GetDescriptorSet(), imageSize - ImVec2(style.FramePadding.x * 2, style.FramePadding.y * 2))) {
                weaponCamoSelection = true;
            }
            if (ImGui::IsItemHovered() && removePressed && item.Camo != -1) {
                item.Camo = -1;
                currentPreset->SavePreset();
            }
            ImVec2 last(ImGui::GetItemRectMin().x + 10, ImGui::GetItemRectMax().y - 15);
            last.y -= ImGui::CalcTextSize(name.c_str()).y;
            ImGui::TextBackground(last, name.c_str());
        }
        else {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            if (ImGui::Button("##CamoButton", imageSize)) {
                weaponCamoSelection = true;
            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize("Choose Camo").y;
            ImGui::TextBackground(last, "Choose Camo");
            ImGui::PopStyleVar();
        }
        // Controls
        if (ImGui::Button("Clear All")) {
            item.Optic = -1;
            item.Attachments.clear();
            item.Camo = -1;
            weaponOpticSelection.Deselect();
            weaponAttachmentsSelection.ClearAll();
            currentPreset->SavePreset();
        }
        ImGui::SameLine();
        ImGui::Image(keyboardIcons[ImGuiKey_R]->GetDescriptorSet(), ImVec2(30, 30));
        ImGui::SameLine();
        ImGui::Text("Remove");

        Walnut::UI::ShiftCursorY(ImGui::GetContentRegionAvail().y - imageSize.y);
        // Weapon selection
        const std::vector<Weapon>& weapons = menuWeapons[weaponPresetsMenu];
        for (size_t i = 0; i < weapons.size(); i++) {
            const ImTextureID& weapon = weaponIconsImgList[weapons[i].Name]->GetDescriptorSet();
            const std::string weapontName = weapons[i].Name;
            if (weaponMainWeapons.Render(static_cast<int8_t>(i), weapon, imageSize).Pressed) {
                weaponBeingEdited = i;
                WeaponPreset::ResetWeaponSelections();
            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(weapontName.c_str()).y;
            ImGui::TextBackground(last, weapontName);
            if (i < weapons.size() - 1) {
                ImGui::SameLine();
            }
        }

        ImGui::EndGroup();
        ImGui::EndGroup();
    }

    // Autosplit Body UI
    // TODO: REWORK DRAG SYSTEM
    void AutosplitPresetsBody()
    {
        AutosplitPreset* currentPreset = AutosplitPreset::GetCurrentPreset();

        if (currentPreset && Memory::injected) {
            ImGui::SameLine();
            Walnut::UI::ShiftCursorX(ImGui::GetContentRegionAvail().x - 110.0f);
            if (ImGui::ToggleButton("Autosplit Presets Active", "Active", &autosplitPresetsActive)) {
                AutosplitPreset::WritePresetToGame();
                LOG_DEBUG("Writing to game");
            }
        }

        float width = ImGui::GetContentRegionAvail().x;
        static float editorWidth = width * 0.2f;
        static float bodyWidth = width * 0.8f;
        ImVec2 editorSize(editorWidth, 0);
        ImGuiStyle style = ImGui::GetStyle();
        ImGui::Splitter("##Files Split", true, 8.0f, &editorWidth, &bodyWidth, 250.0f, 800.0f);

        if (ImGui::BeginChild("AutosplitPresets", editorSize)) {
            ImGui::HelpMarker("Right click to popup a menu. This section is essentially a file explorer.");
            autosplitFiles.Render();
            ImGui::EndChild();
        }

        if (!currentPreset) {
            return;
        }

        AutosplitData& currentData = currentPreset->GetData();

        ImGui::SameLine();
        Walnut::UI::ShiftCursor(30.0f, -40.0f);
        ImGui::BeginGroup();

        ImGui::TextBold("Currently Editing: %s", currentPreset->GetPresetName().data());

        float widthAvail = ImGui::GetContentRegionAvail().x - style.ScrollbarSize;

        // MapNum
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::BeginCombo("##Autosplit Map Select", t7Maps[currentData.MapNum].c_str())) {
            size_t previousMap = currentData.MapNum;
            if (ImGui::Selection(t7Maps, currentData.MapNum)) {
                // save
                autosplitAddType = 0;
                if (currentData.NumSplits) {
                    ImGui::OpenOverridePopup("Autosplit Map Change Confirmation");
                    autosplitMapChangeNum = currentData.MapNum;
                    currentData.MapNum = previousMap;
                }
                else {
                    currentPreset->SavePreset();
                }
            }
            ImGui::EndCombo();
        }
        // SplitType
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::BeginCombo("##Autosplit Type", autosplitTypes[currentData.SplitType].c_str())) {
            if (ImGui::Selection(autosplitTypes, currentData.SplitType)) {
                currentPreset->SavePreset();
            }
            ImGui::EndCombo();
        }
        // IGT
        ImGui::SameLine();
        if (ImGui::Checkbox("In Game Timer", &currentData.IGT)) {
            currentPreset->SavePreset();
        }
        // IGRT
        ImGui::SameLine();
        if (ImGui::Checkbox("In Game Round Timer", &currentData.IGRT)) {
            currentPreset->SavePreset();
        }

        width = ImGui::GetContentRegionAvail().x;
        float height = ImGui::GetContentRegionAvail().y;
        static float addHeight = height * 0.5f;
        static float infoHeight = height * 0.5f;
        ImGui::Splitter("##Add - Info Split", false, 8.0f, &addHeight, &infoHeight, 50.0f, 50.0f);

        if (ImGui::BeginChild("Add Split", ImVec2(0, addHeight))) {
            size_t amount = autosplitMenuSplits[currentData.MapNum].size();
            if (ImGui::BeginListBox("##Add Splits Type", ImVec2(width * 0.5f - style.ItemSpacing.x, addHeight))) {
                for (size_t i = 0; i < amount; i++) {
                    const auto& type = autosplitMenuSplits[currentData.MapNum][i];
                    const bool selected = autosplitAddType == i;
                    if (ImGui::Selectable(type.Name.c_str(), selected)) {
                        autosplitAddType = i;
                        autosplitAddSplit = 0;
                    }
                    if (selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndListBox();
            }

            ImGui::SameLine();
            ImGui::BeginGroup();
            if (ImGui::Button("Add Split")) {
                std::string name;
                int round = -1;
                if (!autosplitAddType) {
                    name = std::format("Round {}", autosplitRoundInput);
                    round = autosplitRoundInput;
                }
                else {
                    name = autosplitMenuSplits[currentData.MapNum][autosplitAddType].Splits[autosplitAddSplit];
                }
                if (std::find(currentData.Splits.begin(), currentData.Splits.end(), name) == currentData.Splits.end()) {
                    currentPreset->AddSplit(Split(name, name, round));
                }
            }
            if (autosplitAddType) {
                size_t amount = autosplitMenuSplits[currentData.MapNum][autosplitAddType].Splits.size();
                if (ImGui::BeginListBox("##Add Splits", ImVec2(width * 0.5f - style.ItemSpacing.x, addHeight - ImGui::GetItemRectSize().y - style.ItemSpacing.y))) {
                    ImGui::Selection(autosplitMenuSplits[currentData.MapNum][autosplitAddType].Splits, autosplitAddSplit);
                    ImGui::EndListBox();
                }
            }
            else {
                ImGui::SetNextItemWidth(width * 0.5f - style.ItemSpacing.x);
                if (ImGui::InputInt("##Round Number", &autosplitRoundInput, NULL, NULL)) {
                    if (autosplitRoundInput < 1) {
                        autosplitRoundInput = 1;
                    }
                    if (autosplitRoundInput > 255) {
                        autosplitRoundInput = 255;
                    }
                }
                ImGui::TextWrapped("Split will trigger when the input round ends. Valid rounds are 1-255 inclusive.");
            }
            ImGui::EndGroup();

            ImGui::EndChild();
        }

        if (ImGui::BeginChild("Splits Info", ImVec2(0, 0))) {
            if (ImGui::BeginTable("Splits", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX)) {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::CenterText("Split");
                ImGui::Text("Split");

                ImGui::TableSetColumnIndex(1);
                ImGui::CenterText("Custom Name");
                ImGui::Text("Custom Name");

                ImGui::TableSetColumnIndex(2);
                ImGui::CenterText("Condition");
                ImGui::Text("Condition");

                ImGui::TableSetColumnIndex(3);
                ImGui::CenterText("Remove");
                ImGui::Text("Remove");

                ImGui::TableSetColumnIndex(4);
                ImGui::CenterText("Move Layer");
                ImGui::Text("Move Layer");

                if (currentPreset->ShouldMove()) {
                    currentPreset->MoveSplit();
                }

                for (size_t row = 0; row < currentData.NumSplits; row++) {
                    std::string& Name = currentData.Splits[row].Name;
                    std::string& CustomName = currentData.Splits[row].CustomName;
                    int& Round = currentData.Splits[row].Round;
                    bool roundSplit = Name.find("Round") == Name.npos;

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    Walnut::UI::ShiftCursorY(5.0f);
                    ImGui::CenterText(Name);
                    ImGui::Text(Name.c_str());

                    ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(width * 0.2f);
					if (ImGui::InputText(std::format("##{}", Name).c_str(), &CustomName, ImGuiInputTextFlags_AutoSelectAll)) {
						if (CustomName.empty()) {
							CustomName = Name;
						}
                        currentPreset->SavePreset();
					}

                    if (roundSplit) {
                        ImGui::TableSetColumnIndex(2);
                        size_t& condition = currentPreset->GetCondition(row);
                        ImGui::SetNextItemWidth(width * 0.15f);
                        if (ImGui::BeginCombo(std::format("##Split Condition {}", std::to_string(row)).c_str(), autosplitConditions[condition].c_str())) {
                            if (ImGui::Selection(autosplitConditions, condition)) {
                                switch (condition) {
                                case 0: {
                                    Round = -1;
                                    break;
                                }
                                case 1: {
                                    Round = 0;
                                    break;
                                }
                                default: {
                                    Round = 1;
                                    break;
                                }
                                }
                                currentPreset->SavePreset();
                            }
                            ImGui::EndCombo();
                        }
                        if (Round > 0) {
                            ImGui::SameLine();
                            ImGui::SetNextItemWidth(width * 0.05f);
                            if (ImGui::InputInt(std::format("##Round Condition {}", row).c_str(), &Round, NULL, NULL)) {
                                if (Round < 1) {
                                    Round = 1;
                                }
                                else if (Round > 255) {
                                    Round = 255;
                                }
                            }
                        }
                    }

                    ImGui::TableSetColumnIndex(3);
                    ImGui::BeginRedButtons();
                    ImGui::CenterText(FONT_MINUS);
                    Walnut::UI::ShiftCursorX(-(style.FramePadding.x));
                    if (ImGui::Button(std::format(FONT_MINUS"##{}", Name).c_str())) {
                        currentPreset->DeleteSplit(row);
                    }
                    ImGui::EndRedButtons();

                    ImGui::TableSetColumnIndex(4);
                    ImGui::Button(std::format(FONT_TAB_ARROWS "##AutosplitIndex{}", row).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0));
                    if (ImGui::BeginDragDropSource()) {
                        ImGui::SetDragDropPayload("Move Autosplits", &row, sizeof(row));
                        ImGui::Text("Drag to reorder splits");
                        ImGui::EndDragDropSource();
                    }
                    if (ImGui::BeginDragDropTarget()) {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Move Autosplits")) {
                            size_t source = *(size_t*)payload->Data;
                            currentPreset->MarkSwap(source, row);
                        }
                        ImGui::EndDragDropTarget();
                    }
                }

                ImGui::EndTable();
            }

            ImGui::EndChild();
        }

        ImGui::EndGroup();

        // Popups
        if (ImGui::BeginCenteredOverridePopupModal("Autosplit Map Change Confirmation", ImVec2(0, 0), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextWrapped("Changing the map will invalidate the current selected extra splits. Press continue to delete the current splits or press close and create a new layout instead.");
            ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 35.0f);
            float width = ImGui::GetContentRegionAvail().x / 2.0f - 5;
            if (ImGui::Button("Continue", ImVec2(width, 0))) {
                currentData.MapNum = autosplitMapChangeNum;
                currentData.Splits = { };
                currentData.NumSplits = 0;
                currentPreset->SavePreset();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", ImVec2(width, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndOverridePopup();
        }
    }

    void PracticePatchesBody()
    {
        if (/*currentPreset && */Memory::injected) {
            ImGui::SameLine();
            Walnut::UI::ShiftCursorX(ImGui::GetContentRegionAvail().x - 110.0f);
            if (ImGui::ToggleButton("Practice Patches Active", "Active", &practicePatchesActive)) {

            }
        }
    }

    // In Game Options

    void PlayerOptionsBody()
    {
        if (!validMap) {
            ImGui::CenterText("Waiting for a map to load");
            ImGui::Text("Waiting for a map to load");
            return;
        }

        float width = ImGui::GetContentRegionAvail().x;
        float height = ImGui::GetContentRegionAvail().y;
        ImGuiStyle style = ImGui::GetStyle();

        ImGui::BeginGroup();
        {
            if (ImGui::BeginChild("General Options", ImVec2(510.0f, 150.0f))) {
                float avail = ImGui::GetContentRegionAvail().x;
                ImGui::CenterText("General Options");
                ImGui::TextBold("General Options");
                ImGui::SetNextItemWidth(avail * 0.75f);
                if (ImGui::SliderInt("Timescale", &timescale, 1, 10)) {
                    NotifyGame(std::format("game timescale-{}", timescale));
                }
                ImGui::ItemTooltip("Modifies how fast the game runs (1 is base)");
                ImGui::SetNextItemWidth(avail * 0.75f);
                if (ImGui::SliderInt("Speed", &playerSpeed, 190, 1900)) {
                    NotifyGame(std::format("game g_speed-{}", playerSpeed));
                }
                ImGui::ItemTooltip("Modifies player speed (190 is base)");
                if (ImGui::ToggleButton("Godmode", "Godmode", &godmode)) {
                    NotifyGame(std::format("game godmode-{}", (int)godmode));
                }
                ImGui::ItemTooltip("Toggles godmode");
                ImGui::SameLine();
                if (ImGui::ToggleButton("Noclip", "Noclip", &noclip)) {
                    NotifyGame("cbuf noclip");
                }
                ImGui::ItemTooltip("Toggles noclip");
                ImGui::SameLine();
                if (ImGui::ToggleButton("Infinite Ammo", "Infinite Ammo", &infiniteAmmo)) {
                    NotifyGame(std::format("game infinite_ammo-{}", (int)infiniteAmmo));
                }
                ImGui::ItemTooltip("Toggles infinite ammo");
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("Point Options", ImVec2(510.0f, 100.0f))) {
                float avail = ImGui::GetContentRegionAvail().x;
                ImGui::CenterText("Point Options");
                ImGui::TextBold("Point Options");
                ImGui::SetNextItemWidth(avail * 0.5f);
                if (ImGui::InputInt("Set Points", &pointInput, 1000, 10000, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (pointInput < 0) {
                        pointInput = 0;
                    }
                    else if (pointInput > 4194303) {
                        pointInput = 4194303;
                    }
                    NotifyGame(std::format("game set_points-{}", pointInput));
                }
                ImGui::ItemTooltip("Set the desired amount of points");
                ImGui::SameLine();
                ImGui::HelpMarker("Increment +- 1000\nCtrl + Click increment +- 10000");
                if (ImGui::Button("Max Points", ImVec2(avail / 2 - style.ItemSpacing.x, 0))) {
                    NotifyGame(std::format("game set_points-{}", 4194303));
                }
                ImGui::ItemTooltip("Sets points to max");
                ImGui::SameLine();
                if (ImGui::Button("Reset Points", ImVec2(avail / 2, 0))) {
                    NotifyGame(std::format("game set_points-{}", 500));
                }
                ImGui::ItemTooltip("Resets points to 500");
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("Perk Options", ImVec2(510.0f, 0))) {
                float avail = ImGui::GetContentRegionAvail().x;
                ImGui::CenterText("Perk Options");
                ImGui::TextBold("Perk Options");
                if (ImGui::Button("Give All Perks", ImVec2(avail / 4 - style.ItemSpacing.x, 0))) {
                    NotifyGame("game perks-give-all");
                }
                ImGui::ItemTooltip("Gives every perk for the current map");
                ImGui::SameLine();
                if (ImGui::Button("Take All Perks", ImVec2(avail / 4 - style.ItemSpacing.x, 0))) {
                    NotifyGame("game perks-take-all");
                }
                ImGui::ItemTooltip("Removes every perk you currently have");
                ImGui::SameLine();
                if (ImGui::Button("Give Perk", ImVec2(avail / 4 - style.ItemSpacing.x, 0))) {
                    NotifyGame(std::format("game perks-give-{}", perks[mapName][perkSelectIndex].InternalName));
                }
                ImGui::ItemTooltip("Gives the currently selected perk");
                ImGui::SameLine();
                if (ImGui::Button("Take Perk", ImVec2(avail / 4, 0))) {
                    NotifyGame(std::format("game perks-take-{}", perks[mapName][perkSelectIndex].InternalName));
                }
                ImGui::ItemTooltip("Takes the currently selected perk");
                if (ImGui::BeginChild("Perk List")) {
                    float scroll = ImGui::GetScrollMaxX() > 0 ? style.ScrollbarSize : 0.0f;
                    if (ImGui::BeginListBox("##Perk List", ImVec2(avail - scroll, ImGui::GetContentRegionAvail().y))) {
                        for (size_t i = 0; i < perks[mapName].size(); i++) {
                            const bool selected = perkSelectIndex == i;
                            if (ImGui::Selectable(perks[mapName][i].Name.c_str(), selected)) {
                                perkSelectIndex = i;
                            }
                            if (selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndListBox();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            if (ImGui::BeginChild("Weapon Options", ImVec2(0, height * 0.66f))) {
                ImGui::CenterText("Weapon Options");
                ImGui::TextBold("Weapon Options");
                Walnut::UI::ShiftCursorX(style.ItemSpacing.x);
                ImGui::BeginGroup();

                if (ImGui::Button("Give Weapon")) {
                    size_t typeIndex = mapWeaponsSelection[mapName].GetSelectionIndex();
                    size_t weaponIndex = mapWeaponsSelection[mapName].GetItemIndex();
                    NotifyGame(std::format("game weapon-give-{}-{}", mapWeapons[mapName][typeIndex][weaponIndex].InternalName, (int)upgradedWeapon));
                }
                ImGui::ItemTooltip("Give the selected weapon");
                ImGui::SameLine();
                ImGui::ToggleButton("Weapon Upgraded", "Upgraded", &upgradedWeapon);
                ImGui::ItemTooltip("Choose if the selected weapon should be upgraded");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
                if (ImGui::BeginCombo("##Take Weapon", takeWeaponChocies[takeWeaponIndex].c_str())) {
                    ImGui::Selection(takeWeaponChocies, takeWeaponIndex);
                    ImGui::EndCombo();
                }
                ImGui::ItemTooltip("Choose a weapon slot to remove");
                ImGui::SameLine();
                if (ImGui::Button("Take Weapon")) {
                    NotifyGame(std::format("game weapon-take-{}", takeWeaponChocies[takeWeaponIndex]));
                    LOG_DEBUG(std::format("game weapon-take-{}", takeWeaponChocies[takeWeaponIndex]));
                }
                ImGui::ItemTooltip("Take the selected weapon slot");
                ImGui::EndGroup();

                ImGui::BeginGroup();
                size_t typeIndex = 0;
                ImVec2 avail = ImGui::GetContentRegionAvail();
                for (const std::string& type : weaponTypes) {
                    if (typeIndex >= mapWeapons[mapName].size()) {
                        break;
                    }

                    if (ImGui::BeginChild(std::format("Weapon Menu {}", typeIndex).c_str(), ImVec2(avail.x / 3 - style.ItemSpacing.x, avail.y / 3 - style.ItemSpacing.y))) {
                        ImGui::CenterText(type);
                        ImGui::Text(type.c_str());
                        if (ImGui::BeginListBox(std::format("##Weapon Type {}", type).c_str(), ImGui::GetContentRegionAvail())) {
                            if (mapWeaponsSelection[mapName].Render()) {

                            }
                            ImGui::EndListBox();
                        }
                        ImGui::EndChild();
                    }

                    typeIndex++;
                    if (!(typeIndex % 3)) {
                        ImGui::EndGroup();
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                    }
                }
                ImGui::EndGroup();
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("Gobblegum Options")) {
                ImGui::CenterText("Gobblegum Options");
                ImGui::TextBold("Gobblegum Options");
                ImGui::BeginGroup();
                {
                    if (ImGui::Button("Give Gum", ImVec2(150.0f, 0))) {
                        NotifyGame(std::format("game bgb-give-{}", currentBGB->InternalName));
                    }
                    ImGui::ItemTooltip("Give the selected gobblegum");
                    if (ImGui::Button("Take Gum", ImVec2(150.0f, 0))) {
                        NotifyGame(std::format("game bgb-take"));
                    }
                    ImGui::ItemTooltip("Take your current gobblegum");
                    if (ImGui::Button("Use Gum Charge", ImVec2(150.0f, 0))) {
                        NotifyGame("game bgb-use");
                    }
                    ImGui::ItemTooltip("Use a charge of your current gobblegum");
                    if (ImGui::Button("Activate Gum", ImVec2(150.0f, 0))) {
                        NotifyGame("game bgb-activate");
                    }
                    ImGui::ItemTooltip("Activate your current gobblegum");
                    ImGui::EndGroup();
                }
                ImGui::SameLine();
                if (bgbImages.find(currentBGB->Name) == bgbImages.end()) {
                    LoadSingleBGBImage(currentBGB->Name);
                }
                ImGui::BeginGroup();
                {
                    ImGui::TextBold(currentBGB->Name.c_str());
                    ImGui::SameLine();
                    ImGui::HelpMarker(currentBGB->Description);
                    float size = std::min(196.0f, ImGui::GetContentRegionAvail().y);
                    ImGui::Image(bgbImages[currentBGB->Name].Image->GetDescriptorSet(), ImVec2(size, size));
                    ImGui::EndGroup();
                }
                ImGui::SameLine();
                float avail = ImGui::GetContentRegionAvail().x;
                if (ImGui::BeginChild("Classic Selection", ImVec2(avail / 2 - style.ItemSpacing.x, 0))) {
                    ImVec2 space = ImGui::GetContentRegionAvail();
                    if (ImGui::BeginListBox("##Classic Selection", ImVec2(space.x - style.ItemSpacing.x, space.y - style.ItemSpacing.y))) {
                        if (bgbSelections.Render()) {
                            currentBGB = &*std::find(bgbs.begin(), bgbs.end(), bgbSelections.GetSelectedItem());
                        }
                        ImGui::EndListBox();
                    }
                    ImGui::EndChild();
                }
                ImGui::SameLine();
                if (ImGui::BeginChild("Mega Selection", ImVec2(avail / 2, 0))) {
                    ImVec2 space = ImGui::GetContentRegionAvail();
                    if (ImGui::BeginListBox("##Mega Selection", ImVec2(space.x - style.ItemSpacing.x, space.y - style.ItemSpacing.y))) {
                        if (bgbSelections.Render()) {
                            currentBGB = &*std::find(bgbs.begin(), bgbs.end(), bgbSelections.GetSelectedItem());
                        }
                        ImGui::EndListBox();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }
    }

    void ZombieOptionsBody()
    {
        if (!validMap) {
            ImGui::CenterText("Waiting for a map to load");
            ImGui::Text("Waiting for a map to load");
            return;
        }

        float width = ImGui::GetContentRegionAvail().x;
        float height = ImGui::GetContentRegionAvail().y;
        ImGuiStyle style = ImGui::GetStyle();

        ImGui::BeginGroup();
        {

            if (ImGui::BeginChild("General Options", ImVec2(480.0f, 175.0f))) {
                ImGui::CenterText("General Options");
                ImGui::TextBold("General Options");
                if (ImGui::ToggleButton("Override Zombie Health", "Override Zombie Health" , &zombieHealthOverride)) {
                    NotifyGame(std::format("game zombie_health-{}-{}", (int)zombieHealthOverride, zombieHealthRound));
                }
                ImGui::ItemTooltip("Toggle overriding zombie health");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(75.0f);
                if (ImGui::InputInt("##Zombie Health Round", &zombieHealthRound, NULL, NULL, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (zombieHealthRound < 1) {
                        zombieHealthRound = 1;
                    }
                    else if (zombieHealthRound > 255) {
                        zombieHealthRound = 255;
                    }
                    if (zombieHealthOverride) {
                        NotifyGame(std::format("game zombie_health-{}-{}", (int)zombieHealthOverride, zombieHealthRound));
                    }
                }
                ImGui::ItemTooltip("Choose the round you want the zombies health to be at");
                if (ImGui::ToggleButton("Override Zombie Speed", "Override Zombie Speed", &zombieSpeedOverride)) {
                    NotifyGame(std::format("game zombie_speed-{}-{}", (int)zombieSpeedOverride, zombieSpeedOverrides[zombieSpeedIndex]));
                }
                ImGui::ItemTooltip("Toggle overriding zombie speed");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(125.0f);
                if (ImGui::BeginCombo("##Zombie Speed", zombieSpeedOverrides[zombieSpeedIndex].c_str())) {
                    if (ImGui::Selection(zombieSpeedOverrides, zombieSpeedIndex) && zombieSpeedOverride) {
                        NotifyGame(std::format("game zombie_speed-{}-{}", (int)zombieSpeedOverride, zombieSpeedOverrides[zombieSpeedIndex]));
                    }
                    ImGui::EndCombo();
                }
                ImGui::ItemTooltip("Choose the speed you want the zombies to be");
                if (ImGui::ToggleButton("Freeze Zombies", "Freeze Zombies", &zombieFreeze)) {
                    NotifyGame(std::format("game zombie_freeze-{}", (int)zombieFreeze));
                }
                ImGui::ItemTooltip("Toggle zombie movement");
                ImGui::SameLine();
                if (ImGui::ToggleButton("Toggle Spawning", "Toggle Spawning", &zombieSpawning)) {
                    NotifyGame(std::format("game zombie_spawns-{}", (int)zombieSpawning));
                }
                ImGui::ItemTooltip("Toggle zombie spawns");
                if (ImGui::ToggleButton("Freeze TP", "Freeze TP", &zombieFreezeTP)) {
                    NotifyGame(std::format("game zombie_tp-freeze-{}", (int)zombieFreezeTP));
                }
                ImGui::ItemTooltip("Freeze zombies in place where your TP location is");
                ImGui::SameLine();
                if (ImGui::Button("TP Zombies")) {
                    NotifyGame(std::format("game zombie_tp"));
                }
                ImGui::ItemTooltip("TP all zombies to player");
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("Round Options", ImVec2(480.0f, 150.0f))) {
                ImGui::CenterText("Round Options");
                ImGui::TextBold("Round Options");
                ImGui::SetNextItemWidth(75.0f);
                if (ImGui::InputInt("##Round Input", &roundInput, NULL, NULL)) {
                    if (roundInput < 1) {
                        roundInput = 1;
                    }
                    else if (roundInput > 255) {
                        roundInput = 255;
                    }
                }
                ImGui::ItemTooltip("Enter the round you want to skip to");
                ImGui::SameLine();
                if (ImGui::Button("Skip to Round")) {
                    NotifyGame(std::format("game round-goto-{}", roundInput));
                }
                ImGui::ItemTooltip("Skip to the selected round");
                ImGui::SameLine();
                if (ImGui::Button("End Round")) {
                    NotifyGame(std::format("game round-end"));
                }
                ImGui::ItemTooltip("Ends the current round");
                ImGui::SameLine();
                if (ImGui::Button("Restart Round")) {
                    NotifyGame(std::format("game round-restart"));
                }
                ImGui::ItemTooltip("Restarts the current round");
                ImGui::SetNextItemWidth(75.0f);
                ImGui::InputInt("##Set Zombie Count", &zombieCount, NULL, NULL);
                ImGui::ItemTooltip("Enter the amount of zombies remaining");
                ImGui::SameLine();
                if (ImGui::Button("Set Zombie Total")) {
                    NotifyGame(std::format("game zombie_total-{}", zombieCount));
                }
                ImGui::ItemTooltip("Set the amount of zombies left to spawn for the round");
                if (ImGui::ToggleButton("Infinite Round", "Infinite Round", &infiniteRound)) {
                    NotifyGame(std::format("game round-infinite-{}", (int)infiniteRound));
                }
                ImGui::ItemTooltip("Makes the round never end");
                ImGui::SameLine();
                if (ImGui::Button("Start Special Round")) {
                    NotifyGame("game round-special");
                }
                ImGui::ItemTooltip("Start a special round on the current round");
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            if (ImGui::BeginChild("Powerup Options", ImVec2(300.0f, 335.0f))) {
                ImGui::CenterText("Powerup Options");
                ImGui::TextBold("Powerup Options");
                if (ImGui::Button("Spawn Powerup")) {
                    NotifyGame(std::format("game powerup-{}-{}-{}", powerups[mapName][powerupIndex].InternalName, (int)instaGrab, powerups[mapName][powerupIndex].Name));
                }
                ImGui::ItemTooltip("Spawn the selected powerup");
                ImGui::SameLine();
                ImGui::ToggleButton("Insta Grab", "Insta Grab", &instaGrab);
                ImGui::ItemTooltip("Choose whether to instantly grab the powerup or not");
                if (ImGui::BeginListBox(std::format("##Powerups {}", mapName).c_str(), ImGui::GetContentRegionAvail())) {
                    for (size_t i = 0; i < powerups[mapName].size(); i++) {
                        const bool selected = powerupIndex == i;
                        if (ImGui::Selectable(powerups[mapName][i].Name.c_str(), selected)) {
                            powerupIndex = i;
                        }
                        if (selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndListBox();
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            if (ImGui::BeginChild("Craftable Options", ImVec2(ImGui::GetContentRegionAvail().x, 335.0f))) {
                ImGui::CenterText("Craftable Options");
                ImGui::TextBold("Craftable Options");
                if (!validCraftMap) {
                    ImGui::CenterText("Waiting for a valid craftable map");
                    ImGui::Text("Waiting for a valid craftable map");
                }
                else {
                    ImGui::BeginGroup();
                    {
                        if (ImGui::BeginListBox(std::format("##Craftable Tree Items {}", mapName).c_str(), ImVec2(200.0f, ImGui::GetContentRegionAvail().y))) {
                            std::vector<TreeItem> treeItems = craftables[mapName];
                            size_t prevItem = treeIndex;
                            for (size_t i = 0; i < treeItems.size(); i++) {
                                const bool selected = treeIndex == i;
                                if (ImGui::Selectable(treeItems[i].Name.c_str(), selected)) {
                                    treeIndex = i;
                                }
                                if (selected) {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            if (prevItem != treeIndex) {
                                pieceIndex = 0;
                            }
                            ImGui::EndListBox();
                        }
                        ImGui::EndGroup();
                    }
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    {
                        if (ImGui::Button("Collect Piece")) {
                            const TreeItem& tree = craftables[mapName][treeIndex];
                            const PieceItem& piece = tree.Pieces[pieceIndex];
                            NotifyGame(std::format("game craftable-{}-{}", tree.InternalName, piece.InternalName));
                        }
                        ImGui::ItemTooltip("Collect the selected crafting piece");
                        ImGui::SameLine();
                        if (ImGui::Button("Collect All Pieces")) {
                            const TreeItem& tree = craftables[mapName][treeIndex];
                            NotifyGame(std::format("game craftable-{}-all", tree.InternalName));
                        }
                        ImGui::ItemTooltip("Collect all crafting pieces for the current craftable");
                        if (ImGui::BeginListBox(std::format("##Craftable Piece Items {}", mapName).c_str(), ImGui::GetContentRegionAvail())) {
                            const std::vector<PieceItem>& pieceItems = craftables[mapName][treeIndex].Pieces;
                            for (size_t i = 0; i < pieceItems.size(); i++) {
                                const bool selected = pieceIndex == i;
                                if (ImGui::Selectable(pieceItems[i].Name.c_str(), selected)) {
                                    pieceIndex = i;
                                }
                                if (selected) {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            ImGui::EndListBox();
                        }
                        ImGui::EndGroup();
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }
        ImGui::BeginGroup();
        {
            if (ImGui::BeginChild("Blocker Options", ImVec2(480.0f, ImGui::GetContentRegionAvail().y))) {
                ImGui::CenterText("Blocker Options");
                ImGui::TextBold("Blocker Options");
                ImGui::SetNextItemWidth(150.0f);
                if (ImGui::BeginCombo(std::format("##Blockers Type {}", mapName).c_str(), blockerTypes[blockerTypeIndex].c_str())) {
                    if (ImGui::Selection(blockerTypes, blockerTypeIndex)) {
                        blockerIndex = 0;
                    }
                    ImGui::EndCombo();
                }
                ImGui::ItemTooltip("Choose the type of blockers to select from");
                ImGui::SameLine();
                if (ImGui::Button("Activate Blocker")) {
                    NotifyGame(std::format("game blocker-{}-{}", blockerTypes[blockerTypeIndex], blockers[mapName][blockerTypeIndex].Blockers[blockerIndex].Notify));
                }
                ImGui::ItemTooltip("Activate the selected blocker");
                ImGui::SameLine();
                if (ImGui::Button("Activate All Blockers")) {
                    NotifyGame(std::format("game blocker-{}-all", blockerTypes[blockerTypeIndex]));
                }
                ImGui::ItemTooltip("Activate all blockers in the selected type");
                const std::vector<MapBlockers>& mapBlockers = blockers[mapName];
                if (ImGui::BeginListBox(std::format("##Blockers List {}", mapName).c_str(), ImGui::GetContentRegionAvail())) {
                    const std::vector<Blocker>& blockerItems = mapBlockers[blockerTypeIndex].Blockers;
                    for (size_t i = 0; i < blockerItems.size(); i++) {
                        const bool selected = blockerIndex == i;
                        if (ImGui::Selectable(blockerItems[i].Name.c_str(), selected)) {
                            blockerIndex = i;
                        }
                        if (selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndListBox();
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            if (ImGui::BeginChild("Egg Step Options", ImGui::GetContentRegionAvail())) {
                ImGui::CenterText("Egg Step Options");
                ImGui::TextBold("Egg Step Options");
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }
    }

    // Resources
    void GumTrackerBody()
    {

    }

    void ZombieCalculatorBody()
    {
        ImGuiStyle style = ImGui::GetStyle();
        ImVec2 avail = ImGui::GetContentRegionAvail();

        if (ImGui::BeginChild("Zombie Calc", ImVec2((avail.x / 5) - style.ItemSpacing.x, (avail.y / 3) - style.ItemSpacing.y))) {
            ImGui::CenterText("Current Round");
            ImGui::Text("Current Round");
            ImGui::SetNextItemWidth(150.0f);
            if (ImGui::InputInt("##Current Round", &calcRound, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (calcRound < 1) {
                    calcRound = 1;
                }
                else if (calcRound > 255) {
                    calcRound = 255;
                }
            }
            ImGui::EndChild();
        }

        for (size_t i = 0; i < 4; i++) {
            ImGui::SameLine();
            ImVec2 size = ImVec2(i != 3 ? ((avail.x / 5) - style.ItemSpacing.x) : (avail.x / 5), (avail.y / 3) - style.ItemSpacing.y);
            if (ImGui::BeginChild(std::format("Zombie Calc Basic Data {}", i).c_str(), size)) {

                ImGui::EndChild();
            }
        }
    }

    void CodeGuidesBody()
    {

    }

    void GKValveSolverBody()
    {
        ImGuiStyle style = ImGui::GetStyle();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImGui::SameLine();
        if (ImGui::Button("Reset Valves")) {
            greenValve = nullptr;
            passwordValve = nullptr;
            for (int i = 0; i < 6; i++) {
                notGreenValves[i] = false;
                valves[i]->Direction1 = false;
                valves[i]->Direction2 = false;
                valves[i]->Direction3 = false;
                valves[i]->Green = false;
                valves[i]->CheckedGreen = false;
                valves[i]->Password = false;
            }
            CalcValveProbabilities();
            CalcValvePossibleSolutions();
        }
        ImGui::SameLine();
        ImGui::CenterTextHorizontalToWindow("Valve Solver");
        ImGui::Text("Valve Solver");

        float size = ImGui::GetContentRegionAvail().x / 3;
        size_t newLine = 0;
        for (int i = 0; i < 6; i++) {
            Valve*& valve = valves[i];
            if (ImGui::BeginChild(valve->Name.c_str(), ImVec2(size - (newLine == 2 ? 0 : style.ItemSpacing.x), (size * 0.7f / 3.19f) + 140.0f))) {
                ImGui::CenterText(valve->Name);
                ImGui::Text(valve->Name.c_str());
                ImGui::BeginGroup();
                {
                    bool disabled = false;
                    // Checked green toggle
                    if (greenValve || valve->Green || valve->Password) {
                        ImGui::BeginDisabled();
                    }
                    if (ImGui::Checkbox(std::format("Not Green##{}", valve->Name).c_str(), &valve->CheckedGreen)) {
                        notGreenValves[i] = !notGreenValves[i];
                        CalcValveProbabilities();
                        CalcValvePossibleSolutions();
                    }
                    if (greenValve || valve->Green || valve->Password) {
                        ImGui::EndDisabled();
                    }
                    ImGui::SameLine();
                    float width = ImGui::GetContentRegionAvail().x / 2 - style.ItemSpacing.x;
                    // Green toggle
                    if (greenValve && !valve->Green || valve->CheckedGreen || valve->Password) {
                        ImGui::BeginDisabled();
                        disabled = true;
                    }
                    if (ImGui::ActiveButton(std::format("Green##{}", valve->Name), &valve->Green, ImVec2(width, 0))) {
                        if (!greenValve) {
                            greenValve = valve;
                        }
                        else {
                            greenValve = nullptr;
                        }
                        CalcValveProbabilities();
                        CalcValvePossibleSolutions();
                    }
                    if (disabled) {
                        ImGui::EndDisabled();
                    }
                    ImGui::SameLine();
                    // Password toggle
                    disabled = false;
                    if (passwordValve && !valve->Password || valve->Green) {
                        ImGui::BeginDisabled();
                        disabled = true;
                    }
                    if (ImGui::ActiveButton(std::format("Pink##{}", valve->Name), &valve->Password, ImVec2(width, 0))) {
                        if (!passwordValve) {
                            passwordValve = valve;
                        }
                        else {
                            passwordValve = nullptr;
                        }
                        CalcValveProbabilities();
                        CalcValvePossibleSolutions();
                    }
                    if (disabled) {
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();
                }

                float width = size * 0.7f;
                Walnut::UI::ShiftCursorX(size * 0.15f);
                ImGui::BeginGroup();
                {
                    if (valve->Password) {
                        ImGui::BeginDisabled();
                    }

                    ImGui::Image(g_ValveImage->GetDescriptorSet(), ImVec2(width, width / 3.19f));

                    ImGui::BeginGroup();
                    {
                        if (ImGui::ActiveButton(std::format("1##{}", valve->Name), &valve->Direction1, ImVec2((width - style.ItemSpacing.x) / 3, 0))) {
                            valve->Direction2 = false;
                            valve->Direction3 = false;
                        }
                        std::string text = std::format("{}%-{}%", valve->Probability1[0], valve->Probability2[0]);
                        Walnut::UI::ShiftCursorX((ImGui::GetItemRectSize().x - ImGui::CalcTextSize(text.c_str()).x + ImGui::CalcTextSize("%%").x) / 2);
                        ImGui::Text(text.c_str());
                        ImGui::EndGroup();
                    }
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    {
                        if (ImGui::ActiveButton(std::format("2##{}", valve->Name), &valve->Direction2, ImVec2((width - style.ItemSpacing.x) / 3, 0))) {
                            valve->Direction1 = false;
                            valve->Direction3 = false;
                        }
                        std::string text = std::format("{}%-{}%", valve->Probability1[1], valve->Probability2[1]);
                        Walnut::UI::ShiftCursorX((ImGui::GetItemRectSize().x - ImGui::CalcTextSize(text.c_str()).x + ImGui::CalcTextSize("%%").x) / 2);
                        ImGui::Text(text.c_str());
                        ImGui::EndGroup();
                    }
                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    {
                        if (ImGui::ActiveButton(std::format("3##{}", valve->Name), &valve->Direction3, ImVec2(width / 3, 0))) {
                            valve->Direction1 = false;
                            valve->Direction2 = false;
                        }
                        std::string text = std::format("{}%-{}%", valve->Probability1[2], valve->Probability2[2]);
                        Walnut::UI::ShiftCursorX((ImGui::GetItemRectSize().x - ImGui::CalcTextSize(text.c_str()).x + ImGui::CalcTextSize("%%").x) / 2);
                        ImGui::Text(text.c_str());
                        ImGui::EndGroup();
                    }
                    if (valve->Password) {
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();
                }

                newLine++;
                ImGui::EndChild();
            }
            if (newLine != 3) {
                ImGui::SameLine();
            }
            else {
                newLine = 0;
            }
        }

        if (possibleSolutions1.size()) {
            if (ImGui::BeginChild("Valve Solutions 1", ImVec2((ImGui::GetContentRegionAvail().x - style.ItemSpacing.x) / 2, 0))) {
                const float scroll = ImGui::GetScrollY();
                drawList->PushClipRect(ImGui::GetCursorScreenPos() + ImVec2(0, scroll), ImGui::GetCursorScreenPos() + ImGui::GetContentRegionAvail() + ImVec2(0, scroll), true);
                ImGui::CenterText("Solutions 1");
                ImGui::Text("Solutions 1");

                if (ImGui::BeginTable("Valve Solutions 1", 6, ImGuiTableFlags_Borders)) {
                    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
                    for (size_t column = 0; column < 6; column++) {
                        ImGui::TableSetColumnIndex(column);
                        ImGui::CenterText(valveLocations[column]);
                        ImGui::TableHeader(valveLocations[column].c_str());
                    }

                    for (size_t row = 0; row < possibleSolutions1.size(); row++) {
                        ImGui::TableNextRow();

                        for (size_t column = 0; column < 6; column++) {
                            if (ImGui::TableNextColumn()) {
                                const ImVec2 screenPos = ImGui::GetCursorScreenPos();
                                float width = ImGui::GetContentRegionAvail().x;
                                if (valveLocations[column] == passwordValve->Name) {
                                    drawList->AddRectFilled(screenPos, screenPos + ImVec2(width, 20), IM_COL32(255, 30, 200, 200));
                                }
                                else {
                                    if (possibleSolutions1[row].first == valveLocations[column]) {
                                        drawList->AddRectFilled(screenPos, screenPos + ImVec2(20, 20), IM_COL32(0, 150, 0, 200));
                                        drawList->AddRectFilled(screenPos + ImVec2(20, 0), screenPos + ImVec2(25, 20), IM_COL32(0, 0, 0, 255));
                                    }
                                    else {
                                        drawList->AddRectFilled(screenPos, screenPos + ImVec2(25, 20), IM_COL32(0, 0, 0, 255));
                                    }
                                    bool directions[3] = { valves[column]->Direction1, valves[column]->Direction2, valves[column]->Direction3 };
                                    ImU32 color = directions[atoi(possibleSolutions1[row].second[column].c_str()) - 1] ? IM_COL32(0, 150, 0, 200) : IM_COL32(200, 0, 0, 200);
                                    drawList->AddRectFilled(screenPos + ImVec2(25, 0), screenPos + ImVec2(25, 0) + ImVec2(width - 50, 20), color);
                                    drawList->AddRectFilled(screenPos + ImVec2(25, 0) + ImVec2(width - 50, 0), screenPos + ImVec2(25, 0) + ImVec2(width - 25, 20), IM_COL32(0, 0, 0, 255));
                                }

                                ImGui::CenterText(possibleSolutions1[row].second[column]);
                                ImGui::Text(possibleSolutions1[row].second[column].c_str());
                            }
                        }
                    }
                    drawList->PopClipRect();
                    ImGui::EndTable();
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            if (ImGui::BeginChild("Valve Solutions 2", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                ImGui::CenterText("Solutions 2");
                ImGui::Text("Solutions 2");

                if (ImGui::BeginTable("Valve Solutions 2", 6, ImGuiTableFlags_Borders)) {
                    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
                    for (size_t column = 0; column < 6; column++) {
                        ImGui::TableSetColumnIndex(column);
                        ImGui::CenterText(valveLocations[column]);
                        ImGui::TableHeader(valveLocations[column].c_str());
                    }

                    for (size_t row = 0; row < possibleSolutions2.size(); row++) {
                        ImGui::TableNextRow();

                        for (size_t column = 0; column < 6; column++) {
                            if (ImGui::TableNextColumn()) {
                                const ImVec2 screenPos = ImGui::GetCursorScreenPos();
                                float width = ImGui::GetContentRegionAvail().x;
                                if (valveLocations[column] == passwordValve->Name) {
                                    drawList->AddRectFilled(screenPos, screenPos + ImVec2(width, 20), IM_COL32(255, 30, 200, 200));
                                }
                                else {
                                    if (possibleSolutions2[row].first == valveLocations[column]) {
                                        drawList->AddRectFilled(screenPos, screenPos + ImVec2(20, 20), IM_COL32(0, 150, 0, 200));
                                        drawList->AddRectFilled(screenPos + ImVec2(20, 0), screenPos + ImVec2(25, 20), IM_COL32(0, 0, 0, 255));
                                    }
                                    else {
                                        drawList->AddRectFilled(screenPos, screenPos + ImVec2(25, 20), IM_COL32(0, 0, 0, 255));
                                    }
                                    bool directions[3] = { valves[column]->Direction1, valves[column]->Direction2, valves[column]->Direction3 };
                                    ImU32 color = directions[atoi(possibleSolutions2[row].second[column].c_str()) - 1] ? IM_COL32(0, 150, 0, 200) : IM_COL32(200, 0, 0, 200);
                                    drawList->AddRectFilled(screenPos + ImVec2(25, 0), screenPos + ImVec2(25, 0) + ImVec2(width - 50, 20), color);
                                    drawList->AddRectFilled(screenPos + ImVec2(25, 0) + ImVec2(width - 50, 0), screenPos + ImVec2(25, 0) + ImVec2(width - 25, 20), IM_COL32(0, 0, 0, 255));
                                }

                                ImGui::CenterText(possibleSolutions2[row].second[column]);
                                ImGui::Text(possibleSolutions2[row].second[column].c_str());
                            }
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::EndChild();
            }
        }
    }

    // Reset variables
    void ResetVariables()
    {
        validMap = Memory::validMap;
        validCraftMap = Memory::validCraftMap;
        mapName = Memory::mapName;

        if (Memory::mapChanged) {
            powerupIndex = 0;
            treeIndex = 0;
            pieceIndex = 0;
            blockerTypeIndex = 0;
            blockerIndex = 0;
            blockerTypes.clear();
            for (const MapBlockers& blocker : blockers[mapName]) {
                blockerTypes.emplace_back(blocker.Type);
            }
        }

        godmode = false;
        noclip = false;
        infiniteAmmo = false;
        zombieHealthOverride = false;
        zombieSpeedOverride = false;
        zombieFreeze = false;
        zombieSpawning = true;
        zombieFreezeTP = false;
        infiniteRound = false;
    }

    void OnGameShutdown()
    {
        bgbPresetsActive = false;
        weaponPresetsActive = false;
        autosplitPresetsActive = false;

        BGBPreset::WritePresetToGame(true);
        AutosplitPreset::WritePresetToGame(true);
        WeaponPreset::WritePresetToGame(true);

        ResetVariables();
    }
}