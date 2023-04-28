KeeperPractice()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_castle")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: DE");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Keeper Practice Starting");
    level flag::clear("spawn_zombies");
    level flag::set("story_playing");
    Timescale(10);
    self.score = 10000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    FinishStorm();
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self zm_weapons::weapon_give(GetWeapon("pistol_standard"));
    self TimeTravel(2);
    self SimonSays();
    self ActivateDempsey();
    thread SkipToRound(9);
    level flag::set("time_travel_teleporter_ready");
    tele = GetEntArray("trigger_teleport_pad", "targetname")[1];
    self SetOrigin(tele.origin);
    wait 0.25;
    tele notify("trigger", self);
    key_slot = struct::get("golden_key_slot_past");
    key_slot notify("trigger_activated");
    wait 0.5;
    tablet = GetEnt("stone_past", "targetname");
    tablet notify("trigger_activated");
    level flag::wait_till("channeling_stone_replacement");
    level flag::wait_till("spawn_zombies");
    Timescale(1);
    level notify("hash_b5927dd");
    thread KeeperProgress();
    level flag::clear("story_playing");
    level.var_1f18338d = "storm";
}

KeeperProgress()
{
    while(!IsDefined(GetEnt("keeper_archon_ai", "targetname"))) wait 0.05;
    progress = hud::createfontstring("big", 1.25);
    progress hud::setpoint("TOPLEFT", "TOPLEFT", 20, 5);
    progress.alpha = 1;
    progress SetText("Keeper Moving");
    keeper = GetEnt("keeper_archon_ai", "targetname");
    wait 5;
    while(!IsDefined(level.var_8ef26cd9))
    {
        keeper waittill("goal");
        thread UpdateProgress(progress);
        while(!KeeperCanMove()) wait 0.05;
        level notify("keeper_update_stop");
        progress SetText("Keeper Moving");
    }
    progress SetText("Keeper Finished!");
    wait 3;
    progress FadeOverTime(5);
    progress.alpha = 0;
    wait 5;
    progress Destroy();
}

UpdateProgress(progress)
{
    level endon("keeper_update_stop");
    while(level.var_8bdb0713) wait 0.05;
    souls = level.var_8bdb0713;
    progress SetText("Keeper Progress: " + souls + "/" + BowSoulCount());
    for(;;)
    {
        if(souls != level.var_8bdb0713)
        {
            souls = level.var_8bdb0713;
            progress SetText("Keeper Progress: " + souls + "/" + BowSoulCount());
        }
        wait 0.05;
    }
}

KeeperCanMove()
{
	cam_move = 0;
	if(level.var_8bdb0713 >= BowSoulCount())
	{
		if(IsTrue(level.var_f91118d9))
		{
			cam_move = 1;
		}
		else if(level.var_8bdb0713 >= PreRoundEndMove())
		{
			cam_move = 1;
		}
	}
	return cam_move;
}

BowSoulCount()
{
	soul_count = 16;
	switch(level.var_1f18338d)
	{
		case "demongate":
		{
			soul_count = 17;
			break;
		}
		case "rune_prison":
		{
			soul_count = 16;
			break;
		}
		case "storm":
		{
			soul_count = 20;
			break;
		}
		case "wolf_howl":
		{
			soul_count = 18;
			break;
		}
		default:
		{
			break;
		}
	}
	return soul_count;
}

PreRoundEndMove()
{
	soul_count = BowSoulCount();
	n_players = level.players.size;
	multiplier = 4 - (n_players - 1);
	soul_count = soul_count * multiplier;
	return soul_count;
}