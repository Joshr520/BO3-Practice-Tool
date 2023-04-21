#pragma once

#include <unordered_map>
#include <vector>
#include <sstream>

namespace PowerupOptions
{
    void InitPowerupList();

    inline std::unordered_map<std::string, std::vector<std::string>> powerupList = { };
    inline int powerupListIndex = 0;
    inline bool instaGrab = false;
}

#define POINTS_PLAYER "Bonus Points Player"
#define POINTS_TEAM "Bonus Points Team"
#define CARPENTER "Carpenter"
#define DOUBLE_POINTS "Double Points"
#define FIRE_SALE "Fire Sale"
#define EMPTY_PERK "Perk Slot"
#define FREE_PERK "Free Perk"
#define MAX "Max Ammo"
#define INSTA "Insta Kill"
#define MINIGUN "Death Machine"
#define NUKE "Nuke"
#define SHIELDCHARGE "Shield Charge"
#define WW_GRENADE "Widows Grenade"
#define TRAM "Tram Token"
#define RUNE_LOR "Void Symbol Tears"
#define RUNE_MAR "Void Symbol Ribbon"
#define RUNE_OTH "Void Symbol Claw"
#define RUNE_UJA "Void Symbol Triangles"
#define RUNE_ULLA "Void Symbol Rib"
#define RUNE_ZOR "Void Symbol Tear"
#define SEED "Seed"
#define CYLINDER_RED "Tank Cylinder"
#define CYLINDER_YELLOW "Supply Cylinder"
#define CYLINDER_BLUE "DC Cylinder"
#define GENESIS_WEAPON "Random Weapon"
#define ZOMBIE_BLOOD "Zombie Blood"
