NikolaiFight()
{
    if (level.mapname != "zm_stalingrad") {
        DebugPrint("error", "Nikolai Practice attempted to load on " + level.mapDisplayName + " but it requires Gorod Krovi");
        return;
    }

    WaitFadein();
    self GivePlayerWeapon("shield", false);
    self GivePlayerWeapon("launcher_dragon_strike", false);
    self GivePlayerWeapon("raygun_mark3", true);
    self GiveAllPerks();
    level flag::wait_till("initial_blackscreen_passed");
    level flag::set("nikolai_start");
}