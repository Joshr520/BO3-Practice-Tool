SetZombieHealthOverride(round)
{
	level flag::set("zombie_health_override");
    level.zombie_health = GetHealthForRound(round);

	thread WriteToScreen("Overriding Zombie Health: Round: " + round);

	switch (level.mapname) {
		case "zm_zod": {
			level.wasp_health = Min(1600, round * 50);
			level.n_raps_health = Min(1600, round * 50);
			break;
		}
		case "zm_stalingrad": {
			// mangler
			level.razgunhealth_restore = level.razgunhealth;
			level.razhelmethealth_restore = level.razhelmethealth;
			level.razleftshoulderarmorhealth_restore = level.razleftshoulderarmorhealth;
			level.razchestarmorhealth_restore = level.razchestarmorhealth;
			level.razthigharmorhealth_restore = level.razthigharmorhealth;

			level.var_f95eaac8 = Min(15000, Max(5500, 5500 + (round * 100)));
			level.razmaxhealth = level.var_f95eaac8;
			level.razgunhealth = level.var_f95eaac8 * 0.15;
			level.razhelmethealth = level.var_f95eaac8 * 0.3;
			level.razleftshoulderarmorhealth = level.var_f95eaac8 * 0.25;
			level.razchestarmorhealth = level.var_f95eaac8 * 0.4;
			level.razthigharmorhealth = level.var_f95eaac8 * 0.25;

			// valkyrie
			level.var_d6d4b6f9 = Min(50000, Max(4500, 2500 + (round * 200)));
			break;
		}
		case "zm_genesis": {
			level.wasp_health = Max(1600, round * 50);
			break;
		}
		default: {
			break;
		}
	}

	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		switch (zombie.archetype) {
			case "zombie":
			case "apothicon_fury":
			case "keeper": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = level.zombie_health;
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "parasite": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = Int(level.wasp_health * (GetDvarString("scr_wasp_health_walk_multiplier") == "" ? 1 : GetDvarFloat("scr_wasp_health_walk_multiplier")));
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "raps": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = Int(level.n_raps_health * (GetDvarString("scr_raps_health_walk_multiplier") == "" ? 1 : GetDvarFloat("scr_raps_health_walk_multiplier")));
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "margwa": {
				max = zombie.headhealthmax;
				zombie.headhealthmax = (round * 100) + 100;
				foreach (head in zombie.head) {
					if (head.health > 0) {
						percent_health = head.health / max;
						head.health = Int(percent_health * zombie.headhealthmax);
					}
				}
				break;
			}
			case "raz": {
				percent_health = zombie.health / zombie.maxhealth;
				percent_max_gun = zombie.razgunhealth / level.razgunhealth_restore;
				percent_max_helmet = zombie.razhelmethealth / level.razhelmethealth_restore;
				percent_max_shoulder = zombie.razleftshoulderarmorhealth / level.razleftshoulderarmorhealth_restore;
				percent_max_chest = zombie.razchestarmorhealth / level.razchestarmorhealth_restore;
				percent_max_thigh_right = zombie.razrightthighhealth / level.razthigharmorhealth_restore;
				percent_max_thigh_left = zombie.razleftthighhealth / level.razthigharmorhealth_restore;
				
				zombie.maxhealth = level.razmaxhealth;
				zombie.health = Int(percent_health * level.razmaxhealth);
				zombie.razgunhealth = Int(percent_max_gun * level.razgunhealth);
				zombie.razhelmethealth = Int(percent_max_helmet * level.razhelmethealth);
				zombie.razleftshoulderarmorhealth = Int(percent_max_shoulder * level.razgunhealth);
				zombie.razchestarmorhealth = Int(percent_max_chest * level.razchestarmorhealth);
				zombie.razrightthighhealth = Int(percent_max_thigh_right * level.razrightthighhealth);
				zombie.razleftthighhealth = Int(percent_max_thigh_left * level.razleftthighhealth);
				break;
			}
			case "sentinel_drone": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = level.var_d6d4b6f9;
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "astronaut": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = (level.zombie_health * GetPlayers().size) * level.astro_zombie_health_mult;
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			default: {
				break;
			}
		}
	}
}

RestoreZombieHealthOverride()
{
	level flag::clear("zombie_health_override");
	level.zombie_health = GetHealthForRound(level.round_number);

	thread WriteToScreen("Restoring Zombie Health Override");

	switch (level.mapname) {
		case "zm_zod": {
			level.wasp_health = Min(1600, level.round_number * 50);
			level.n_raps_health = Min(1600, level.round_number * 50);
			break;
		}
		case "zm_stalingrad": {
			// mangler
			level.razgunhealth_restore = level.razgunhealth;
			level.razhelmethealth_restore = level.razhelmethealth;
			level.razleftshoulderarmorhealth_restore = level.razleftshoulderarmorhealth;
			level.razchestarmorhealth_restore = level.razchestarmorhealth;
			level.razthigharmorhealth_restore = level.razthigharmorhealth;

			level.var_f95eaac8 = Min(15000, Max(5500, 5500 + (level.round_number * 100)));
			level.razmaxhealth = level.var_f95eaac8;
			level.razgunhealth = level.var_f95eaac8 * 0.15;
			level.razhelmethealth = level.var_f95eaac8 * 0.3;
			level.razleftshoulderarmorhealth = level.var_f95eaac8 * 0.25;
			level.razchestarmorhealth = level.var_f95eaac8 * 0.4;
			level.razthigharmorhealth = level.var_f95eaac8 * 0.25;

			// valkyrie
			level.var_d6d4b6f9 = Min(50000, Max(4500, 2500 + (level.round_number * 200)));
			break;
		}
		case "zm_genesis": {
			level.wasp_health = Max(1600, level.round_number * 50);
			break;
		}
		default: {
			break;
		}
	}

	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		switch (zombie.archetype) {
			case "zombie":
			case "apothicon_fury":
			case "keeper": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = level.zombie_health;
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "parasite": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = Int(level.wasp_health * (GetDvarString("scr_wasp_health_walk_multiplier") == "" ? 1 : GetDvarFloat("scr_wasp_health_walk_multiplier")));
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "raps": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = Int(level.n_raps_health * (GetDvarString("scr_raps_health_walk_multiplier") == "" ? 1 : GetDvarFloat("scr_raps_health_walk_multiplier")));
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "margwa": {
				max = zombie.headhealthmax;
				zombie.headhealthmax = (level.round_number * 100) + 100;
				foreach (head in zombie.head) {
					if (head.health > 0) {
						percent_health = head.health / max;
						head.health = Int(percent_health * zombie.headhealthmax);
					}
				}
				break;
			}
			case "raz": {
				percent_health = zombie.health / zombie.maxhealth;
				percent_max_gun = zombie.razgunhealth / level.razgunhealth_restore;
				percent_max_helmet = zombie.razhelmethealth / level.razhelmethealth_restore;
				percent_max_shoulder = zombie.razleftshoulderarmorhealth / level.razleftshoulderarmorhealth_restore;
				percent_max_chest = zombie.razchestarmorhealth / level.razchestarmorhealth_restore;
				percent_max_thigh_right = zombie.razrightthighhealth / level.razthigharmorhealth_restore;
				percent_max_thigh_left = zombie.razleftthighhealth / level.razthigharmorhealth_restore;
				
				zombie.maxhealth = level.razmaxhealth;
				zombie.health = Int(percent_health * level.razmaxhealth);
				zombie.razgunhealth = Int(percent_max_gun * level.razgunhealth);
				zombie.razhelmethealth = Int(percent_max_helmet * level.razhelmethealth);
				zombie.razleftshoulderarmorhealth = Int(percent_max_shoulder * level.razgunhealth);
				zombie.razchestarmorhealth = Int(percent_max_chest * level.razchestarmorhealth);
				zombie.razrightthighhealth = Int(percent_max_thigh_right * level.razrightthighhealth);
				zombie.razleftthighhealth = Int(percent_max_thigh_left * level.razleftthighhealth);
				break;
			}
			case "sentinel_drone": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = level.var_d6d4b6f9;
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			case "astronaut": {
				percent_health = zombie.health / zombie.maxhealth;
				zombie.maxhealth = (level.zombie_health * GetPlayers().size) * level.astro_zombie_health_mult;
				zombie.health = Int(percent_health * zombie.maxhealth);
				break;
			}
			default: {
				break;
			}
		}
	}
}

GetHealthForRound(round)
{
    health = level.zombie_vars["zombie_health_start"];
    for (i = 2; i <= round; i++) {
		if (i >= 10) {
			old_health = health;
			health = health + (Int(health * level.zombie_vars["zombie_health_increase_multiplier"]));
			if (health < old_health) {
				health = old_health;
				return health;
			}
			continue;
		}
		health = Int(health + level.zombie_vars["zombie_health_increase"]);
	}
    return health;
}

SetZombieSpeedOverride(speed)
{
	thread WriteToScreen("Overriding Zombie Speed: " + speed);
	spawner::add_global_spawn_function("axis", ::SetZombieSpeedOnSpawn, speed);
	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		if (zombie.archetype == "zombie" && zombie.team == level.zombie_team) {
			zombie zombie_utility::set_zombie_run_cycle_override_value(speed);
		}
	}
}

ResetZombieSpeedOverride()
{
	thread WriteToScreen("Resetting Zombie Speed Override");
	spawner::remove_global_spawn_function("axis", ::SetZombieSpeedOnSpawn);
	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		if (zombie.archetype == "zombie" && zombie.team == level.zombie_team) {
			zombie zombie_utility::set_zombie_run_cycle_restore_from_override();
		}
	}
}

SetZombieSpeedOnSpawn(speed)
{
	if (IsAlive(self) && self.archetype == "zombie" && self.team == level.zombie_team) {
		self zombie_utility::set_zombie_run_cycle_override_value(speed);
	}
}

FreezeZombies()
{
	thread WriteToScreen("Freezing Zombies");
	spawner::add_global_spawn_function("axis", ::FreezeZombieOnSpawn);
	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		if (IsAlive(zombie) && !zombie IsPaused() && zombie.team == level.zombie_team && !zombie IsHidden()) {
			[[ @zm_bgb_fear_in_headlights<scripts\zm\bgbs\_zm_bgb_fear_in_headlights.gsc>::function_b8eb33c5 ]](zombie);
		}
	}
}

UnfreezeZombies()
{
	thread WriteToScreen("Unfreezing Zombies");
	spawner::remove_global_spawn_function("axis", ::FreezeZombieOnSpawn);
	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		[[ @zm_bgb_fear_in_headlights<scripts\zm\bgbs\_zm_bgb_fear_in_headlights.gsc>::function_31a2964e ]](zombie);
	}
}

FreezeZombieOnSpawn()
{
	if (IsAlive(self) && !self IsPaused() && self.team == level.zombie_team && !self IsHidden()) {
		[[ @zm_bgb_fear_in_headlights<scripts\zm\bgbs\_zm_bgb_fear_in_headlights.gsc>::function_b8eb33c5 ]](self);
	}
}

TPZombiesToPlayer()
{
	query = positionquery_source_navigation(self.origin, 100, 500, 200, 32, 16);
	if (!query.data.size) {
		thread WriteToScreen("Failed to find valid TP position");
		return;
	}
	spots = query.data;
	foreach (spot in spots) {
		if (zm_utility::check_point_in_enabled_zone(spot.origin, 0)) {
			tp_origin = spot.origin + VectorScale((0, 0, 1), 16);
			if (level flag::get("zombie_tp_override")) {
				spawner::add_global_spawn_function("axis", ::TPWhenReady, tp_origin);
			}
			break;
		}
	}
	if (!IsDefined(tp_origin)) {
		thread WriteToScreen("Failed to find valid TP position");
		return;
	}
	thread WriteToScreen("TPing Zombies To Player");
	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		zombie thread TPWhenReady(tp_origin);
	}
}

SetTPZombiesOverride()
{
	thread WriteToScreen("Freezing Zombie TP Override");
	level flag::set("zombie_tp_override");
}

ResetTPZombiesOverride()
{
	thread WriteToScreen("Resetting Zombie TP Override");
	level flag::clear("zombie_tp_override");
	spawner::remove_global_spawn_function("axis", ::TPWhenReady);
	foreach (zombie in GetAITeamArray(level.zombie_team)) {
		zombie PushActors(true);
		[[ @zm_bgb_fear_in_headlights<scripts\zm\bgbs\_zm_bgb_fear_in_headlights.gsc>::function_31a2964e ]](zombie);
	}
}

TPWhenReady(tp_origin)
{
	self notify("zombie_tp_request");
	self endon("zombie_tp_request");
	switch (self.archetype) {
		case "zombie":
		case "sentinel_drone": {
			self waittill("completed_emerging_into_playable_area");
		}
		case "wasp":
		case "parasite":
		case "zombie_dog":
		case "spider": {
			self waittill("visible");
			break;
		}
		case "margwa": {
			while (!IsFalse(self.isfrozen)) {
				wait 0.05;
			}
			break;
		}
		case "mechz": {
			while (!IsTrue(self.b_flyin_done)) {
				wait 0.05;
			}
			break;
		}
		default: {
			while (!zm_utility::check_point_in_playable_area(self.origin)) {
				wait 0.05;
			}
			break;
		}
	}
	self ForceTeleport(tp_origin);
	self.origin = tp_origin;
	wait 0.05;
	if (level flag::get("zombie_tp_override")) {
		self PushActors(false);
		[[ @zm_bgb_fear_in_headlights<scripts\zm\bgbs\_zm_bgb_fear_in_headlights.gsc>::function_b8eb33c5 ]](self);
	}
}

detour zombie_utility<scripts\shared\ai\zombie_utility.gsc>::ai_calculate_health(round_number)
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zombie_utility<scripts\shared\ai\zombie_utility.gsc>::ai_calculate_health ]](round_number);
}

detour margwaserverutils<scripts\shared\ai\margwa.gsc>::margwasetheadhealth(health)
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @margwaserverutils<scripts\shared\ai\margwa.gsc>::margwasetheadhealth ]](health);
}

detour zm_ai_raz<scripts\zm\_zm_ai_raz.gsc>::function_a67ada8()
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zm_ai_raz<scripts\zm\_zm_ai_raz.gsc>::function_a67ada8 ]]();
}

detour zm_ai_sentinel_drone<scripts\zm\_zm_ai_sentinel_drone.gsc>::function_e930da45()
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zm_ai_sentinel_drone<scripts\zm\_zm_ai_sentinel_drone.gsc>::function_e930da45 ]]();
}

detour zm_ai_wasp<scripts\zm\_zm_ai_wasp.gsc>::wasp_health_increase()
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zm_ai_wasp<scripts\zm\_zm_ai_wasp.gsc>::wasp_health_increase ]]();
}

detour zm_ai_wasp<scripts\zm\_zm_ai_wasp_no_idgun.gsc>::wasp_health_increase()
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zm_ai_wasp<scripts\zm\_zm_ai_wasp_no_idgun.gsc>::wasp_health_increase ]]();
}

detour zm_genesis_wasp<scripts\zm\zm_genesis_wasp.gsc>::wasp_health_increase()
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zm_genesis_wasp<scripts\zm\zm_genesis_wasp.gsc>::wasp_health_increase ]]();
}

detour zm_ai_raps<scripts\zm\_zm_ai_raps.gsc>::raps_health_increase()
{
	if (!IsTrue(level.num_splits) && level flag::get("zombie_health_override")) {
        return;
    }
	self [[ @zm_ai_raps<scripts\zm\_zm_ai_raps.gsc>::raps_health_increase ]]();
}

detour zm_ai_wasp<scripts\zm\_zm_ai_wasp.gsc>::wasp_cleanup_failsafe()
{
	if (IsTrue(level.num_splits)) {
        self [[ @zm_ai_wasp<scripts\zm\_zm_ai_wasp.gsc>::wasp_cleanup_failsafe ]]();
		return;
    }

	self endon("death");
	n_wasp_created_time = GetTime();
	n_check_time = n_wasp_created_time;
	v_check_position = self.origin;
	for (;;) {
		n_current_time = GetTime();
		if (level flag::get("zombie_tp_override")) {
			n_check_time = n_current_time;
			wait(1);
			continue;
		}
		if(IsTrue(level.bzm_worldpaused)) {
			n_check_time = n_current_time;
			wait(1);
			continue;
		}
		n_dist = Distance(v_check_position, self.origin);
		if(n_dist > 100) {
			n_check_time = n_current_time;
			v_check_position = self.origin;
		}
		else {
			n_delta_time = (n_current_time - n_check_time) / 1000;
			if(n_delta_time >= 20) {
				break;
			}
		}
		n_delta_time = (n_current_time - n_wasp_created_time) / 1000;
		if(n_delta_time >= 150) {
			break;
		}
		wait(1);
	}
	self DoDamage(self.health + 100, self.origin);
}