Boss2Practice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_genesis")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Rev");
        return;
    }
    thread WriteToScreen("Boss 2 Practice Starting");
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    level flag::set("character_stones_done");
    level flag::set("phased_sophia_start");
    level flag::set("toys_collected");
    level flag::set("boss_fight");
    level flag::set("rune_circle_on");
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self zm_weapons::weapon_give(GetWeapon("shotgun_pump_upgraded"), 1);
    self zm_weapons::weapon_give(GetWeapon("idgun_genesis_0_upgraded"), 1);
    self [[level.zombie_weapons_callbacks[level.w_octobomb]]]();
    self thread bgb::give("zm_bgb_reign_drops");
    self SetOrigin((-133.799, -7799.34, -3607.38));
    self SetPlayerAngles((4.39453, -123.492, 0));
    while(!IsDefined(level.var_40ffc71d)) wait 0.05;
    progress = hud::createfontstring("big", 1.25);
    progress hud::setpoint("TOPLEFT", "TOPLEFT", 20, 5);
    progress.alpha = 1;
    progress SetText("Boss Progress: " + ((level.var_40ffc71d / 15) * 100));
    previous = level.var_40ffc71d;
    while(level.var_40ffc71d < 15)
    {
        if(level.var_40ffc71d > previous)
        {
            previous = level.var_40ffc71d;
            progress SetText("Boss Progress: " + level.var_40ffc71d + "/15");
        }
        wait 0.05;
    }
    progress SetText("Boss Souls Finished!");
    wait 3;
    progress FadeOverTime(5);
    progress.alpha = 0;
    wait 5;
    progress Destroy();
}