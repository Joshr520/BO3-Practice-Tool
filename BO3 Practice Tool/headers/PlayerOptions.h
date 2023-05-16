#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Points
{
    // menu ints
    inline int pointInput = 0;
}

namespace Weapons
{
    void InitWeaponsList();

    inline std::unordered_map<std::string, std::vector<std::string>> weaponList;

    inline bool upgradedWeapon = false;
}

namespace BGB
{
    struct BGBPreset
    {
        std::string presetName;
        std::vector<int> presetGums { 0, 0, 0, 0, 0 };
    };

    void InitBGBDescriptions();
    void InitClassicGumsList();
    void InitMegaGumsList();
    void LoadGumProfiles();
    void DeleteGumPreset(const std::string& preset);
    void CreateNewGumPreset(const std::string& presetName);
    std::string GetCurrentPresetName();
    bool CheckPresetExists(const std::string& inPreset);
    void WriteGumPreset(const std::vector<int>& gumPreset);
    void WritePresetToGame(BGBPreset& gumPreset, const std::string& file);

    inline std::vector<std::string> classicList;
    inline std::vector<std::string> megaList;
    inline std::vector<std::string> gumDescriptions;
    inline std::vector<BGBPreset> gumPresets;
    inline int currentGumPreset = 0;
    inline int gumSelectIndex = 0;
    inline int gumContextIndex = 0;
    inline bool showGumSelection = false;
    inline bool writeGums = false;
    inline BGBPreset inactiveGumPreset = { "No Presets Available", { -1, -1, -1, -1, -1 } };
}

namespace Perks
{
    void InitPerksList();

    inline std::unordered_map<std::string, std::vector<std::string>> perksList;
}

namespace PlayerOptions
{
    // menu ints
    inline int timescaleInt = 1;

    // toggle bools
    inline bool godActive = false;
    inline bool infAmmoActive = false;
}