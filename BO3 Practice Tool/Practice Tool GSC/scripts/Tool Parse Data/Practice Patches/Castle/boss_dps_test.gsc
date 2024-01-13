BossDPSTest()
{
    if (level.mapname != "zm_castle") {
        DebugPrint("error", "Boss DPS Test attempted to load on " + level.mapDisplayName + " but it requires Der Eisendrache");
        return;
    }
    level.display_messages = false;

    WaitFadeIn();

    zm_zonemgr::enable_zone("zone_boss_arena");
    TeleportPlayers();

    boss_struct = SpawnStruct();
	boss_struct InitBossData();
    boss_struct thread MonitorBossDamage();

	boss_struct.boss_model [[ @zm_castle_ee_bossfight<scripts\zm\zm_castle_ee_bossfight.gsc>::function_a1658f19 ]]("ai_zm_dlc1_archon_float_spell_elemental_pain_hitspike", "ai_zm_dlc1_archon_float_spell_elemental_pain_hitspike_idle");
	boss_struct.boss_model clientfield::set("boss_elemental_storm_stunned_keeper_fx", 1);
	boss_struct.boss_model clientfield::set("boss_weak_point_shader", 0);

    boss_struct.elapsed_time_hud = hud::createservertimer("default", 1.5);
    boss_struct.elapsed_time_hud hud::setpoint("TOPLEFT", "TOPLEFT", 5, 15);
    boss_struct.elapsed_time_hud.label = &"^3Elapsed Time: ^7";
    boss_struct.total_damage_hud = hud::createserverfontstring("default", 1.5);
    boss_struct.total_damage_hud hud::setpoint("TOPLEFT", "TOPLEFT", 5, 30);
    boss_struct.total_damage_hud.label = &"^3Total Damage: ^7";
    boss_struct.dps_hud = hud::createserverfontstring("default", 1.5);
    boss_struct.dps_hud hud::setpoint("TOPLEFT", "TOPLEFT", 5, 45);
    boss_struct.dps_hud.label = &"^3DPS: ^7";
    boss_struct.weapon_hud = hud::createserverfontstring("default", 1.5);
    boss_struct.weapon_hud hud::setpoint("TOPLEFT", "TOPLEFT", 5, 60);
    boss_struct.weapon_hud.label = &"^3Weapon: ^7";

    level flag::clear("spawn_zombies");
    level flag::wait_till("initial_blackscreen_passed");
    level.display_messages = true;
    thread WriteToScreen("DE Boss Damage Test Loaded");
}

TeleportPlayers()
{
    boss_start_tele_points = struct::get_array("boss_start_tele_point", "targetname");
    boss_start_tele_points_sorted = array::sort_by_script_int(boss_start_tele_points, 1);

    foreach(player in GetPlayers()) {
		v_pos = boss_start_tele_points_sorted[player.characterindex].origin;
		v_angles = boss_start_tele_points_sorted[player.characterindex].angles;
		player SetOrigin(v_pos);
		player SetPlayerAngles(v_angles);
		player clientfield::increment_to_player("player_snow_fx_off");
	}
}

InitBossData()
{
	boss_spawn_point = struct::get("boss_keeper", "targetname");
	self.boss_model = util::spawn_anim_model("c_zom_dlc1_keeper_archon_fb", boss_spawn_point.origin, boss_spawn_point.angles);
	self.boss_model EnableLinkTo();
	tag_aim_origin = self.boss_model GetTagOrigin("tag_aim");
	tag_aim_angles = self.boss_model GetTagAngles("tag_aim");
	self.boss_weak_point = GetEnt("boss_weak_point", "targetname");
	self.boss_weak_point.origin = tag_aim_origin;
	self.boss_weak_point.angles = tag_aim_angles;
	self.boss_weak_point.takedamage = 1;
	self.boss_weak_point LinkTo(self.boss_model, "tag_aim", (-7, 0, -15));
	self.boss_weak_point_blocker = GetEnt("boss_weak_point_blocker", "targetname");
	self.boss_weak_point_blocker.origin = tag_aim_origin;
	self.boss_weak_point_blocker.angles = tag_aim_angles;
	self.boss_weak_point_blocker LinkTo(self.boss_model, "tag_aim", (-17, 0, -15));
	self.boss_player_collision = GetEnt("boss_player_collision", "targetname");
	self.boss_player_collision LinkTo(self.boss_model);
	self.boss_dot_area = GetEnt("boss_dot_area", "targetname");
	self.boss_dot_area NotSolid();
	self.boss_dot_area LinkTo(self.boss_model);
	wait(0.05);
	self.boss_model clientfield::set("boss_fx", 1);
	self.boss_model clientfield::set("boss_weak_point_shader", 1);
	level clientfield::set("boss_wolf_howl_fx_change", 1);
    level clientfield::set("boss_gravity_spike_fx_change", 1);
}

MonitorBossDamage()
{
	for(;;) {
		self.boss_weak_point waittill("damage", damage, attacker, dir, loc, type, model, tag, part, weapon, flags);
        self notify("boss_damaged");
        if(IsDefined(self.total_damage)) {
            self.total_damage += damage;
            self.total_damage_hud SetValue(Int(self.total_damage));
            self thread HudTimeout();
        }
        else {
            self.total_damage = damage;
            self.start_time = GetTime();
            self.total_damage_hud SetValue(Int(damage));
            self.elapsed_time_hud SetTenthsTimerUp(0.05);
        }
        self.dps_hud SetValue(Int(self.total_damage / ((GetTime() - self.start_time) / 1000)));
        self.weapon_hud SetText(MakeLocalizedString(weapon.displayName));
	}
}

HudTimeout()
{
    self endon("boss_damaged");
    wait 5;
    self.total_damage = undefined;
    self.start_time = undefined;
    self.elapsed_time_hud SetText("");
    self.total_damage_hud SetValue(0);
    self.dps_hud SetValue(0);
    self.weapon_hud SetText("");
}