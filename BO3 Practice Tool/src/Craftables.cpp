#include "craftables.h"

namespace Craftables
{
    void InitCraftablesList()
    {
        std::unordered_map<int, std::vector<std::string>> zodCraftables;
        zodCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        zodCraftables.insert({ 1, { "Fuse 1", "Fuse 2", "Fuse 3" } });
        zodCraftables.insert({ 2, { "Heart", "Tentacle", "Xenomatter" } });
        zodCraftables.insert({ 3, { "Boxer", "Detective", "Femme", "Magician" } });
        zodCraftables.insert({ 4, { "Boxer", "Detective", "Femme", "Magician" } });

        std::unordered_map<int, std::string> zodCraftNames;
        zodCraftNames.insert({ 0, "Shield" });
        zodCraftNames.insert({ 1, "Civil Protector" });
        zodCraftNames.insert({ 2, "ID Gun" });
        zodCraftNames.insert({ 3, "Mementos" });
        zodCraftNames.insert({ 4, "Gateworms" });

        std::unordered_map<int, int> zodCraftIndexes;
        zodCraftIndexes.insert({ 0, 0 });
        zodCraftIndexes.insert({ 1, 0 });
        zodCraftIndexes.insert({ 2, 0 });
        zodCraftIndexes.insert({ 3, 0 });
        zodCraftIndexes.insert({ 4, 0 });

        std::unordered_map<int, std::vector<std::string>> castleCraftables;
        castleCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        castleCraftables.insert({ 1, { "Body", "Guards", "Handle" } });

        std::unordered_map<int, std::string> castleCraftNames;
        castleCraftNames.insert({ 0, "Shield" });
        castleCraftNames.insert({ 1, "Rags"});

        std::unordered_map<int, int> castleCraftIndexes;
        castleCraftIndexes.insert({ 0, 0 });
        castleCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> islandCraftables;
        islandCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        islandCraftables.insert({ 1, { "Visor", "Filter", "Strap" } });

        std::unordered_map<int, std::string> islandCraftNames;
        islandCraftNames.insert({ 0, "Shield" });
        islandCraftNames.insert({ 1, "Gasmask" });

        std::unordered_map<int, int> islandCraftIndexes;
        islandCraftIndexes.insert({ 0, 0 });
        islandCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> stalingradCraftables;
        stalingradCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });

        std::unordered_map<int, std::string> stalingradCraftNames;
        stalingradCraftNames.insert({ 0, "Shield" });

        std::unordered_map<int, int> stalingradCraftIndexes;
        stalingradCraftIndexes.insert({ 0, 0 });
        stalingradCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> genesisCraftables;
        genesisCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        genesisCraftables.insert({ 1, { "Totem", "Head", "Gem" } });

        std::unordered_map<int, std::string> genesisCraftNames;
        genesisCraftNames.insert({ 0, "Shield" });
        genesisCraftNames.insert({ 1, "Keeper" });

        std::unordered_map<int, int> genesisCraftIndexes;
        genesisCraftIndexes.insert({ 0, 0 });
        genesisCraftIndexes.insert({ 1, 0 });

        std::unordered_map<int, std::vector<std::string>> tombCraftables;
        tombCraftables.insert({ 0, { "Dolly", "Door", "Clamp" } });
        tombCraftables.insert({ 1, { "Body", "Brain", "Engine" } });
        tombCraftables.insert({ 2, { "Vinyl Player", "Vinyl Master", "Vinyl Wind", "Vinyl Ice", "Vinyl Fire", "Vinyl Lightning" } });
        tombCraftables.insert({ 3, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });
        tombCraftables.insert({ 4, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });
        tombCraftables.insert({ 5, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });
        tombCraftables.insert({ 6, { "Gem", "Upper Staff", "Middle Staff", "Lower Staff" } });

        std::unordered_map<int, std::string> tombCraftNames;
        tombCraftNames.insert({ 0, "Shield" });
        tombCraftNames.insert({ 1, "Maxis Drone" });
        tombCraftNames.insert({ 2, "Gramophone" });
        tombCraftNames.insert({ 3, "Wind Staff" });
        tombCraftNames.insert({ 4, "Fire Staff" });
        tombCraftNames.insert({ 5, "Lightning Staff" });
        tombCraftNames.insert({ 6, "Ice Staff" });

        std::unordered_map<int, int> tombCraftIndexes;
        tombCraftIndexes.insert({ 0, 0 });
        tombCraftIndexes.insert({ 1, 0 });
        tombCraftIndexes.insert({ 2, 0 });
        tombCraftIndexes.insert({ 3, 0 });
        tombCraftIndexes.insert({ 4, 0 });
        tombCraftIndexes.insert({ 5, 0 });
        tombCraftIndexes.insert({ 6, 0 });

        craftList.insert({ "zm_zod", zodCraftables });
        craftList.insert({ "zm_castle", castleCraftables });
        craftList.insert({ "zm_island", islandCraftables });
        craftList.insert({ "zm_stalingrad", stalingradCraftables });
        craftList.insert({ "zm_genesis", genesisCraftables });
        craftList.insert({ "zm_tomb", tombCraftables });

        craftNames.insert({ "zm_zod", zodCraftNames });
        craftNames.insert({ "zm_castle", castleCraftNames });
        craftNames.insert({ "zm_island", islandCraftNames });
        craftNames.insert({ "zm_stalingrad", stalingradCraftNames });
        craftNames.insert({ "zm_genesis", genesisCraftNames });
        craftNames.insert({ "zm_tomb", tombCraftNames });

        craftComboIndexes.insert({ "zm_zod", zodCraftIndexes });
        craftComboIndexes.insert({ "zm_castle", castleCraftIndexes });
        craftComboIndexes.insert({ "zm_island", islandCraftIndexes });
        craftComboIndexes.insert({ "zm_stalingrad", stalingradCraftIndexes });
        craftComboIndexes.insert({ "zm_genesis", genesisCraftIndexes });
        craftComboIndexes.insert({ "zm_tomb", tombCraftIndexes });
    }
}