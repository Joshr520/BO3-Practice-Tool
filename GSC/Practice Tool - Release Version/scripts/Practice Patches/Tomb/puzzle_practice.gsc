PuzzlePractice()
{
    WaitFadeIn();
    if(level.script != "zm_tomb")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Tomb");
        return;
    }
    
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Puzzle Practice Starting");

    level flag::set("puzzle_practice");

    self thread DebugTesting();
    self TakeAllWeapons();
    self thread IceResetWatch();
    self thread WindResetWatch();
    self thread ShowPuzzleControls();

    self zm_weapons::weapon_give(GetWeapon("staff_water"));
    self zm_weapons::weapon_give(GetWeapon("staff_air"));
    level flag::clear("spawn_zombies");

    self SetOrigin((10345.8, -7916.6, -411.875));
}

IceResetWatch()
{
    for(;;)
    {
        WaitF5();
        thread WriteToScreen("Resetting Ice Puzzle");
        level flag::clear("ice_tile_flipping");
        level notify("ice_puzzle_1_complete");
        [[ @zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::reset_tiles ]]();
        wait 2;
        foreach(index, tile in GetEntArray("ice_ceiling_tile", "script_noteworthy")) tile thread [[ @zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::ceiling_tile_process_damage ]]();
        level thread [[ @zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::update_ternary_display ]]();
        wait 0.1;
        [[ @zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::change_ice_gem_value ]]();
    }
}

WindResetWatch()
{
    for(;;)
    {
        WaitF6();
        thread WriteToScreen("Resetting Wind Puzzle");
        level notify("air_puzzle_1_complete");
        level flag::clear("air_puzzle_1_complete");
        [[ @zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::air_puzzle_1_init ]]();
        [[ @zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::air_puzzle_1_run ]]();
    }
}

ShowPuzzleControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 10, 90);
    controls SetText("Controls List\nHide Controls: F3\nReset Ice Puzzle: F5\nReset Wind Puzzle: F6");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}

detour zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::ice_puzzle_1_cleanup()
{
	if(!level flag::get("puzzle_practice")) [[ @zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::ice_puzzle_1_cleanup ]]();
}

detour zm_tomb_quest_ice<scripts\zm\zm_tomb_quest_ice.gsc>::ice_tiles_randomize()
{
	a_original_tiles = GetEntArray("ice_tile_original", "targetname");
	a_original_tiles = array::sort_by_script_int(a_original_tiles, 1);
	a_original_positions = [];
	foreach(e_tile in a_original_tiles)
	{
		a_original_positions[a_original_positions.size] = e_tile.origin;
	}
	a_unused_tiles = GetEntArray("ice_ceiling_tile", "script_noteworthy");
	n_total_tiles = a_unused_tiles.size;
	n_index = 0;
	foreach(v_pos in a_original_positions)
	{
		e_tile = array::random(a_unused_tiles);
		ArrayRemoveValue(a_unused_tiles, e_tile, 0);
		e_tile MoveTo(v_pos, 0.5);
		e_tile waittill("movedone");
		str_model_name = "ice_ceiling_tile_model_" + n_index;
		var_fa4117e3 = GetEnt(str_model_name, "targetname");
		var_fa4117e3 LinkTo(e_tile);
		n_index++;
	}
	if(!level flag::get("puzzle_practice")) array::delete_all(a_unused_tiles);
}

detour zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::air_puzzle_1_cleanup()
{
	if(!level flag::get("puzzle_practice")) [[ @zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::air_puzzle_1_cleanup ]]();
}

detour zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::ceiling_ring_rotate()
{
	self.position = (self.position + 1) % 4;
	self [[ @zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::ceiling_ring_update_position ]]();
	solved = [[ @zm_tomb_quest_air<scripts\zm\zm_tomb_quest_air.gsc>::check_puzzle_solved ]]();
	if(solved && !level flag::get("air_puzzle_1_complete"))
	{
        if(level flag::get("puzzle_practice")) PlaySoundAtPosition("zmb_squest_step1_finished", (0, 0, 0));
        level flag::set("air_puzzle_1_complete");
	}
}