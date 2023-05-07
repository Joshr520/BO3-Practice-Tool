RainFireLineups()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_tomb")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Origins");
        return;
    }

    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Rain Fire Lineups Practice Loaded");

    level flag::clear("spawn_zombies");
    level flag::set("all_robot_hatch");
    self thread DebugTesting();
    self thread WaypointSetup(1);
    self thread zm_weapons::weapon_give(level.w_beacon);

    level flag::set("ee_all_staffs_crafted");
	level flag::set("ee_all_staffs_upgraded");
    level flag::set("ee_all_staffs_placed");

    GetEnt("fire_link_damage", "targetname") thread WaitDamage();
    level.last_fire_link_time = -35000;
    for(;;)
    {
        level flag::wait_till("fire_link_enabled");
        level flag::clear("fire_link_enabled");
        level.last_fire_link_time = GetTime();
        timer = InitHud();
        timer.y = 35;
        timer SetTenthsTimer(35);
        timer thread DeleteTimer(35);
    }
}

DeleteTimer(time)
{
    wait time;
    self Destroy();
}

WaitDamage()
{
    for(;;)
    {
        self waittill("damage", damage, attacker, direction, point, type, tagname, modelname, partname, weapon);
        if(IsDefined(weapon) && weapon.name == "beacon")
        {
            IPrintLnBold("Hole Damaged - Success: " + (GetTime() - level.last_fire_link_time <= 35000));
            IPrintLnBold("Time Remaining: " + Int(35000 - (GetTime() - level.last_fire_link_time)));
        }
    }
}