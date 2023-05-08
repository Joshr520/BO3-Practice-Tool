#include "PlayerOptions.h"
#include "GUIWindow.h"

#include <fstream>
#include <sstream>

namespace Weapons
{
#pragma region Weapons
#define SVG "SVG-100"
#define DRAKON "Drakon"
#define LOCUS "Locus"
#define BOOTLEGGER "Bootlegger"
#define VMP "VMP"
#define KUDA "Kuda"
#define VESPER "Vesper"
#define WEEVIL "Weevil"
#define PHARO "Pharo"
#define HG40 "HG 40"
#define RAZORBACK "Razorback"
#define PPSH "PPSh-41"
#define THOMPSON "M1927"
#define AK74 "AK-74u"
#define MP40 "MP40"
#define XM "XM-53"
#define L4 "L4 Siege"
#define BRECCI "205 Brecci"
#define KRM "KRM-262"
#define ARGUS "Argus"
#define HAYMAKER "Haymaker 12"
#define BANSHII "Banshii"
#define LCAR "L-CAR 9"
#define RK5 "RK5"
#define BLOODHOUND "Bloodhound"
#define MR6 "MR6"
#define MARSHAL "Marshal 16 Dual-Wield"
#define RIFT "Rift E9"
#define M1911 "1911"
#define MAUSER "Mauser C96"
#define GORGON "Gorgon"
#define BRM "BRM"
#define DREDGE "48 Dredge"
#define DINGO "Dingo"
#define RPK "RPK"
#define MG08 "MG-08/15"
#define KN "KN-44"
#define SHIEVA "Shieva"
#define M8 "M8A7"
#define MANOWAR "Man-O-War"
#define HVK "HVK-30"
#define ICR "ICR-1"
#define GARAND "MX Garand"
#define FFAR "FFAR"
#define PEACEKEEPER "Peacekeeper MK2"
#define STG "StG-44"
#define M14 "M14"
#define M16 "M16"
#define GALIL "Galil"
#define SHADOWCLAW "NX ShadowClaw Dual Wield"
#define APOTHSERVANT "Apothicon Servant"
#define WAFFE "Wunderwaffe DG-2"
#define RAYGUN "Ray Gun"
#define RAYGUNMK2 "Ray Gun Mark II"
#define RAYGUNMK3 "GKZ-45 Mk3"
#define TGUN "Thundergun"
#define BABYGUN "31-79 JGb215"
#define BOW "Default Bow"
#define STORMBOW "Lightning Bow"
#define FIREBOW "Fire Bow"
#define VOIDBOW "Void Bow"
#define WOLFBOW "Wolf Bow"
#define KT4 "KT-4"
#define WAVEGUN "Zap Gun Dual Wield"
#define ICESTAFF "Staff of Ice"
#define LIGHTNINGSTAFF "Staff of Lightning"
#define FIRESTAFF "Staff of Fire"
#define WINDSTAFF "Staff of Wind"
#define BOWIEKNIFE "Bowie Knife"
#define PLUNGER "Plunger"
#define WRENCH "Wrench"
#define SWORD "Fury's Song"
#define AXE "Slash N' Burn"
#define DAGGER "Malice"
#define KATANA "Path of Sorrows"
#define MACE "Skull Splitter"
#define NUNCHUCKS "Nunchucks"
#define IMPROVISE "Buzz Cut"
#define BONEGLASS "Nightbreaker"
#define SICKLE "Sickle"
#define GRENADES "Frag Grenades"
#define ARNIE "Li'l Arnie"
#define MONKEY "Cymbal Monkey"
#define DOLLS "Matryoshka Doll"
#define GERSH "Gersh Device"
#define QED "Quantum Entanglement Device"
#define GSTRIKE "G Strikes"
#define TRIPMINE "Trip Mine"
#define DSTRIKE "Dragon Strike"
#define SHIELD "Shield"
#define GASMASK "P.E.S."
#define HACKER "Hacker"
#define GLAIVE "Keeper Sword"
#define RAGS "Ragnarok DG-4"
#define SKULL "Skull of Nan Sapwe"
#define GAUNTLET "Gauntlet of Siegfried"
#define ANNIHILATOR "Annihilator"
#pragma endregion Weapons

    void InitWeaponsList()
{
    std::vector<std::string> zodSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> zodSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, BOOTLEGGER }; std::vector<std::string> zodLauncher = { XM }; std::vector<std::string> zodShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> zodPistol = { BLOODHOUND, RK5, LCAR }; std::vector<std::string> zodLMG = { DINGO, DREDGE, BRM, GORGON }; std::vector<std::string> zodAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN }; std::vector<std::string> zodSpecial = { APOTHSERVANT, RAYGUN }; std::vector<std::string> zodMelee = { BOWIEKNIFE }; std::vector<std::string> zodEquipment = { ARNIE, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> zodHero = { GLAIVE };
    std::vector<std::string> factorySniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> factorySMG = { PHARO, WEEVIL, VESPER, KUDA, VMP }; std::vector<std::string> factoryLauncher = { XM }; std::vector<std::string> factoryShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> factoryPistol = { MR6, RK5, LCAR }; std::vector<std::string> factoryLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> factoryAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN }; std::vector<std::string> factorySpecial = { WAFFE, RAYGUN }; std::vector<std::string> factoryMelee = { BOWIEKNIFE }; std::vector<std::string> factoryEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> factoryHero = { ANNIHILATOR };
    std::vector<std::string> castleSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> castleSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP }; std::vector<std::string> castleLauncher = { XM }; std::vector<std::string> castleShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> castlePistol = { MR6, RK5, LCAR }; std::vector<std::string> castleLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> castleAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN }; std::vector<std::string> castleSpecial = { BOW, STORMBOW, FIREBOW, VOIDBOW, WOLFBOW, RAYGUN }; std::vector<std::string> castleMelee = { BOWIEKNIFE, PLUNGER }; std::vector<std::string> castleEquipment = { MONKEY, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> castleHero = { RAGS };
    std::vector<std::string> islandSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> islandSMG = { PHARO, WEEVIL, VESPER, RAZORBACK, HG40, KUDA, VMP }; std::vector<std::string> islandLauncher = { XM }; std::vector<std::string> islandShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> islandPistol = { MR6, RK5, LCAR, MARSHAL }; std::vector<std::string> islandLMG = { DINGO, DREDGE, BRM, GORGON }; std::vector<std::string> islandAR = { ICR, HVK, MANOWAR, GARAND, M8, SHIEVA, KN }; std::vector<std::string> islandSpecial = { KT4, RAYGUN }; std::vector<std::string> islandMelee = { BOWIEKNIFE }; std::vector<std::string> islandEquipment = { MONKEY, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> islandHero = { SKULL };
    std::vector<std::string> stalingradSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> stalingradSMG = { PHARO, WEEVIL, VESPER, HG40, PPSH, KUDA, VMP }; std::vector<std::string> stalingradLauncher = { L4, XM }; std::vector<std::string> stalingradShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> stalingradPistol = { MR6, RK5, LCAR }; std::vector<std::string> stalingradLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> stalingradAR = { ICR, HVK, MANOWAR, FFAR, GARAND, M8, SHIEVA, KN }; std::vector<std::string> stalingradSpecial = { RAYGUNMK3, RAYGUN, SHADOWCLAW }; std::vector<std::string> stalingradMelee = { BOWIEKNIFE, DAGGER, AXE, SWORD, WRENCH }; std::vector<std::string> stalingradEquipment = { DSTRIKE, MONKEY, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> stalingradHero = { GAUNTLET };
    std::vector<std::string> genesisSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> genesisSMG = { PHARO, WEEVIL, VESPER, KUDA, THOMPSON, VMP }; std::vector<std::string> genesisLauncher = { XM }; std::vector<std::string> genesisShotgun = { BANSHII, HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> genesisPistol = { MR6, RIFT, RK5, LCAR }; std::vector<std::string> genesisLMG = { DINGO, DREDGE, BRM, GORGON }; std::vector<std::string> genesisAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, PEACEKEEPER }; std::vector<std::string> genesisSpecial = { TGUN, APOTHSERVANT, RAYGUN }; std::vector<std::string> genesisMelee = { BOWIEKNIFE, BONEGLASS, IMPROVISE, NUNCHUCKS, MACE, KATANA }; std::vector<std::string> genesisEquipment = { ARNIE, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> genesisHero = { RAGS };
    std::vector<std::string> prototypeSniper = { LOCUS, SVG }; std::vector<std::string> prototypeSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, MP40, BOOTLEGGER, AK74 }; std::vector<std::string> prototypeLauncher = { XM }; std::vector<std::string> prototypeShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> prototypePistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> prototypeLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> prototypeAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, GARAND, STG }; std::vector<std::string> prototypeSpecial = { RAYGUNMK2, RAYGUN }; std::vector<std::string> prototypeMelee = { BOWIEKNIFE }; std::vector<std::string> prototypeEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> prototypeHero = { ANNIHILATOR };
    std::vector<std::string> asylumSniper = { LOCUS, SVG }; std::vector<std::string> asylumSMG = { PHARO, WEEVIL, VESPER, PPSH, KUDA, BOOTLEGGER, VMP, MP40, AK74 }; std::vector<std::string> asylumLauncher = { XM }; std::vector<std::string> asylumShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> asylumPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> asylumLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> asylumAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, GARAND, STG }; std::vector<std::string> asylumSpecial = { WAFFE, RAYGUNMK2, RAYGUN }; std::vector<std::string> asylumMelee = { BOWIEKNIFE }; std::vector<std::string> asylumEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> asylumHero = { ANNIHILATOR };
    std::vector<std::string> sumpfSniper = { LOCUS, SVG }; std::vector<std::string> sumpfSMG = { PHARO, WEEVIL, VESPER, PPSH, KUDA, BOOTLEGGER, VMP, MP40, AK74 }; std::vector<std::string> sumpfLauncher = { XM }; std::vector<std::string> sumpfShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> sumpfPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> sumpfLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> sumpfAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, GARAND, STG }; std::vector<std::string> sumpfSpecial = { WAFFE, RAYGUNMK2, RAYGUN }; std::vector<std::string> sumpfMelee = { BOWIEKNIFE }; std::vector<std::string> sumpfEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> sumpfHero = { ANNIHILATOR };
    std::vector<std::string> theaterSniper = { DRAKON, SVG }; std::vector<std::string> theaterSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, MP40, AK74 }; std::vector<std::string> theaterLauncher = { XM }; std::vector<std::string> theaterShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> theaterPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> theaterLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> theaterAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> theaterSpecial = { TGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> theaterMelee = { BOWIEKNIFE }; std::vector<std::string> theaterEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> theaterHero = { ANNIHILATOR };
    std::vector<std::string> cosmodromeSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> cosmodromeSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, AK74 }; std::vector<std::string> cosmodromeLauncher = { L4, XM }; std::vector<std::string> cosmodromeShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> cosmodromePistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> cosmodromeLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> cosmodromeAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> cosmodromeSpecial = { TGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> cosmodromeMelee = { SICKLE }; std::vector<std::string> cosmodromeEquipment = { GERSH, DOLLS, TRIPMINE, GRENADES }; std::vector<std::string> cosmodromeHero = { ANNIHILATOR };
    std::vector<std::string> templeSniper = { LOCUS, DRAKON, SVG }; std::vector<std::string> templeSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, AK74 }; std::vector<std::string> templeLauncher = { L4, XM }; std::vector<std::string> templeShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> templePistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> templeLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> templeAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> templeSpecial = { BABYGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> templeMelee = { BOWIEKNIFE }; std::vector<std::string> templeEquipment = { MONKEY, TRIPMINE, GRENADES }; std::vector<std::string> templeHero = { ANNIHILATOR };
    std::vector<std::string> moonSniper = { LOCUS, DRAKON }; std::vector<std::string> moonSMG = { PHARO, WEEVIL, VESPER, KUDA, VMP, AK74 }; std::vector<std::string> moonLauncher = { L4, XM }; std::vector<std::string> moonShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> moonPistol = { MR6, M1911, RK5, LCAR }; std::vector<std::string> moonLMG = { DINGO, DREDGE, BRM, GORGON, RPK }; std::vector<std::string> moonAR = { ICR, HVK, MANOWAR, FFAR, GALIL, M8, M16, SHIEVA, KN, M14 }; std::vector<std::string> moonSpecial = { WAVEGUN, RAYGUNMK2, RAYGUN }; std::vector<std::string> moonMelee = { BOWIEKNIFE }; std::vector<std::string> moonEquipment = { GASMASK, HACKER, GERSH, QED, TRIPMINE, GRENADES }; std::vector<std::string> moonHero = { ANNIHILATOR };
    std::vector<std::string> tombSniper = { LOCUS, DRAKON }; std::vector<std::string> tombSMG = { PHARO, WEEVIL, VESPER, MP40, KUDA, THOMPSON, VMP, AK74 }; std::vector<std::string> tombLauncher = { L4, XM }; std::vector<std::string> tombShotgun = { HAYMAKER, ARGUS, KRM, BRECCI }; std::vector<std::string> tombPistol = { RK5, MAUSER, LCAR }; std::vector<std::string> tombLMG = { DINGO, DREDGE, BRM, MG08, GORGON, RPK }; std::vector<std::string> tombAR = { ICR, HVK, MANOWAR, M8, SHIEVA, KN, STG, M14 }; std::vector<std::string> tombSpecial = { ICESTAFF, LIGHTNINGSTAFF, FIRESTAFF, WINDSTAFF, RAYGUNMK2, RAYGUN }; std::vector<std::string> tombMelee = { BOWIEKNIFE }; std::vector<std::string> tombEquipment = { MONKEY, GSTRIKE, TRIPMINE, GRENADES, SHIELD }; std::vector<std::string> tombHero = { ANNIHILATOR };
    weaponList.insert({ "zm_zod_sniper", zodSniper }); weaponList.insert({ "zm_zod_smg", zodSMG }); weaponList.insert({ "zm_zod_launcher", zodLauncher }); weaponList.insert({ "zm_zod_shotgun", zodShotgun }); weaponList.insert({ "zm_zod_pistol", zodPistol }); weaponList.insert({ "zm_zod_lmg", zodLMG }); weaponList.insert({ "zm_zod_ar", zodAR }); weaponList.insert({ "zm_zod_special", zodSpecial }); weaponList.insert({ "zm_zod_melee", zodMelee }); weaponList.insert({ "zm_zod_equipment", zodEquipment }); weaponList.insert({ "zm_zod_hero", zodHero });
    weaponList.insert({ "zm_factory_sniper", factorySniper }); weaponList.insert({ "zm_factory_smg", factorySMG }); weaponList.insert({ "zm_factory_launcher", factoryLauncher }); weaponList.insert({ "zm_factory_shotgun", factoryShotgun }); weaponList.insert({ "zm_factory_pistol", factoryPistol }); weaponList.insert({ "zm_factory_lmg", factoryLMG }); weaponList.insert({ "zm_factory_ar", factoryAR }); weaponList.insert({ "zm_factory_special", factorySpecial }); weaponList.insert({ "zm_factory_melee", factoryMelee }); weaponList.insert({ "zm_factory_equipment", factoryEquipment }); weaponList.insert({ "zm_factory_hero", factoryHero });
    weaponList.insert({ "zm_castle_sniper", castleSniper }); weaponList.insert({ "zm_castle_smg", castleSMG }); weaponList.insert({ "zm_castle_launcher", castleLauncher }); weaponList.insert({ "zm_castle_shotgun", castleShotgun }); weaponList.insert({ "zm_castle_pistol", castlePistol }); weaponList.insert({ "zm_castle_lmg", castleLMG }); weaponList.insert({ "zm_castle_ar", castleAR }); weaponList.insert({ "zm_castle_special", castleSpecial }); weaponList.insert({ "zm_castle_melee", castleMelee }); weaponList.insert({ "zm_castle_equipment", castleEquipment }); weaponList.insert({ "zm_castle_hero", castleHero });
    weaponList.insert({ "zm_island_sniper", islandSniper }); weaponList.insert({ "zm_island_smg", islandSMG }); weaponList.insert({ "zm_island_launcher", islandLauncher }); weaponList.insert({ "zm_island_shotgun", islandShotgun }); weaponList.insert({ "zm_island_pistol", islandPistol }); weaponList.insert({ "zm_island_lmg", islandLMG }); weaponList.insert({ "zm_island_ar", islandAR }); weaponList.insert({ "zm_island_special", islandSpecial }); weaponList.insert({ "zm_island_melee", islandMelee }); weaponList.insert({ "zm_island_equipment", islandEquipment }); weaponList.insert({ "zm_island_hero", islandHero });
    weaponList.insert({ "zm_stalingrad_sniper", stalingradSniper }); weaponList.insert({ "zm_stalingrad_smg", stalingradSMG }); weaponList.insert({ "zm_stalingrad_launcher", stalingradLauncher }); weaponList.insert({ "zm_stalingrad_shotgun", stalingradShotgun }); weaponList.insert({ "zm_stalingrad_pistol", stalingradPistol }); weaponList.insert({ "zm_stalingrad_lmg", stalingradLMG }); weaponList.insert({ "zm_stalingrad_ar", stalingradAR }); weaponList.insert({ "zm_stalingrad_special", stalingradSpecial }); weaponList.insert({ "zm_stalingrad_melee", stalingradMelee }); weaponList.insert({ "zm_stalingrad_equipment", stalingradEquipment }); weaponList.insert({ "zm_stalingrad_hero", stalingradHero });
    weaponList.insert({ "zm_genesis_sniper", genesisSniper }); weaponList.insert({ "zm_genesis_smg", genesisSMG }); weaponList.insert({ "zm_genesis_launcher", genesisLauncher }); weaponList.insert({ "zm_genesis_shotgun", genesisShotgun }); weaponList.insert({ "zm_genesis_pistol", genesisPistol }); weaponList.insert({ "zm_genesis_lmg", genesisLMG }); weaponList.insert({ "zm_genesis_ar", genesisAR }); weaponList.insert({ "zm_genesis_special", genesisSpecial }); weaponList.insert({ "zm_genesis_melee", genesisMelee }); weaponList.insert({ "zm_genesis_equipment", genesisEquipment }); weaponList.insert({ "zm_genesis_hero", genesisHero });
    weaponList.insert({ "zm_prototype_sniper", prototypeSniper }); weaponList.insert({ "zm_prototype_smg", prototypeSMG }); weaponList.insert({ "zm_prototype_launcher", prototypeLauncher }); weaponList.insert({ "zm_prototype_shotgun", prototypeShotgun }); weaponList.insert({ "zm_prototype_pistol", prototypePistol }); weaponList.insert({ "zm_prototype_lmg", prototypeLMG }); weaponList.insert({ "zm_prototype_ar", prototypeAR }); weaponList.insert({ "zm_prototype_special", prototypeSpecial }); weaponList.insert({ "zm_prototype_melee", prototypeMelee }); weaponList.insert({ "zm_prototype_equipment", prototypeEquipment }); weaponList.insert({ "zm_prototype_hero", prototypeHero });
    weaponList.insert({ "zm_asylum_sniper", asylumSniper }); weaponList.insert({ "zm_asylum_smg", asylumSMG }); weaponList.insert({ "zm_asylum_launcher", asylumLauncher }); weaponList.insert({ "zm_asylum_shotgun", asylumShotgun }); weaponList.insert({ "zm_asylum_pistol", asylumPistol }); weaponList.insert({ "zm_asylum_lmg", asylumLMG }); weaponList.insert({ "zm_asylum_ar", asylumAR }); weaponList.insert({ "zm_asylum_special", asylumSpecial }); weaponList.insert({ "zm_asylum_melee", asylumMelee }); weaponList.insert({ "zm_asylum_equipment", asylumEquipment }); weaponList.insert({ "zm_asylum_hero", asylumHero });
    weaponList.insert({ "zm_sumpf_sniper", sumpfSniper }); weaponList.insert({ "zm_sumpf_smg", sumpfSMG }); weaponList.insert({ "zm_sumpf_launcher", sumpfLauncher }); weaponList.insert({ "zm_sumpf_shotgun", sumpfShotgun }); weaponList.insert({ "zm_sumpf_pistol", sumpfPistol }); weaponList.insert({ "zm_sumpf_lmg", sumpfLMG }); weaponList.insert({ "zm_sumpf_ar", sumpfAR }); weaponList.insert({ "zm_sumpf_special", sumpfSpecial }); weaponList.insert({ "zm_sumpf_melee", sumpfMelee }); weaponList.insert({ "zm_sumpf_equipment", sumpfEquipment }); weaponList.insert({ "zm_sumpf_hero", sumpfHero });
    weaponList.insert({ "zm_theater_sniper", theaterSniper }); weaponList.insert({ "zm_theater_smg", theaterSMG }); weaponList.insert({ "zm_theater_launcher", theaterLauncher }); weaponList.insert({ "zm_theater_shotgun", theaterShotgun }); weaponList.insert({ "zm_theater_pistol", theaterPistol }); weaponList.insert({ "zm_theater_lmg", theaterLMG }); weaponList.insert({ "zm_theater_ar", theaterAR }); weaponList.insert({ "zm_theater_special", theaterSpecial }); weaponList.insert({ "zm_theater_melee", theaterMelee }); weaponList.insert({ "zm_theater_equipment", theaterEquipment }); weaponList.insert({ "zm_theater_hero", theaterHero });
    weaponList.insert({ "zm_cosmodrome_sniper", cosmodromeSniper }); weaponList.insert({ "zm_cosmodrome_smg", cosmodromeSMG }); weaponList.insert({ "zm_cosmodrome_launcher", cosmodromeLauncher }); weaponList.insert({ "zm_cosmodrome_shotgun", cosmodromeShotgun }); weaponList.insert({ "zm_cosmodrome_pistol", cosmodromePistol }); weaponList.insert({ "zm_cosmodrome_lmg", cosmodromeLMG }); weaponList.insert({ "zm_cosmodrome_ar", cosmodromeAR }); weaponList.insert({ "zm_cosmodrome_special", cosmodromeSpecial }); weaponList.insert({ "zm_cosmodrome_melee", cosmodromeMelee }); weaponList.insert({ "zm_cosmodrome_equipment", cosmodromeEquipment }); weaponList.insert({ "zm_cosmodrome_hero", cosmodromeHero });
    weaponList.insert({ "zm_temple_sniper", templeSniper }); weaponList.insert({ "zm_temple_smg", templeSMG }); weaponList.insert({ "zm_temple_launcher", templeLauncher }); weaponList.insert({ "zm_temple_shotgun", templeShotgun }); weaponList.insert({ "zm_temple_pistol", templePistol }); weaponList.insert({ "zm_temple_lmg", templeLMG }); weaponList.insert({ "zm_temple_ar", templeAR }); weaponList.insert({ "zm_temple_special", templeSpecial }); weaponList.insert({ "zm_temple_melee", templeMelee }); weaponList.insert({ "zm_temple_equipment", templeEquipment }); weaponList.insert({ "zm_temple_hero", templeHero });
    weaponList.insert({ "zm_moon_sniper", moonSniper }); weaponList.insert({ "zm_moon_smg", moonSMG }); weaponList.insert({ "zm_moon_launcher", moonLauncher }); weaponList.insert({ "zm_moon_shotgun", moonShotgun }); weaponList.insert({ "zm_moon_pistol", moonPistol }); weaponList.insert({ "zm_moon_lmg", moonLMG }); weaponList.insert({ "zm_moon_ar", moonAR }); weaponList.insert({ "zm_moon_special", moonSpecial }); weaponList.insert({ "zm_moon_melee", moonMelee }); weaponList.insert({ "zm_moon_equipment", moonEquipment }); weaponList.insert({ "zm_moon_hero", moonHero });
    weaponList.insert({ "zm_tomb_sniper", tombSniper }); weaponList.insert({ "zm_tomb_smg", tombSMG }); weaponList.insert({ "zm_tomb_launcher", tombLauncher }); weaponList.insert({ "zm_tomb_shotgun", tombShotgun }); weaponList.insert({ "zm_tomb_pistol", tombPistol }); weaponList.insert({ "zm_tomb_lmg", tombLMG }); weaponList.insert({ "zm_tomb_ar", tombAR }); weaponList.insert({ "zm_tomb_special", tombSpecial }); weaponList.insert({ "zm_tomb_melee", tombMelee }); weaponList.insert({ "zm_tomb_equipment", tombEquipment }); weaponList.insert({ "zm_tomb_hero", tombHero });
}
}

namespace BGB
{
#define PRESET_FILE "Gum Profiles/GumPresets.csv"

    void InitBGBDescriptions()
    {
        gumDescriptions.reserve(63);
        gumDescriptions.push_back("Activated (2x Activations, 60 seconds each)\nEvery 10 points is instead awarded 1 ammo in the stock of the current weapon.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 full rounds)\nWalk faster while aiming. Raise and lower your weapon to aim more quickly.");
        gumDescriptions.push_back("Activated (2x Activations)\nInstantly teleport to a random location. A concussive blast knocks away any nearby zombies, keeping you safe.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 full rounds)\nSwitch weapons and recover from performing melee attacks faster. Reload and use items more quickly.");
        gumDescriptions.push_back("Activates Immediately (Lasts until next respawn)\nRespawn with the guns you had when you bled out.");
        gumDescriptions.push_back("Activates Immediately (Lasts 10 minutes)\nCharge your special weapon faster.");
        gumDescriptions.push_back("Activates Immediately (Lasts 20 minutes)\nLonger bleedout time.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 full rounds)\nZero explosive damage.");
        gumDescriptions.push_back("Activated (4x Activations)\nOverrides the colors you see.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 full rounds)\nCan fire while sprinting.");
        gumDescriptions.push_back("Activates Immediately (Lasts until bleedout)\nRespawn near the end of the current round instead of at the start of the next round.");
        gumDescriptions.push_back("Activated (2x Activations, 10 seconds each)\nYou are ignored by zombies for 10 seconds.");
        gumDescriptions.push_back("Activates Immediately (Lasts 1 full round)\nImproves your chances of activating an Alternate Ammo Type.");
        gumDescriptions.push_back("Activates Immediately (Lasts 25 minutes)\nZombies killed fall straight up.");
        gumDescriptions.push_back("Activated (2x Activations)\nAll zombies will chase you for 10 seconds.");
        gumDescriptions.push_back("Activates Immediately (Lasts 5 full rounds)\nZombies you kill with grenades and large projectiles vomit uncontrollably.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 minutes)\nAmmo is taken from your stockpile instead of your weapon's magazine.");
        gumDescriptions.push_back("Activates Immediately (Lasts 2.5 minutes)\nMelee attacks deal zombies 5x as much damage.");
        gumDescriptions.push_back("Auto-activates when killing a zombie (25x Activations)\nSilly sounds play when zombies are killed.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 full rounds)\nKeep all perks after being revived.");
        gumDescriptions.push_back("Activates Immediately (Lasts 5 full rounds)\nRepairing a board immediately repairs all boards at that window.");
        gumDescriptions.push_back("Activates Immediately (Lasts for 5 minutes)\nRepairing a board will kill all nearby zombies.");
        gumDescriptions.push_back("Activated (2x Activations)\nRe-Pack your current Pack-a-Punched gun(if supported).");
        gumDescriptions.push_back("Activates Immediately (Lasts two hits)\nThe next time you take melee damage, nearby zombies burst into fire.");
        gumDescriptions.push_back("Activated (1x Activation)\nSpawns a max ammo power up.");
        gumDescriptions.push_back("Auto-activates next time you take a gun from the magic box\nThe next gun taken from the magic box comes Pack-a-Punched.");
        gumDescriptions.push_back("Activated (5x Activations)\nAll nearby zombies become crawlers.");
        gumDescriptions.push_back("Activated (2x Activations)\nSpawns a nuke power up.");
        gumDescriptions.push_back("Activates Immediately (Lasts for 5 minutes)\nGives a random gun every 10 seconds.");
        gumDescriptions.push_back("Activated (2x Activations, 60 seconds each)\nTurns the weapon in your hands into Pack-a-Punched weapon.");
        gumDescriptions.push_back("Activated (4x Activations)\nSpawns a personal 1250 point power up.");
        gumDescriptions.push_back("Activated (2x Activations)\nSpawn a Death Machine power up.");
        gumDescriptions.push_back("Activated (1x Activation, 2 minutes)\nZombies seen by players will not move.");
        gumDescriptions.push_back("Activates Immediately (2x Activations)\nTransforms into a random Mega GobbleGum not in your Pack.");
        gumDescriptions.push_back("Activates Immediately (Lasts for the remainder of the round)\nAny bullet which hits a zombie will damage its head.");
        gumDescriptions.push_back("Activated (3x Activations, 30 seconds each)\nAll zombies will ignore all players.");
        gumDescriptions.push_back("Activated (2x Activations)\nSpawn a random power up.");
        gumDescriptions.push_back("Activated (3x Activations)\nSpawns a fire sale power up.");
        gumDescriptions.push_back("Activated (2x Activations)\nSpawns an Instakill power up.");
        gumDescriptions.push_back("Activated (1x Activation)\nAll zombies freeze in place for 20 seconds.If they are shot, they will be annihilated when the time is up.");
        gumDescriptions.push_back("Activated (3x Activations)\nSpawns a Carpenter power up.");
        gumDescriptions.push_back("Activated (3x Activations)\nGib the heads of all zombies you can see, killing them.");
        gumDescriptions.push_back("Activates Immediately (Lasts 3 full rounds)\nRevive, or be revived, simply by being near other players. Revived players keep all of their perks.");
        gumDescriptions.push_back("Activated (1x Activation)\nSpawns a free perk power up.");
        gumDescriptions.push_back("Activates Immediately\nGives you all perks in the map.");
        gumDescriptions.push_back("Activated (1x Activation)\nRevives all teammates. Teammates keep all of their perks.");
        gumDescriptions.push_back("Auto-activates when melee attacking zombies (5x Activations)\nMelee attacks trigger an electrostatic discharge, electrocuting nearby zombies.");
        gumDescriptions.push_back("Activates Immediately (Lasts for 4 full rounds)\nMore power ups can drop each round.");
        gumDescriptions.push_back("Activates Immediately (Lasts 10 minutes)\nPoints you earn are also received by nearby players, and vice versa.");
        gumDescriptions.push_back("Activated (2x Activations)\nSpawns one of each of the nine core power ups.");
        gumDescriptions.push_back("Activated (2x Activations)\nRe-spins the weapons in a magic box after it has been activated.");
        gumDescriptions.push_back("Activated (1x Activation)\nEnds the current round. All players gain 1600 points.");
        gumDescriptions.push_back("Activates Immediately (Lasts 10 minutes)\nAny gun wall-buy can be used to buy ammo for any gun.");
        gumDescriptions.push_back("Auto-activates by getting a kill in last stand (3x Activations)\nAuto revive yourself. Keep all of your perks.");
        gumDescriptions.push_back("Activates Immediately (Lasts 1 minute)\nAll purchases are free.");
        gumDescriptions.push_back("Auto-activates when sliding (6x Activations)\nCreate 2 lethal explosions by sliding.");
        gumDescriptions.push_back("Auto-activates when you buy a perk (5x Activations)\nCan buy an extra perk. Gives you a free perk after you buy one.");
        gumDescriptions.push_back("Activates Immediately (Lasts 1 full round)\nPower ups last longer.");
        gumDescriptions.push_back("Auto-activates when a teddy bear appears in the magic box.\nMagic box re-spins automatically. Magic box will not move for several uses.");
        gumDescriptions.push_back("Activates Immediately (Lasts 4 minutes)\nSlow down all zombies to shambling speed.");
        gumDescriptions.push_back("Auto-activates when you have maximum perks.\nCan buy an extra perk.");
        gumDescriptions.push_back("Auto-activates on your next wall-buy gun purchase\nThe next gun bought off a wall comes Pack-a-Punched.");
        gumDescriptions.push_back("Activated (2x Activations)\nSpawns a double points power up.");
    }

    void InitClassicGumsList()
    {
        classicList.push_back("Alchemical Antithesis");
        classicList.push_back("Always Done Swiftly");
        classicList.push_back("Anywhere But Here!");
        classicList.push_back("Armamental Accomplishment");
        classicList.push_back("Arms Grace");
        classicList.push_back("Arsenal Accelerator");
        classicList.push_back("Coagulant");
        classicList.push_back("Danger Closest");
        classicList.push_back("Eye Candy");
        classicList.push_back("Firing On All Cylinders");
        classicList.push_back("Impatient");
        classicList.push_back("In Plain Sight");
        classicList.push_back("Lucky Crit");
        classicList.push_back("Newtonian Negation");
        classicList.push_back("Now You See Me");
        classicList.push_back("Projectile Vomiting");
        classicList.push_back("Stock Option");
        classicList.push_back("Sword Flay");
        classicList.push_back("Tone Death");
    }

    void InitMegaGumsList()
    {
        megaList.push_back("Aftertaste");
        megaList.push_back("Board Games");
        megaList.push_back("Board To Death");
        megaList.push_back("Bullet Boost");
        megaList.push_back("Burned Out");
        megaList.push_back("Cache Back");
        megaList.push_back("Crate Power");
        megaList.push_back("Crawl Space");
        megaList.push_back("Dead Of Nuclear Winter");
        megaList.push_back("Disorderly Combat");
        megaList.push_back("Ephemeral Enhancement");
        megaList.push_back("Extra Credit");
        megaList.push_back("Fatal Contraption");
        megaList.push_back("Fear In Headlights");
        megaList.push_back("Flavor Hexed");
        megaList.push_back("Head Drama");
        megaList.push_back("Idle Eyes");
        megaList.push_back("I'm Feelin' Lucky");
        megaList.push_back("Immolation Liquidation");
        megaList.push_back("Kill Joy");
        megaList.push_back("Killing Time");
        megaList.push_back("Licensed Contractor");
        megaList.push_back("Mind Blown");
        megaList.push_back("Near Death Experience");
        megaList.push_back("On The House");
        megaList.push_back("Perkaholic");
        megaList.push_back("Phoenix Up");
        megaList.push_back("Pop Shocks");
        megaList.push_back("Power Vacuum");
        megaList.push_back("Profit Sharing");
        megaList.push_back("Reign Drops");
        megaList.push_back("Respin Cycle");
        megaList.push_back("Round Robbin'");
        megaList.push_back("Secret Shopper");
        megaList.push_back("Self Medication");
        megaList.push_back("Shopping Free");
        megaList.push_back("Slaughter Slide");
        megaList.push_back("Soda Fountain");
        megaList.push_back("Temporal Gift");
        megaList.push_back("Unbearable");
        megaList.push_back("Undead Man Walking");
        megaList.push_back("Unquenchable");
        megaList.push_back("Wall Power");
        megaList.push_back("Who's Keeping Score");
    }

    void LoadGumProfiles()
    {
        gumPresets.clear();

        std::string line;
        std::ifstream file(PRESET_FILE);

        while (getline(file, line))
        {
            BGBPreset loadedPreset;
            std::stringstream sstream(line);
            std::string substr;
            getline(sstream, substr, ',');
            loadedPreset.presetName = substr;
            for (int i = 1; i < 6; i++)
            {
                getline(sstream, substr, ',');
                loadedPreset.presetGums[i - 1] = atoi(substr.c_str());
            }
            gumPresets.push_back(loadedPreset);
        }
        file.close();

        if (gumPresets.size())
            gumContextIndex = gumPresets[currentPreset].presetGums[0];
        else
        {
            BGBPreset tempFill;
            tempFill.presetGums = { 0, 1, 2, 3, 4 };
            tempFill.presetName = "No Presets Available";
            gumPresets.push_back(tempFill);
        }
    }

    void DeleteGumPreset(const std::string& presetName)
    {
        std::string line;
        std::string outData;
        std::ifstream inFile(PRESET_FILE);

        while (getline(inFile, line))
        {
            std::stringstream sstream(line);
            std::string name;
            getline(sstream, name, ',');
            if (name != presetName)
                outData.append(line + "\n");
        }
        inFile.close();

        std::ofstream outFile(PRESET_FILE);
        outFile << outData;
        outFile.close();
        if (currentPreset > 0) currentPreset--;
        LoadGumProfiles();
    }

    void CreateNewGumPreset(const std::string& presetName)
    {
        std::string line;
        std::ifstream checkFile(PRESET_FILE);
        while (getline(checkFile, line));

        std::ofstream file(PRESET_FILE, std::ios::app);
        if (line != "")
            file << "\n";
        checkFile.close();
        file << presetName << ",0,1,2,3,4\n";
        file.close();
        LoadGumProfiles();
        currentPreset = (int)gumPresets.size() - 1;
    }

    std::string GetCurrentPresetName()
    {
        return gumPresets[currentPreset].presetName;
    }

    bool CheckPresetExists(const std::string& inPreset)
    {
        for (const BGBPreset& preset : gumPresets)
        {
            if (preset.presetName == inPreset)
                return true;
        }
        return false;
    }

    void WriteGumPreset(const std::vector<int>& gumPreset)
    {
        std::string line;
        std::string outData;
        std::ifstream inFile(PRESET_FILE);

        while (getline(inFile, line))
        {
            std::stringstream sstream(line);
            std::string name;
            getline(sstream, name, ',');
            std::string preset(GetCurrentPresetName());
            if (name != preset)
            {
                outData.append(line + "\n");
                continue;
            }
            outData.append(preset + ",");
            for (const int& gum : gumPreset)
            {
                outData.append(std::to_string(gum) + ",");
            }
            outData.replace(outData.size() - 1, 1, "\n");
        }
        inFile.close();

        std::ofstream outFile(PRESET_FILE);
        outFile << outData;
        outFile.close();

        LoadGumProfiles();
    }

    void WritePresetToGame(BGBPreset& gumPreset, const std::string& file)
    {
        if (gumPreset.presetName == "No Presets Available")
            gumPreset = inactivePreset;
        std::string outData;
        for (const int& gum : gumPreset.presetGums)
        {
            outData.append(std::to_string(gum) + ",");
        }
        outData.replace(outData.size() - 1, 1, "");
        std::ofstream outFile(file);
        outFile << outData;
        outFile.close();
    }
}

namespace Perks
{
#pragma region Perks
#define JUG "Juggernog"
#define SPEED "Speed Cola"
#define DTAP "Double Tap"
#define QR "Quick Revive"
#define STAM "Stamin-Up"
#define MULEKICK "Mule Kick"
#define DEADSHOT "Deadshot Daiquiri"
#define WIDOWS "Widow's Wine"
#define CHERRY "Electric Cherry"
#pragma endregion Perks

    void InitPerksList()
    {
        std::vector<std::string> zodPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, WIDOWS };
        std::vector<std::string> factoryPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT };
        std::vector<std::string> castlePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> islandPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> stalingradPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> genesisPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        std::vector<std::string> prototypePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> asylumPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> sumpfPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> theaterPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> cosmodromePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> templePerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> moonPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS };
        std::vector<std::string> tombPerks = { JUG, SPEED, DTAP, QR, STAM, MULEKICK, DEADSHOT, WIDOWS, CHERRY };
        perksList.insert({ "zm_zod",  zodPerks });
        perksList.insert({ "zm_factory",  factoryPerks });
        perksList.insert({ "zm_castle",  castlePerks });
        perksList.insert({ "zm_island",  islandPerks });
        perksList.insert({ "zm_stalingrad",  stalingradPerks });
        perksList.insert({ "zm_genesis",  genesisPerks });
        perksList.insert({ "zm_prototype",  prototypePerks });
        perksList.insert({ "zm_asylum",  asylumPerks });
        perksList.insert({ "zm_sumpf",  sumpfPerks });
        perksList.insert({ "zm_theater",  theaterPerks });
        perksList.insert({ "zm_cosmodrome",  cosmodromePerks });
        perksList.insert({ "zm_temple",  templePerks });
        perksList.insert({ "zm_moon",  moonPerks });
        perksList.insert({ "zm_tomb",  tombPerks });
    }
}