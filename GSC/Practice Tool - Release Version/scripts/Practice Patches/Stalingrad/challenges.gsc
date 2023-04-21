ChallengesPractice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_stalingrad")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Stalingrad");
        return;
    }
    thread WriteToScreen("Challenges Practice Starting");
    self.score = 6000;
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_shopping_free");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_shopping_free");
    self.bgb_pack_randomized = Array("zm_bgb_reign_drops", "zm_bgb_perkaholic", "zm_bgb_shopping_free");
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(level.w_raygun_mark3_upgraded, 1);
    self zm_weapons::weapon_give(GetWeapon("lmg_cqb"));
    self zm_weapons::weapon_give(GetWeapon("sniper_fastsemi"));
    self thread GiveGauntlet();
    self thread GiveDragonStrike();
    self GiveRiotshield(0);
    self SetOrigin((-275.725, 4768.26, 144.125));
    self SetPlayerAngles((5.32288, -179.615, 0));
    level flag::set("story_playing");
    level flag::set("dragonride_crafted");
	level flag::set("dragon_pavlov_first_time");
    level flag::set("key_placement");
    level flag::set("keys_placed");
    for(i = 1; i < 7; i++)
    {
        GetEnt("ee_map_shelf", "targetname") ShowPart("wall_map_shelf_figure_0" + i);
    }
    level flag::set("dragon_platforms_all_used");
    level flag::set("dragon_wings_items_aquired");
    thread SkipToRound(10);
    level.var_a78effc7 = 18;
    level.var_9d19c7e = "library";
    level.disable_nuke_delay_spawning = 1;
    IPrintLnBold("Wait...");
    wait 10;
    IPrintLnBold("Activate Challenges");
    level flag::clear("story_playing");
    level notify("ee_cylinder_acquired");
    level.var_a090a655 HidePart("button_green_jnt");
	level.var_a090a655 ShowPart("button_red_jnt");
}
