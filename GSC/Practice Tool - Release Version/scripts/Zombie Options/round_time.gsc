RoundTime()
{
    self thread GodMode(1);
    level.custom_start_time = 0;
    level.last_spawn = 0;
    level.spawn_times = Array();
    level.spawned = 0;
    level.corpse_delay = 0;
    thread MonitorStartTimes();
    thread MonitorRoundEnd();
    spawner::add_archetype_spawn_function("zombie", ::ZombieRoundTimeMeasure);

    level.fn_custom_zombie_spawner_selection = undefined;
    level.next_wasp_round = 256;
    level.n_next_raps_round = 256;
    level.next_dog_round = 256;
    level.n_next_spider_round = 256;
    level.n_next_sentinel_round = 256;
    level.n_next_chaos_round = 256;

    level.var_ba0d6d40 = 256; // genesis boss

	while(level.round_number != 255)
    {
        level waittill("start_of_round");
    }
    level waittill("end_of_round");
    level flag::clear("spawn_zombies");
    compiler::nprintln("End Data");
}

MonitorStartTimes()
{
    level flag::wait_till("start_zombie_round_logic");
    wait 2.15;
    level.custom_start_time = GetTime();
    for(;;)
    {
        level waittill("end_of_round");
        level.custom_start_time = GetTime();
        level.last_spawn = 0;
        level.corpse_delay = 0;
        level.spawn_times = Array();
    }
}

MonitorRoundEnd()
{
    for(;;)
    {
        level waittill("start_of_round");
        while(level.zombie_total > 0) wait 0.05;
        compiler::nprintln("Round: " + level.round_number + " Cleared Corpses " + level.corpse_delay + " Times");
        time = GetTime();
        wait 0.1;
        times = CalcAverageSpawnTime();
        //compiler::nprintln("Round " + level.round_number + " Length: " + CalcTime(time - level.custom_start_time));
        //compiler::nprintln("Round " + level.round_number + " Average Spawn Time Per Zombie: " + CalcTime(times.average));
        //compiler::nprintln("Round " + level.round_number + " Min Spawn Time Per Zombie: " + CalcTime(times.min));
        //compiler::nprintln("Round " + level.round_number + " Max Spawn Time Per Zombie: " + CalcTime(times.max));
    }
}

ZombieRoundTimeMeasure()
{
    level.spawned++;
    if(!level.last_spawn) level.last_spawn = GetTime();
    else if(level.last_spawn != GetTime())
    {
        if(!IsDefined(level.spawn_times[GetTime() - level.last_spawn])) level.spawn_times[GetTime() - level.last_spawn] = 0;
        level.spawn_times[GetTime() - level.last_spawn]++;
        //IPrintLnBold(GetTime() - level.last_spawn);
        //IPrintLnBold(level.spawn_times[GetTime() - level.last_spawn]);
        level.last_spawn = GetTime();
    }
    wait 2.25;
    self Kill();
}

CalcAverageSpawnTime()
{
    min = 9999;
    max = 0;
    average = 0;
    count = 0;
    foreach(key in GetArrayKeys(level.spawn_times))
    {
        if(key < min) min = key;
        if(key > max) max = key;
        if(level.spawn_times[key] > count)
        {
            count = level.spawn_times[key];
            average = key;
        }
    }
    result = SpawnStruct();
    result.min = min;
    result.max = max;
    result.average = average;
    return result;
}

CalcTime(ms)
{
    min = 0;
    sec = 0;
    if(ms >= 60000)
    {
        min = Int(ms / 60000);
        ms -= min * 60000;
    }
    if(ms >= 1000)
    {
        sec = Int(ms / 1000);
        ms -= sec * 1000;
    }
    time = "";
    if(min > 0)
    {
        time += min + ":";
    }
    if(min || sec > 0)
    {
        if(sec < 10 && min) time += "0";
        time += sec + ".";
    }
    if(min || sec || ms > 0)
    {
        if(!min && !sec) time += "0.";
        if(ms <= 50) time += "0";
        ms /= 10;
        time += ms;
    }
    return time;
}

DiagnosticSpawn()
{
    msg = "Zombie Spawned At " + Int(GetTime() - level.n_gameplay_start_time);
    if(IsDefined(self.archetype))
    {
        if(!IsDefined(level.flag_printout[self.archetype]))
        {
            level.flag_printout[self.archetype] = [];
            level.flag_printout[self.archetype]["spawned"] = 0;
        }
        level.flag_printout[self.archetype]["spawned"]++;
        msg += " - Archetype: " + self.archetype;
    }
    compiler::nprintln(msg);

    self waittill("death", attacker);
    msg = "Zombie Killed At " + Int(GetTime() - level.n_gameplay_start_time);
    if(IsDefined(self.archetype))
    {
        if(!IsDefined(level.flag_printout[self.archetype]["player"])) level.flag_printout[self.archetype]["player"] = 0;
        if(!IsDefined(level.flag_printout[self.archetype]["self"])) level.flag_printout[self.archetype]["self"] = 0;
        msg += " - Archetype: " + self.archetype;
    }
    if(IsPlayer(self.attacker) || IsPlayer(attacker))
    {
        if(IsDefined(level.flag_printout[self.archetype]["player"])) level.flag_printout[self.archetype]["player"]++;
        msg += " - Killed By Player";
    }
    else
    {
        if(IsDefined(level.flag_printout[self.archetype]["self"])) level.flag_printout[self.archetype]["self"]++;
        msg += " - Self Death";
    }
    compiler::nprintln(msg);
}

/*detour zm<scripts\zm\_zm.gsc>::round_spawning()
{
	level endon("intermission");
	level endon("end_of_round");
	level endon("restart_round");
	if(level.intermission) return;
	if(level.zm_loc_types["zombie_location"].size < 1) return;
	zombie_utility::ai_calculate_health(level.round_number);
	count = 0;
	players = getplayers();
	for(i = 0; i < players.size; i++)
	{
		players[i].zombification_time = 0;
	}
	if(!(IsDefined(level.kill_counter_hud) && level.zombie_total > 0))
	{
		level.zombie_total = zm::get_zombie_count_for_round(level.round_number, level.players.size);
		level.zombie_respawns = 0;
		level notify("zombie_total_set");
	}
	if(IsDefined(level.zombie_total_set_func))
	{
		level thread [[level.zombie_total_set_func]]();
	}
	if(level.round_number < 10 || level.speed_change_max > 0)
	{
		level thread zombie_utility::zombie_speed_up();
	}
	old_spawn = undefined;
	while(true)
	{
		while(zombie_utility::get_current_zombie_count() >= level.zombie_ai_limit || level.zombie_total <= 0)
		{
			wait(0.1);
		}
		while(zombie_utility::get_current_actor_count() >= level.zombie_actor_limit)
		{
            //compiler::nprintln("Round: " + level.round_number + " Cleared Corpses - Zombie #" + level.spawned);
            level.corpse_delay++;
			zombie_utility::clear_all_corpses();
			wait(0.1);
		}
		if(flag::exists("world_is_paused"))
		{
			level flag::wait_till_clear("world_is_paused");
		}
		level flag::wait_till("spawn_zombies");
		while(level.zm_loc_types["zombie_location"].size <= 0)
		{
			wait(0.1);
		}
		zm::run_custom_ai_spawn_checks();
		if(IsDefined(level.hostmigrationtimer) && level.hostmigrationtimer)
		{
			util::wait_network_frame();
			continue;
		}
		if(IsDefined(level.fn_custom_round_ai_spawn))
		{
			if([[level.fn_custom_round_ai_spawn]]())
			{
				util::wait_network_frame();
				continue;
			}
		}
		if(IsDefined(level.zombie_spawners))
		{
			if(IsDefined(level.fn_custom_zombie_spawner_selection))
			{
				spawner = [[level.fn_custom_zombie_spawner_selection]]();
			}
			else
			{
				if(IsTrue(level.use_multiple_spawns))
				{
					if(IsDefined(level.spawner_int) && (IsTrue(level.zombie_spawn[level.spawner_int].size)))
					{
						spawner = array::random(level.zombie_spawn[level.spawner_int]);
					}
					else
					{
						spawner = array::random(level.zombie_spawners);
					}
				}
				else
				{
					spawner = array::random(level.zombie_spawners);
				}
			}
			ai = zombie_utility::spawn_zombie(spawner, spawner.targetname);
		}
		if(IsDefined(ai))
		{
			level.zombie_total--;
			if(level.zombie_respawns > 0)
			{
				level.zombie_respawns--;
			}
			ai thread zombie_utility::round_spawn_failsafe();
			count++;
			if(ai ai::has_behavior_attribute("can_juke"))
			{
				ai ai::set_behavior_attribute("can_juke", 0);
			}
			if(level.zombie_respawns > 0)
			{
				wait(0.1);
			}
			else
			{
				wait(level.zombie_vars["zombie_spawn_delay"]);
			}
		}
		util::wait_network_frame();
	}
}*/