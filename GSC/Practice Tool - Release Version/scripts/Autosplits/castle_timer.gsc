WaitDragonFull(num)
{
    while(!IsDefined(level.soul_catchers[num])) wait 0.05;
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
    level flag::wait_till("elemental_storm_spawned");
}

WaitStartFire()
{
    while(!IsDefined(struct::get("quest_start_rune_prison").var_67b5dd94)) wait 0.05;
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
    level flag::wait_till("rune_prison_spawned");
}

WaitStartVoid()
{
    while(!IsDefined(struct::get("quest_start_demon_gate").var_67b5dd94)) wait 0.05;
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
    level flag::wait_till("demon_gate_spawned");
}

WaitStartWolf()
{
    while(!IsDefined(struct::get("quest_start_wolf_howl").var_67b5dd94)) wait 0.05;
    struct::get("quest_start_wolf_howl").var_67b5dd94 waittill("trigger", player);
}

WaitWolfShrine()
{
    while(CheckQuestProgress("wolf") < 2) wait 0.05;
}

WaitWolfSpawnWolf()
{
    while(!IsDefined(GetEnt("aq_wh_skadi_skull", "targetname").var_67b5dd94)) wait 0.05;
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
    level flag::wait_till("wolf_howl_spawned");
}

WaitTeleport(num)
{
    if(!IsDefined(level.tp_watch)) foreach(tp in level.var_27b3c884) tp thread WaitAnyTP();
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
    level.tp_watch = true;
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

TrackKeeper()
{
    level.tracking_keeper = true;
    level.keeper_num = 1;
    for(i = 0; i < 4; i++)
    {
        level flag::wait_till("next_channeling_stone");
        level.keeper_num++;
        level flag::wait_till_clear("next_channeling_stone");
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
            for(;;)
            {
                level flag::wait_till("next_channeling_stone");
                if(level.keeper_num >= num) return;
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
    while(!level IsScenePlaying("cin_cas_01_outro_3rd_sh010")) wait 0.05;
}