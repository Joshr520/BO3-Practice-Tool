SetZombieSpeed(speed)
{
    if(!IsDefined(level.custom_speed)) level.custom_speed = "walk";
    switch(speed)
    {
        case 0:
            level.custom_speed = "walk";
            break;
        case 1:
            level.custom_speed = "run";
            break;
        case 2:
            level.custom_speed = "sprint";
            break;
    }
    array::thread_all(level.zombie_spawners, spawner::remove_spawn_function, ::SpeedOverride);
    text = ToUpper(GetSubStr(level.custom_speed, 0, 1)) + GetSubStr(level.custom_speed, 1);
    if(speed < 3) thread WriteToScreen("Setting Zombie Speed " + text);
    else
    {
        thread WriteToScreen("Removing Zombie Speed Override");
        foreach(zombie in zombie_utility::get_zombie_array()) zombie zombie_utility::set_zombie_run_cycle_restore_from_override();
        return;
    }
    array::thread_all(level.zombie_spawners, spawner::add_spawn_function, ::SpeedOverride);
    foreach(zombie in zombie_utility::get_zombie_array()) zombie zombie_utility::set_zombie_run_cycle_restore_from_override();
    foreach(zombie in zombie_utility::get_zombie_array()) zombie zombie_utility::set_zombie_run_cycle_override_value(level.custom_speed);
}

SpeedOverride()
{
    self zombie_utility::set_zombie_run_cycle_override_value(level.custom_speed);
}

ClearSpawns(enabled)
{
    if(!enabled)
    {
        level notify("clear_spawn_disable");
        thread WriteToScreen("Enabling Zombie Spawns");
        level flag::set("spawn_zombies");
        return;
    }
    level endon("clear_spawn_disable");
    thread WriteToScreen("Disabling Zombie Spawns");
    for(;;)
    {
        level flag::clear("spawn_zombies");
        level flag::wait_till("spawn_zombies");
    }
}

IgnorePlayers(enabled)
{
    level notify("ignore_request");
    level endon("ignore_request");
    text = "Zombies No Longer Ignoring Players";
    if(enabled) text = "Zombies Now Ignoring Players";
    thread WriteToScreen(text);
    for(;;)
    {
        foreach(player in GetPlayers())
        {
            if(enabled) player.ignoreme = 1;
            else player.ignoreme = 0;
        }
        wait 0.25;
    }
}

KillHorde()
{
    thread WriteToScreen("Killing Zombies On Map");
    foreach(zombie in GetAITeamArray(level.zombie_team))
    {
        zombie Kill();
    }
}

FreezeZombies(enabled)
{
    level notify("new_freeze_request");
    level endon("new_freeze_request");
    level endon("freeze_disable");
    text = "Unfreezing Zombies";
    if(enabled) text = "Freezing Zombies";
    thread WriteToScreen(text);
    for(;;)
    {
        zombies = GetAITeamArray(level.zombie_team);
        foreach(zombie in zombies)
        {
            if(enabled && IsAlive(zombie) && !zombie IsPaused() && zombie.team == level.zombie_team && !zombie IsHidden()) Freeze(zombie);
            else if(!enabled) Unfreeze(zombie);

            if(IsTrue(zombie.aat_turned) && zombie IsPaused()) Unfreeze(zombie);
        }
        if(!enabled) level notify("freeze_disable");
        wait 0.05;
    }
}

Freeze(ai)
{
	ai notify(#"hash_4e7f43fc");
	ai thread WatchDeath();
	ai SetEntityPaused(1);
	ai.var_70a58794 = ai.b_ignore_cleanup;
	ai.b_ignore_cleanup = 1;
	ai.var_7f7a0b19 = ai.is_inert;
	ai.is_inert = 1;
}

Unfreeze(ai)
{
	ai notify(#"hash_4e7f43fc");
	ai SetEntityPaused(0);
	if(IsDefined(ai.var_7f7a0b19))
	{
		ai.is_inert = ai.var_7f7a0b19;
	}
	if(IsDefined(ai.var_70a58794))
	{
		ai.b_ignore_cleanup = ai.var_70a58794;
	}
	else
	{
		ai.b_ignore_cleanup = 0;
	}
}

WatchDeath()
{
	self endon(#"hash_4e7f43fc");
	self waittill("death");
	if(IsDefined(self) && self IsPaused())
	{
		self SetEntityPaused(0);
		if(!self IsRagDoll())
		{
			self StartRagDoll();
		}
	}
}