LockdownPractice()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_stalingrad")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Stalingrad");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_shopping_free");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_shopping_free");
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Lockdown Practice Starting");
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self SetOrigin((-3270.17, 21277.4, 160.125));
    self SetPlayerAngles((4.19678, 61.4575, 0));
    self zm_weapons::weapon_give(level.w_raygun_mark3_upgraded, 1);
    self zm_weapons::weapon_give(GetWeapon("lmg_cqb"));
    self zm_weapons::weapon_give(GetWeapon("sniper_fastsemi"));
    self GiveRiotshield(0);
    self bgb::give("zm_bgb_dead_of_nuclear_winter");
    level flag::set("dragonride_crafted");
	level flag::set("dragon_pavlov_first_time");
    self thread TakePlayerGumCharge();
    thread SkipToRound(7);
    IPrintLnBold("Wait For Round Change");
}