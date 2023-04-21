#include "PowerupOptions.h"

namespace PowerupOptions
{
    void InitPowerupList()
    {
        std::vector<std::string> zodPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, POINTS_TEAM, WW_GRENADE, SHIELDCHARGE };
        std::vector<std::string> factoryPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER };
        std::vector<std::string> castlePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, POINTS_TEAM, WW_GRENADE, SHIELDCHARGE, TRAM, RUNE_LOR, RUNE_ULLA, RUNE_UJA, RUNE_OTH, RUNE_ZOR, RUNE_MAR };
        std::vector<std::string> islandPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, POINTS_TEAM, WW_GRENADE, SHIELDCHARGE, EMPTY_PERK, SEED };
        std::vector<std::string> stalingradPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE, SHIELDCHARGE, CYLINDER_RED, CYLINDER_YELLOW, CYLINDER_BLUE };
        std::vector<std::string> genesisPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE, SHIELDCHARGE, GENESIS_WEAPON };
        std::vector<std::string> prototypePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> asylumPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> sumpfPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> theaterPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> cosmodromePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> templePowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> moonPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE };
        std::vector<std::string> tombPowerups = { MAX, DOUBLE_POINTS, INSTA, NUKE, MINIGUN, CARPENTER, FIRE_SALE, FREE_PERK, POINTS_PLAYER, WW_GRENADE, SHIELDCHARGE, ZOMBIE_BLOOD };
        powerupList.insert({ "zm_zod",  zodPowerups });
        powerupList.insert({ "zm_factory",  factoryPowerups });
        powerupList.insert({ "zm_castle",  castlePowerups });
        powerupList.insert({ "zm_island",  islandPowerups });
        powerupList.insert({ "zm_stalingrad",  stalingradPowerups });
        powerupList.insert({ "zm_genesis",  genesisPowerups });
        powerupList.insert({ "zm_prototype",  prototypePowerups });
        powerupList.insert({ "zm_asylum",  asylumPowerups });
        powerupList.insert({ "zm_sumpf",  sumpfPowerups });
        powerupList.insert({ "zm_theater",  theaterPowerups });
        powerupList.insert({ "zm_cosmodrome",  cosmodromePowerups });
        powerupList.insert({ "zm_temple",  templePowerups });
        powerupList.insert({ "zm_moon",  moonPowerups });
        powerupList.insert({ "zm_tomb",  tombPowerups });
    }
}
