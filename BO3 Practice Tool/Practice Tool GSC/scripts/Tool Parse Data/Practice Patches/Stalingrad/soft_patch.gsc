StalingradSoftPatch()
{
    if (level.mapname != "zm_stalingrad") {
        DebugPrint("error", "Soft Patch attempted to load on " + level.mapDisplayName + " but it requires Gorod Krovi");
        return;
    }
    level flag::set("stalingrad_soft_patch");
    self flag::wait_till("bgb_weapon_cycling");
    if (!IsInArray(self.bgb_pack, "zm_bgb_reign_drops") && !IsInArray(self.bgb_pack, "zm_bgb_shopping_free")) {
        DebugPrint("error", "Soft Patch requires Reign Drops or Shopping Free to work");
        return;
    }

    WaitFadeIn();
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while(self.bgb_pack_randomized[0] != "zm_bgb_reign_drops" && self.bgb_pack_randomized[0] != "zm_bgb_shopping_free") {
        self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    }
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("GK Soft Patch Loaded");

    level.customrandomweaponweights = ::StalingradBoxOdds;
    thread IncreaseCylinderOdds();
    thread IncreaseDragonOdds();
    thread DeleteSupplyGroph(); 
}

StalingradBoxOdds(weapons)
{
    pap_triggers = zm_pap_util::get_triggers();
	if(weapons[0] === level.w_raygun_mark3) {
		level.var_12d3a848 = 0;
		return weapons;
	}
	if(zm_weapons::limited_weapon_below_quota(level.w_raygun_mark3)) {
		level.var_12d3a848++;
	}
	else {
		level.var_12d3a848 = 0;
	}
	if((RandomInt(100) <= 10 || level.var_12d3a848 >= 5) && zm_magicbox::treasure_chest_canplayerreceiveweapon(self, level.w_raygun_mark3, pap_triggers) && !self HasWeapon(level.w_raygun_mark3_upgraded)) {
		ArrayInsert(weapons, level.w_raygun_mark3, 0);
		level.var_12d3a848 = 0;
	}
	return weapons;
}

IncreaseCylinderOdds()
{
    level flag::wait_till("power_on");
    wait 0.05;

    zm_spawner::deregister_zombie_death_event_callback(@namespace_2e6e7fce<scripts\zm\zm_stalingrad_drop_pods.gsc>::function_1389d425);
    zm_spawner::register_zombie_death_event_callback(::CylinderOnDeathCallback);
}

CylinderOnDeathCallback(e_attacker)
{
	if(IsTrue(self.var_4d11bb60) || IsDefined(level.var_583e4a97.var_caa5bc3e) || level flag::get("drop_pod_spawned") || level flag::get("drop_pod_active") || IsTrue(self.no_powerups)) {
		return false;
	}
	self thread CylinderDrop();
}

CylinderDrop()
{
	drop_count = level.powerup_drop_count;
	org = self.origin;
	emerged = IsTrue(self.completed_emerging_into_playable_area);
    force_drop = !(level.zombie_total + zombie_utility::get_current_zombie_count());
	wait(0.5);
	if(drop_count != level.powerup_drop_count || level.var_583e4a97.var_a622ee25) {
		return false;
	}
	n_rate = level.var_583e4a97.var_a43689b5;
	n_roll = RandomInt(100);
	if(force_drop || n_roll <= n_rate && emerged) {
		cylinder_type = [[ @namespace_2e6e7fce<scripts\zm\zm_stalingrad_drop_pods.gsc>::function_a9d4f2ec ]]();
		s_powerup = zm_powerups::specific_powerup_drop("code_cylinder_" + cylinder_type, org);
		level.var_583e4a97.var_a622ee25 = 1;
		level.var_583e4a97.var_a43689b5 = 10;
		s_powerup thread [[ @namespace_2e6e7fce<scripts\zm\zm_stalingrad_drop_pods.gsc>::function_9411a0ff ]]();
	}
	else {
		level.var_583e4a97.var_a43689b5 = level.var_583e4a97.var_a43689b5 + 5;
	}
}

IncreaseDragonOdds()
{
    level flag::wait_till("dragon_egg_acquired");
    level.spit_odds = 1;
    level.var_de98a8ad = level.spit_odds;
    thread MonitorDragonRound();
    thread MonitorDragonDepartment();

    level flag::wait_till("egg_bathed_in_flame");
    level.spit_odds = 0;
    level.var_de98a8ad = level.spit_odds;

    level flag::wait_till("egg_awakened");
    for(;;) {
        level waittill("newzoneactive", zone);
        if(zone == "factory_arm_zone" || zone == "factory_A_zone" || zone == "factory_B_zone") {
            break;
        }
    }
    level.spit_odds = 1;
    level.var_de98a8ad = level.spit_odds;

    level flag::wait_till("gauntlet_step_2_complete");
    level.spit_odds = 0.35;
    level.var_de98a8ad = level.spit_odds;
}

MonitorDragonDepartment()
{
    level flag::wait_till("department_store_upper_open");
    wait 0.05;
    level.var_de98a8ad = level.spit_odds;
}

MonitorDragonRound()
{
    while(!level flag::get("gauntlet_step_2_complete")) {
		level waittill("between_round_over");
        wait 0.05;
        level.var_de98a8ad = level.spit_odds;
	}
}

DeleteSupplyGroph()
{
    foreach(pod in struct::get_array("ee_pod", "targetname")) {
        if(IsSubStr(pod.script_string, "library")) {
            pod struct::delete();
        }
    }
    foreach(volume in GetEntArray("ee_pod_score_volume", "targetname")) {
        if(IsSubStr(volume.script_string, "library")) {
            volume Delete();
        }
    }
    foreach(attackable in struct::get_array("ee_pod_attackable", "targetname")) {
        if(IsSubStr(attackable.script_string, "library")) {
            attackable struct::delete();
        }
    }
}

detour zm_stalingrad_pap<scripts\zm\zm_stalingrad_pap_quest.gsc>::function_809fbbff(var_db0ac3dc)
{
    if(!level flag::get("stalingrad_soft_patch")) {
        self [[ @zm_stalingrad_pap<scripts\zm\zm_stalingrad_pap_quest.gsc>::function_809fbbff ]](var_db0ac3dc);
        return;
    }

	if(level flag::get("drop_pod_active") || level flag::get("drop_pod_spawned")) {
		return;
	}
	level [[ @zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::function_3a92f7f ]]();
	if(level.var_583e4a97.var_365bcb3c == 0) {
		var_1196aeee = [[ @zm_stalingrad_pap<scripts\zm\zm_stalingrad_pap_quest.gsc>::function_23b93c79 ]](var_db0ac3dc);
	}
	else {
		var_1196aeee = [[ @zm_stalingrad_pap<scripts\zm\zm_stalingrad_pap_quest.gsc>::function_a0a37968 ]](var_db0ac3dc);
	}
	var_e7a36389 = array::random(var_1196aeee);
	level.var_8cc024f2 = var_e7a36389;
	level thread [[ @namespace_2e6e7fce<scripts\zm\zm_stalingrad_drop_pods.gsc>::function_d1a91c4f ]](var_e7a36389);
}