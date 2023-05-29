WaitDragonFull(num)
{
    dragon = level.soul_catchers[num];
    dragon waittill("fully_charged");
}

WaitBow()
{
    foreach(player in GetPlayers()) player thread WatchBow();
    level waittill("bow_picked_up");
}

WatchBow()
{
    level endon("bow_picked_up");
    self waittill(#"hash_99ff6d52");
    level notify("bow_picked_up");
}

WaitStartStorm()
{
    level waittill(#"hash_6d0730ef");
}

WaitStormBonfires()
{
    bonfires = GetEntArray("aq_es_beacon_trig", "script_noteworthy");
    array::wait_till(bonfires, "beacon_activated");
}

WaitStormWallrun()
{
    level flag::wait_till("elemental_storm_wallrun");
}

WaitStormCrackle()
{
    level flag::wait_till_all(Array("elemental_storm_batteries", "elemental_storm_beacons_charged"));
}

WaitStormUpgraded()
{
    trig = struct::get("upgraded_bow_struct_elemental_storm", "targetname");
    for(;;)
    {
        trig.var_67b5dd94 waittill("trigger", player);
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_9dfa159b ]]()) continue;
        weapons = player GetWeaponsListPrimaries();
        if(weapons.size == 1 && IsSubStr(weapons[0].name, "elemental_bow")) continue;
    }
}

WaitStartFire()
{
    struct::get("quest_start_rune_prison").var_67b5dd94 waittill("trigger", player);
}

WaitFireObelisk()
{
    level flag::wait_till("rune_prison_obelisk");
}

WaitFireCirclesFilled()
{
    array::wait_till(GetEntArray("aq_rp_runic_circle_volume", "script_noteworthy"), "runic_circle_charged");
}

WaitFireGolf()
{
    level flag::wait_till("rune_prison_golf");
}

WaitFireUpgraded()
{
    trig = struct::get("upgraded_bow_struct_rune_prison", "targetname");
    for(;;)
    {
        trig.var_67b5dd94 waittill("trigger", player);
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_9dfa159b ]]()) continue;
        weapons = player GetWeaponsListPrimaries();
        if(weapons.size == 1 && IsSubStr(weapons[0].name, "elemental_bow")) continue;
        return;
    }
}

WaitStartVoid()
{
    struct::get("quest_start_demon_gate").var_67b5dd94 waittill("trigger", player);
}

WaitVoidKnife()
{
    level flag::wait_till("demon_gate_seal");
}

WaitVoidSkulls()
{
    array::wait_till(GetEntArray("aq_dg_fossil", "script_noteworthy"), "returned");
}

WaitVoidCrawlers()
{
    level flag::wait_till("demon_gate_crawlers");
}

WaitVoidRunes()
{
    level flag::wait_till("demon_gate_runes");
}

WaitVoidUpgraded()
{
    trig = struct::get("upgraded_bow_struct_demon_gate", "targetname");
    for(;;)
    {
        trig.var_67b5dd94 waittill("trigger", player);
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_9dfa159b ]]()) continue;
        weapons = player GetWeaponsListPrimaries();
        if(weapons.size == 1 && IsSubStr(weapons[0].name, "elemental_bow")) continue;
        return;
    }
}

WaitStartWolf()
{
    struct::get("quest_start_wolf_howl").var_67b5dd94 waittill("trigger", player);
}

WaitWolfShrine()
{
    for(;;)
    {
        level.var_52978d72 waittill("projectile_impact", weapon, point, radius, attacker, normal);
        if([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_51a90202 ]](weapon, 1, point, GetEnt("aq_wh_skull_shrine_trig", "targetname"))) return;
    }
}

WaitWolfSpawnWolf()
{
    for(;;)
    {
        GetEnt("aq_wh_skadi_skull", "targetname").var_67b5dd94 waittill("trigger", player);
        if(player === level.var_52978d72) return;
    }
}

WaitWolfSoulsFilled()
{
    level flag::wait_till("wolf_howl_escort");
}

WaitWolfArrowForged()
{
    level flag::wait_till("wolf_howl_repaired");
}

WaitWolfUpgraded()
{
    trig = struct::get("upgraded_bow_struct_wolf_howl", "targetname");
    for(;;)
    {
        trig.var_67b5dd94 waittill("trigger", player);
        if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_9dfa159b ]]()) continue;
        weapons = player GetWeaponsListPrimaries();
        if(weapons.size == 1 && IsSubStr(weapons[0].name, "elemental_bow")) continue;
        return;
    }
}

WaitTeleport(num)
{
    foreach(tp in level.var_27b3c884) tp thread WaitAnyTP();
    for(;;)
    {
        level waittill("teleport_activated");
        if(!num) return;
        if(!level flag::get("time_travel_teleporter_ready")) continue;
        if(!level flag::get("mpd_canister_replacement") && num == 1) return;
        else if(level flag::get("mpd_canister_replacement") && num == 2) return;
    }
}

WaitAnyTP()
{
    for(;;)
    {
        self waittill("trigger", player);
		if(zm_utility::is_player_valid(player) && !level.is_cooldown && !level flag::get("rocket_firing"))
		{
			if([[ @zm_castle_teleporter<scripts\zm\zm_castle_teleporter.gsc>::function_6b3344b4 ]]())
			{
				if(![[ @zm_castle_teleporter<scripts\zm\zm_castle_teleporter.gsc>::function_ad16f13c ]](player)) continue;
				level notify("teleport_activated");
			}
		}
    }
}

WaitSafeCode()
{
    level flag::wait_till("ee_safe_open");
}

WaitSimon(num)
{
    terminals = struct::get_array("golden_key_slot");
    for(;;)
    {
        level waittill(#"hash_b0b992fb");
        completed = 0;
        if(terminals[0].var_cf5a713.b_done) completed++;
        if(terminals[1].var_cf5a713.b_done) completed++;
        if(completed >= num) return;
    }
}

WaitKeeper(num)
{
    switch(num)
    {
        case 0:
            struct::get("vril_generator_family") waittill("trigger_activated");
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            for(i = 0; i < num; i++)
            {
                level flag::wait_till("next_channeling_stone");
                level flag::wait_till_clear("next_channeling_stone");
            }
            break;
        case 5:
            level flag::wait_till("see_keeper");
            break;
    }
}

WaitBoss(num)
{
    if(!num) level flag::wait_till("boss_fight_begin");
    else level flag::wait_till("boss_fight_completed");
}

WaitCastleEnd()
{
    while(!level IsScenePlaying("cin_cas_01_outro_3rd_static_poses")) wait 0.05;
}