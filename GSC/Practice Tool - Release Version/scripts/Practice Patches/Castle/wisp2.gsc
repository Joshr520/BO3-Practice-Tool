Wisp2Practice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_castle")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: DE");
        return;
    }
    thread WriteToScreen("Wisp 2 Practice Starting");
    level flag::clear("spawn_zombies");
    level flag::set("story_playing");
    level flag::set("castle_teleporter_used");
    wait 10;
    Timescale(10);
    self.score = 4500;
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self zm_weapons::weapon_give(GetWeapon("pistol_standard"));
    FinishStorm();
    self bgb::give("zm_bgb_extra_credit");
    thread SkipToRound(7, 1);
    wait 2;
    level flag::set("time_travel_teleporter_ready");
    tele = GetEntArray("trigger_teleport_pad", "targetname")[0];
    self SetOrigin(tele.origin);
    wait 0.25;
    tele notify("trigger", self);
    wait 25;
    Timescale(1);
    IPrintLnBold("Read Safe Code");
    wait 7;
    Timescale(10);
    fuse = struct::get("ee_lab_fuse");
    canister = struct::get("mpd_canister");
    fuse_found = 0;
    canister_found = 0;
    for(;;)
    {
        foreach(stub in level._unitriggers.trigger_stubs)
        {
            if(!fuse_found && fuse.origin == stub.origin)
            {
                self BuildAndActivateTrigger(stub);
                fuse_found = 1;
            }
            if(!canister_found && canister.origin == stub.origin)
            {
                self BuildAndActivateTrigger(stub);
                canister_found = 1;
            }
        }
        if(fuse_found && canister_found) break;
        wait 0.05;
    }
    level flag::wait_till("spawn_zombies");
    Timescale(1);
    level flag::clear("story_playing");
}