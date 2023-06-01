TempLineups()
{
    WaitFadeIn();
    if(level.script != "zm_tomb")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Origins");
        return;
    }

    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Temp Lineups Practice Loaded");

    level flag::clear("spawn_zombies");
    self thread DebugTesting();
    self thread zm_weapons::weapon_give(level.w_beacon);

    level.fixed_temp_gen = 0;
    self thread WatchTempGenChange();
    self thread WatchTempSpawn();
    self thread WatchKillTemps();
    self thread CameraPos(0x77);
    self thread CameraToggle(0x78);

    self thread ShowTempLineupsControls();
}

WatchTempGenChange()
{
    // 3, 5, 4, 1, 2, 6
    gen_hud = hud::createfontstring("big", 1.25);
    gen_hud hud::setpoint("TOPLEFT", "TOPLEFT", 10, 75);
    gen_hud SetText("Current Gen: 1");
    for(;;)
    {
        WaitF5();
        level.fixed_temp_gen++;
        if(level.fixed_temp_gen > 5) level.fixed_temp_gen = 0;
        gen_hud SetText("Current Gen: " + Int(level.fixed_temp_gen + 1));
    }
}

WatchTempSpawn()
{
    for(;;)
    {
        WaitF6();
        if(level flag::get("recapture_event_in_progress"))
        {
            thread WriteToScreen("Temps Already Spawned");
            continue;
        }
        level thread SpawnTemps();
    }
}

WatchKillTemps()
{
    for(;;)
    {
        WaitF7();
        self KillHorde();
        foreach(generator in struct::get_array("s_generator", "targetname"))
        {
            generator [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::set_player_controlled_area ]]();
            generator.n_current_progress = 100;
        }
    }
}

SpawnTemps()
{
	level flag::set("recapture_event_in_progress");
	level flag::clear("recapture_zombies_cleared");
	level flag::clear("generator_under_attack");
	level.recapture_zombies_killed = 0;
	b_is_first_generator_attack = 1;
    gens = Array("generator_start_bunker", "generator_tank_trench", "generator_mid_trench", "generator_nml_right", "generator_nml_left", "generator_church");
	s_recapture_target_zone = level.zone_capture.zones[gens[level.fixed_temp_gen]];
	[[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::capture_event_handle_ai_limit ]]();
	[[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::recapture_round_audio_starts ]]();
	var_c746b61a = struct::get_array("generator_attackable", "targetname");
	foreach(var_b454101b in var_c746b61a)
	{
		var_b454101b [[ @zm_attackables<scripts\zm\_zm_attackables.gsc>::deactivate ]]();
		var_b454101b.health = 1000000;
		var_b454101b.max_health = var_b454101b.health;
		var_b454101b.aggro_distance = 1024;
	}
	while(!level flag::get("recapture_zombies_cleared") && [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::get_captured_zone_count ]]() > 0)
	{
		if ([[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::get_captured_zone_count ]]() < 6) s_recapture_target_zone = [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::get_recapture_zone ]](s_recapture_target_zone);
		var_28e07566 = s_recapture_target_zone.var_b454101b;
		level.zone_capture.recapture_target = s_recapture_target_zone.script_noteworthy;
		level.zone_capture.var_186a84eb = var_28e07566;
		s_recapture_target_zone [[ @zm_tomb_capture_zones_ffotd<scripts\zm\zm_tomb_capture_zones_ffotd.gsc>::recapture_event_start ]]();
		var_28e07566 [[ @zm_attackables<scripts\zm\_zm_attackables.gsc>::activate ]]();
		if(b_is_first_generator_attack)
		{
			s_recapture_target_zone thread [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::monitor_recapture_zombies ]]();
			util::delay(10, undefined, @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::broadcast_vo_category_to_team, "recapture_generator_attacked");
		}
		s_recapture_target_zone thread [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::generator_under_attack_warnings ]]();
		s_recapture_target_zone flag::set("current_recapture_target_zone");
		s_recapture_target_zone thread [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::hide_zone_objective_while_recapture_group_runs_to_next_generator ]](b_is_first_generator_attack);
		s_recapture_target_zone [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::activate_capture_zone ]](b_is_first_generator_attack);
		s_recapture_target_zone flag::clear("attacked_by_recapture_zombies");
		s_recapture_target_zone flag::clear("current_recapture_target_zone");
		var_28e07566 [[ @zm_attackables<scripts\zm\_zm_attackables.gsc>::deactivate ]]();
		if(!s_recapture_target_zone flag::get("player_controlled"))
		{
			util::delay(3, undefined, @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::broadcast_vo_category_to_team, "recapture_started");
		}
		b_is_first_generator_attack = 0;
		s_recapture_target_zone [[ @zm_tomb_capture_zones_ffotd<scripts\zm\zm_tomb_capture_zones_ffotd.gsc>::recapture_event_end ]]();
		wait(0.05);
	}
	if(s_recapture_target_zone.n_current_progress == 0 || s_recapture_target_zone.n_current_progress == 100)
	{
		s_recapture_target_zone [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::handle_generator_capture ]]();
	}
	[[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::capture_event_handle_ai_limit ]]();
	[[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::kill_all_recapture_zombies ]]();
	[[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::recapture_round_audio_ends ]]();
	var_c746b61a = struct::get_array("generator_attackable", "targetname");
	foreach(var_b454101b in var_c746b61a)
	{
		var_b454101b [[ @zm_attackables<scripts\zm\_zm_attackables.gsc>::deactivate ]]();
	}
	level flag::clear("recapture_event_in_progress");
	level flag::clear("generator_under_attack");
}

ShowTempLineupsControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 10, 90);
    controls SetText("Controls List\nHide Controls: F3\nChange Target Gen: F5\nSpawn Temps: F6\nKill Temps: F7\nSet Camera Pos: F8\nToggle Camera: F9");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}