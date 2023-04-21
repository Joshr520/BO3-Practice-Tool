InitValveSolutions()
{
    if(level.script != "zm_stalingrad") return;
    level.tube_solutions = [];
    level.tube_solutions["armory"] = SpawnStruct(); level.tube_solutions["armory"].green = []; level.tube_solutions["barracks"] = SpawnStruct(); level.tube_solutions["barracks"].green = [];
    level.tube_solutions["command"] = SpawnStruct(); level.tube_solutions["command"].green = []; level.tube_solutions["factory"] = SpawnStruct(); level.tube_solutions["factory"].green = [];
    level.tube_solutions["library"] = SpawnStruct(); level.tube_solutions["library"].green = []; level.tube_solutions["store"] = SpawnStruct(); level.tube_solutions["store"].green = [];
    level.tube_solutions["armory"].green["store"] = Array(1, 0, -1, 2, 2, 2); level.tube_solutions["armory"].green["command"] = Array(0, 0, -1, 2, 0, 0); level.tube_solutions["armory"].green["library"] = Array(1, 1, -1, 0, 1, 2);
    level.tube_solutions["armory"].green["barracks"] = Array(2, 0, -1, 2, 0, 2); level.tube_solutions["armory"].green["factory"] = Array(2, 0, -1, 1, 0, 0);
    level.tube_solutions["barracks"].green["store"] = Array(0, 2, 1, 0, -1, 1); level.tube_solutions["barracks"].green["command"] = Array(1, 0, 2, 2, -1, 2); level.tube_solutions["barracks"].green["armory"] = Array(1, 1, 1, 0, -1, 1);
    level.tube_solutions["barracks"].green["library"] = Array(0, 1, 1, 0, -1, 0); level.tube_solutions["barracks"].green["factory"] = Array(2, 2, 2, 1, -1, 1);
    level.tube_solutions["command"].green["store"] = Array(0, -1, 0, 2, 2, 0); level.tube_solutions["command"].green["armory"] = Array(2, -1, 0, 2, 0, 0); level.tube_solutions["command"].green["library"] = Array(2, -1, 1, 1, 0, 0);
    level.tube_solutions["command"].green["barracks"] = Array(0, -1, 1, 0, 0, 1); level.tube_solutions["command"].green["factory"] = Array(0, -1, 0, 0, 0, 0);
    level.tube_solutions["factory"].green["store"] = Array(0, 2, 0, 0, 1, -1); level.tube_solutions["factory"].green["command"] = Array(1, 0, 2, 1, 1, -1); level.tube_solutions["factory"].green["armory"] = Array(1, 1, 0, 0, 1, -1);
    level.tube_solutions["factory"].green["library"] = Array(0, 2, 1, 0, 0, -1); level.tube_solutions["factory"].green["barracks"] = Array(2, 0, 1, 1, 0, -1);
    level.tube_solutions["library"].green["store"] = Array(0 ,0, 1, -1, 2, 0); level.tube_solutions["library"].green["command"] = Array(1, 1, 0, -1, 1, 2); level.tube_solutions["library"].green["armory"] = Array(2, 0, 1, -1, 0, 0);
    level.tube_solutions["library"].green["barracks"] = Array(2, 0, 2, -1, 1, 2); level.tube_solutions["library"].green["factory"] = Array(0, 1, 0, -1, 2, 0);
    level.tube_solutions["store"].green["command"] = Array(-1, 0, 1, 1, 0, 0); level.tube_solutions["store"].green["armory"] = Array(-1, 1, 0, 2, 2, 0); level.tube_solutions["store"].green["library"] = Array(-1, 2, 2, 0, 1, 2);
    level.tube_solutions["store"].green["barracks"] = Array(-1, 1, 0, 0, 1, 2); level.tube_solutions["store"].green["factory"] = Array(-1, 0, 2, 1, 2, 0);

}

CompleteTubePuzzle()
{
    level flag::set("generator_charged");
	level flag::set("generator_on");
    wait 1;
    foreach(tube in level.var_57f8b6c5)
    {
        if(tube == level.var_57f8b6c5[5]) continue;
        trig = struct::get("ee_tube_use_" + tube.script_label, "targetname");
        rotate = GetEnt(tube.target, "targetname");
        switch(tube.script_label)
        {
            case "armory":
                thread RotateTube(trig, tube, rotate, 2);
				break;
			case "barracks":
                thread RotateTube(trig, tube, rotate, 4);
				break;
			case "command":
                thread RotateTube(trig, tube, rotate, 1);
				break;
			case "factory":
                thread RotateTube(trig, tube, rotate, 5);
				break;
			case "library":
                thread RotateTube(trig, tube, rotate, 3);
				break;
			case "store":
                thread RotateTube(trig, tube, rotate, 0);
				break;
        }
    }
    level flag::wait_till("tube_puzzle_complete");
    wait 0.25;
    cylinder = struct::get("ee_tube_use_" + level.var_57f8b6c5[5].script_label, "targetname");
    level scene::skip_scene("p7_fxanim_zm_stal_pneumatic_tube_drop_bundle", 0, 0, 0);
    wait 0.5;
    cylinder notify("trigger_activated", self);
}

RotateTube(trig, tube, rotate, index)
{
    while(tube.var_1f3c0ca7 != level.tube_solutions[level.var_57f8b6c5[5].script_label].green[level.var_57f8b6c5[0].script_label][index])
    {
        trig notify("trigger_activated", self);
        rotate waittill("rotatedone");
    }
}

CompletePassword()
{
    level flag::wait_till("ee_cylinder_acquired");
    wait 1;
    sophia = struct::get("ee_sophia_struct", "targetname");
    sophia notify("trigger_activated", self);
    wait 2;
    foreach(letter in level.var_4c56821d)
	{
        self thread RotateLetter(letter);
    }
    while(!PasswordSolved()) wait 0.05;
    sophia notify("trigger_activated", self);
}

RotateLetter(letter)
{
    while(letter.var_c957db9f != letter.var_92f9e88c)
    {
        letter notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "MOD_PROJECTILE", "", "", "", GetWeapon("ar_standard"));
        letter waittill("rotatedone");
    }
}

PasswordSolved()
{
	foreach(letter in level.var_4c56821d)
	{
		if(letter.var_c957db9f != letter.var_92f9e88c)
		{
			return false;
		}
	}
	return true;
}

PickupKeys()
{
    self thread PickupGersh();
    self thread PickupBomb();
    self thread PickupGroph();
    self thread PickupDrone();
    self thread Pickup935();
    self thread PickupMangler();
}

PickupGersh()
{
    trophy = GetEnt("ee_keys_anomaly_damage_trig", "targetname");
    trophy notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "MOD_PROJECTILE", "", "", "", GetWeapon("ar_standard"));
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_stal_pickups_figure_blob_bundle", 0, 0, 0);
    wait 1;
    pickup = GetEnt("pickup_blob", "targetname");
    pickup notify("trigger_activated", self);
}

PickupBomb()
{
    trophy = GetEnt("ee_keys_puddle_damage_trig", "targetname");
    trophy notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "MOD_PROJECTILE", "", "", "", GetWeapon("launcher_dragon_fire"));
    wait 3.1;
    level scene::skip_scene("p7_fxanim_zm_stal_pickups_figure_nuke_bundle", 0, 0, 0);
    wait 1;
    pickup = GetEnt("pickup_nuke", "targetname");
    pickup notify("trigger_activated", self);
}

PickupGroph()
{
    self.var_ae0fff53 = level.var_ae0fff53;
    trophy = GetEnt("ee_keys_safe_damage_trig", "targetname");
    pickup = struct::get("ee_keys_pod_struct", "targetname").origin + VectorScale((0, 1, 0), 20);
    trophy notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "MOD_MELEE", "", "", "", level.var_ae0fff53);
    wait 1;
    foreach(stub in level._unitriggers.trigger_stubs)
	{
        if(pickup == stub.origin)
        {
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
}

PickupDrone()
{
    level notify(#"hash_a8bfa21a", GetWeapon("dragonshield"));
    wait 0.25;
    level scene::skip_scene("p7_fxanim_zm_stal_pickups_figure_drone_bundle", 0, 0, 0);
    wait 1;
    pickup = GetEnt("pickup_drone", "targetname");
    pickup notify("trigger_activated", self);
}

Pickup935()
{
    trophy = GetEnt("ee_sewer_damage_trig", "targetname");
    pickup = struct::get("ee_keys_935_struct", "targetname").origin;
    trophy notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "MOD_PROJECTILE", "", "", "", GetWeapon("launcher_dragon_fire"));
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_stal_sewer_switch_bundle", 0, 0, 0);
    wait 1;
    foreach(stub in level._unitriggers.trigger_stubs)
	{
        if(pickup == stub.origin)
        {
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
}

PickupMangler()
{
    level notify(#"hash_278aa663", "fxexp_200");
    wait 1.5;
    pickup = struct::get("ee_keys_raz_struct", "targetname").origin;
    foreach(stub in level._unitriggers.trigger_stubs)
	{
        if(pickup == stub.origin)
        {
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
}

CompleteChallenges()
{
    level flag::wait_till("key_placement");
    while(level.var_4e272444 < 6) wait 0.05;
    wait 1;
    challenges = struct::get("ee_map_button_struct", "targetname");
    challenges notify("trigger_activated", self);
    self SetOrigin(challenges.origin);
    while(!IsDefined(level.var_f0d11538 )) wait 0.05;
    wait 1;
    self thread WatchChallenge("groph");
    self thread WatchChallenge("valk");
    self thread WatchChallenge("mangler");
    self thread WatchChallenge("bombs");
    self thread WatchChallenge("gersh");
    for(i = 0; i < 6; i++)
    {
        challenges notify("trigger_activated", self);
        if(i == 5)
        {
            wait 5;
            shelf = GetEnt("ee_map_shelf", "targetname");
            pickup = shelf GetTagOrigin("wall_map_shelf_door") + (-5, 0, -2);
            foreach(stub in level._unitriggers.trigger_stubs)
            {
                if(pickup == stub.origin)
                {
                    self BuildAndActivateTrigger(stub);
                    break;
                }
            }
            download = struct::get("ee_koth_terminal_use", "targetname");
            while(!IsDefined(download.s_unitrigger)) wait 0.05;
            self BuildAndActivateTrigger(download.s_unitrigger);
            wait 6;
            level.var_cd867f7a = 179;
            level flag::wait_till("ee_lockdown_complete");
            while(!IsDefined(download.s_unitrigger)) wait 0.05;
            self BuildAndActivateTrigger(download.s_unitrigger);
            wait 0.5;
            sophia = struct::get("ee_sophia_struct", "targetname");
            org = self.origin;
            self SetOrigin(sophia.origin);
            wait 2;
            sophia notify("trigger_activated", self);
            self SetOrigin(org);
            return;
        }
        level waittill("start_challenge", type);
        switch(type)
        {
            case "groph":
                self thread CompleteGrophChallenge();
                break;
            case "valk":
                self thread CompleteValkChallenge();
                break;
            case "mangler":
                self thread CompleteManglerChallenge();
                break;
            case "bombs":
                self thread CompleteBombsChallenge();
                break;
            case "gersh":
                self thread CompleteGershChallenge();
                break;
        }
        level flag::wait_till("scenario_active");
        level flag::wait_till_clear("scenario_active");
    }
}

WatchChallenge(type)
{
    wait 1;
    switch(type)
    {
        case "groph":
            while(!level.var_bd16d335) wait 0.05;
            break;
        case "valk":
            while(!level.var_83edfb9) wait 0.05;
            break;
        case "mangler":
            while(!level.var_68a92652) wait 0.05;
            break;
        case "bombs":
            while(!level.var_8acfb18e) wait 0.05;
            break;
        case "gersh":
            while(!level.var_9777b703) wait 0.05;
            break;
    }
    level notify("start_challenge", type);
}

CompleteGrophChallenge()
{
    level flag::wait_till("drop_pod_spawned");
    wait 1;
    level.var_3947d49c = 1;
    zombie = zombie_utility::spawn_zombie(level.zombie_spawners[0]);
    wait 0.5;
    zombie ForceTeleport(level.var_8cc024f2.e_goal_volume.origin);
    zombie DoDamage(zombie.health + 666, self.origin, self);
    level flag::wait_till_clear("ee_round");
    self SpawnAndSendWhelp();
    while(!IsDefined(level.var_79fa326a.s_unitrigger)) wait 0.05;
    wait 1;
    self BuildAndActivateTrigger(level.var_79fa326a.s_unitrigger);
    sophia = struct::get("ee_sophia_struct", "targetname");
    org = self.origin;
    self SetOrigin(sophia.origin);
    wait 2;
    sophia notify("trigger_activated", self);
    self SetOrigin(org);
}

SpawnAndSendWhelp()
{
    switch(level.var_8cc024f2.script_string)
    {
        case "ee_command":
		{
			origin = (-175.606, 6205.36, 376.125);
			break;
		}
		case "ee_factory":
		{
			origin = (-2979.36, 2854.55, 568.125);
			break;
		}
		case "ee_library":
		{
			origin = (3145.36, 2856.06, 72.125);
			break;
		}
    }
    org = self.origin;
    self.var_d15b9a33 = "spawner_bo3_dragon_whelp";
    whelp = SpawnVehicle(self.var_d15b9a33, origin, (0, 0, 0));
    self LinkTo(whelp);
    self.var_4bd1ce6b = whelp;
    whelp ai::set_ignoreme(1);
    whelp SetIgnorePauseWorld(1);
    whelp.owner = self;
    wait 0.1;
    level notify(#"hash_fbd59317", self);
    self.var_4bd1ce6b waittill("cargo_dropped");
    self Unlink();
    self SetOrigin(org);
    whelp.dragon_recall_death = 1;
    whelp.var_a0e2dfff = 1;
    whelp Kill();
}

CompleteManglerChallenge()
{
    manglers = GetEntArray("zombie_raz", "targetname");
    for(;;)
    {
        manglers = GetEntArray("zombie_raz", "targetname");
        foreach(mangler in manglers)
        {
            if(IsDefined(mangler.ignore_nuke))
            {
                goal = GetEnt("ee_raz_capture", "targetname");
                mangler waittill("completed_emerging_into_playable_area");
                mangler ForceTeleport(goal.origin);
                return;
            }
        }
        wait 0.05;
    }
}

CompleteValkChallenge()
{
    sentinels = GetEntArray("zombie_sentinel", "targetname");
    for(;;)
    {
        sentinels = GetEntArray("zombie_sentinel", "targetname");
        foreach(sentinel in sentinels)
        {
            if(IsDefined(sentinel.ignore_nuke))
            {
                org = self.origin;
                self SetOrigin(level.zones["start_A_zone"].volumes[0].origin);
                wait 3;
                self LinkTo(sentinel);
                sentinel MonitorSpeed();
                self Unlink();
                self SetOrigin(org);
                return;
            }
        }
        wait 0.05;
    }
}

MonitorSpeed()
{
    level endon("ee_escort_complete");
    for(;;)
    {
        self SetSpeed(100, 100, 100);
        wait 0.05;
    }
}

CompleteBombsChallenge()
{
    bombs = GetEntArray("ee_timed", "script_label");
    while(!IsDefined(bombs[0].s_unitrigger)) wait 0.05;
    wait 1;
    foreach(bomb in bombs)
    {
        bomb PlaySound("zmb_rigged_button_press_good");
	    bomb StopLoopSound(2);
	    bomb scene::play("p7_fxanim_zm_stal_rigged_button_retract_bundle", Array(bomb));
    }
    level.var_e5f51155 = 1;
    level notify("ee_timed_complete");
    level.var_f0d11538 StopLoopSound(0.1);
}

CompleteGershChallenge()
{
    while(!IsDefined(level.var_a5fb1d00.var_44b9cab5)) wait 0.05;
    wait 1;
    for(i = 0; i < 3; i++)
    {
        level.var_a5fb1d00 notify("damage", 99999, self, (0, 0, 0), (0, 0, 0), "MOD_PROJECTILE", "", "", "", GetWeapon("ar_standard_upgraded"));
        level waittill(#"hash_f9b2d970");
        wait 0.1;
        level notify(#"hash_f9b2d970");
        wait 0.1;
    }
    level.var_a5fb1d00 waittill("movedone");
    org = self.origin;
    self SetOrigin(level.var_a5fb1d00.origin);
    wait 1;
    self SetOrigin(org);
}

DeliverPowerCore()
{
    level flag::wait_till("scenarios_complete");
    wait 3;
    pickup = level.var_a090a655 GetTagOrigin("tag_weapon_cores") + (0, 3, 2);
    foreach(stub in level._unitriggers.trigger_stubs)
	{
        if(pickup == stub.origin)
        {
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
    wait 0.5;
    origin = GetEnt("ee_weapon_cores_volume", "targetname").origin;
    org = self.origin;
    self.var_d15b9a33 = "spawner_bo3_dragon_whelp";
    whelp = SpawnVehicle(self.var_d15b9a33, origin, (0, 0, 0));
    self LinkTo(whelp);
    self.var_4bd1ce6b = whelp;
    whelp ai::set_ignoreme(1);
    whelp SetIgnorePauseWorld(1);
    whelp.owner = self;
    wait 0.1;
    level notify(#"hash_fbd59317", self);
    level flag::wait_till("weapon_cores_delivered");
    self Unlink();
    self SetOrigin(org);
    whelp.dragon_recall_death = 1;
    whelp.var_a0e2dfff = 1;
    whelp Kill();
}

StartNikolaiFight()
{
    level flag::set("nikolai_start");
}