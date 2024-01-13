BowShotsPractice()
{
    if (level.mapname != "zm_castle") {
        DebugPrint("error", "Bow Shot Practice attempted to load on " + level.mapDisplayName + " but it requires Der Eisendrache");
        return;
    }
    RegisterHotkey(0x74, "Toggle Beacon FX: ^9F5", undefined, ::ToggleBeaconFX);
    RegisterHotkey(0x75, "Toggle Dog Fog: ^9F6", undefined, ::ToggleDogFog);
    RegisterHotKey(0x76, "Toggle Bow Camera: ^9F7\n^3Camera: ", "Active", ::ToggleProjectileCamera, ::ProjectileActiveText);
    RegisterHotKey(0x77, "View Impact Point: ^9F8", undefined, ::ViewImpactPoint, undefined, level.players[0]);
    level.projectile_camera = true;
    level.display_messages = false;
    level flag::init("camera_viewing_projectile");
    level flag::init("camera_viewing_impact");

    WaitFadeIn();
    level.players[0] thread OpenAllDoors();
    level.players[0] thread ActivateAllPower();
    foreach (player in GetPlayers()) {
        player.score = 50000;
        player SetOrigin((300 - (50 * player.characterindex), 2300, 912.125));
        player SetPlayerAngles((0, -90, 0));
        player InfiniteAmmo(true);
        player GiveAllPerks();
        player GivePlayerWeapon("elemental_bow");
        player thread WatchBowShots();
    }
    
    level flag::wait_till("initial_blackscreen_passed");
    level.display_messages = true;
    thread WriteToScreen("Bow Shots Practice Loaded");
    level flag::clear("spawn_zombies");

    foreach(wisp in GetEntArray("activate_teleporter_object", "targetname")) {
		wisp thread WatchWisp();
	}

    obelisk = GetEnt("aq_rp_obelisk_magma_trig", "targetname");
    for(;;) {
        obelisk waittill("damage", amount, attacker, direction, point);
        if([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_51a90202 ]](level.var_be94cdb, 1, point, obelisk)) {
            IPrintLnBold("Fire Obelisk Hit");
        }
    }
}

ToggleBeaconFX()
{
    foreach (beacon in GetEntArray("aq_es_beacon_trig", "script_noteworthy")) {
        beacon_fx = struct::get(beacon.target);
        if (!IsDefined(beacon_fx.fx)) {
            beacon_fx.fx = util::spawn_model("tag_origin", beacon_fx.origin);
        }
        value = beacon_fx.fx clientfield::get("beacon_fx");
        beacon_fx.fx clientfield::set("beacon_fx", !value);
    }
    IPrintLnBold("Beacons " + (value ? "Deactivated" : "Activated"));
}

ToggleDogFog()
{
    value = level clientfield::get("castle_fog_bank_switch");
    level clientfield::set("castle_fog_bank_switch", !value);
    IPrintLnBold("Fog " + (value ? "Deactivated" : "Activated"));
}

WatchBowShots()
{
    for (;;) {
        self waittill("missile_fire", projectile);
        self.projectile_impact_origin = undefined;
        self thread WaitArrowImpact(projectile);
        if (level.projectile_camera) {
            self FollowProjectile(projectile, "bow_camera_ended", "end_arrow_camera", "death");
            self notify("bow_camera_ended");
        }
    }
}

WaitArrowImpact(projectile)
{
    self endon("projectile_timeout");
    projectile WaitEntDeath(1, "projectile_timeout");
    self waittill("projectile_impact", weapon, point);
    self.projectile_impact_origin = point;
    CheckArrowImpactTouching(point);
}

CheckArrowImpactTouching(point)
{
    foreach (beacon in GetEntArray("aq_es_beacon_trig", "script_noteworthy")) {
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_51a90202 ]](level.var_be94cdb, 1, point, beacon)) {
            IPrintLnBold("Storm Beacon Hit");
            return;
        }
    }
    foreach (circle in GetEntArray("aq_rp_runic_circle_volume", "script_noteworthy")) {
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_51a90202 ]](level.var_be94cdb, 1, point, circle)) {
            IPrintLnBold("Fire Circle Hit");
            return;
        }
    }
    foreach (fireplace in struct::get_array("aq_rp_fireplace_struct", "targetname")) {
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_51a90202 ]](level.var_be94cdb, 1, point, GetEnt(fireplace.target, "targetname"))) {
            IPrintLnBold("Fireplace Hit");
            return;
        }
    }
}

WatchWisp()
{
    self SetCanDamage(1);
	self.health = 999999;

    s_fx = struct::get(self.target, "targetname");
	s_fx fx::play("battery_charge", s_fx.origin, undefined, "delete_fx", 0, undefined, 1);

    for(;;) {
		self waittill("damage", n_damage, e_attacker, v_direction, v_point, str_means_of_death, str_tag_name, str_model_name, str_part_name, w_weapon);
		if(IsSubStr(w_weapon.name, "elemental_bow")) {
			IPrintLnBold("Wisp Hit");
		}
        wait 0.25;
	}
}