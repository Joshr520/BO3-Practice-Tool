SplitDragonFilled(split_name)
{
    switch (split_name) {
        case "Church Dragon": {
            num = 1;
            break;
        }
        case "Courtyard Dragon": {
            num = 0;
            break;
        }
        case "Undercroft Dragon": {
            num = 2;
            break;
        }
        default: {
            return;
        }
    }
    while (!IsDefined(level.soul_catchers[num])) {
        wait 0.05;
    }
    level.soul_catchers[num] waittill("fully_charged");
}

SplitBowPickup(split_name)
{
    level flag::wait_till("soul_catchers_charged");
    foreach (player in GetPlayers()) {
        player thread WatchPlayerForBow();
    }
    level waittill("bow_acquired");
}

WatchPlayerForBow()
{
    level endon("bow_acquired");
    for (;;) {
        self waittill("weapon_give", weapon);
        if (weapon == GetWeapon("elemental_bow")) {
            level notify("bow_acquired");
            return;
        }
    }
}

SplitStormStart(split_name)
{
    level waittill(#"hash_6d0730ef");
}

SplitStormBonfires(split_name)
{
    array::wait_till(GetEntArray("aq_es_beacon_trig", "script_noteworthy"), "beacon_activated");
}

SplitStormWallrun(split_name)
{
    level flag::wait_till("elemental_storm_wallrun");
}

SplitStormCrackle(split_name)
{
    level flag::wait_till_all(Array("elemental_storm_batteries", "elemental_storm_beacons_charged"));
}

SplitStormUpgraded(split_name)
{
    level flag::wait_till("elemental_storm_spawned");
}

SplitFireStart(split_name)
{
    while (!IsDefined(struct::get("quest_start_rune_prison").var_67b5dd94)) {
        wait 0.05;
    }
    for (;;) {
        struct::get("quest_start_rune_prison").var_67b5dd94 waittill("trigger", player);
        if (player BasicTrigPlayerCheck()) {
            return;
        }
    }
}

SplitFireObelisk(split_name)
{
    level flag::wait_till("rune_prison_obelisk");
}

SplitFireCircles(split_name)
{
    array::wait_till(GetEntArray("aq_rp_runic_circle_volume", "script_noteworthy"), "runic_circle_charged");
}

SplitFireGolf(split_name)
{
    level flag::wait_till("rune_prison_golf");
}

SplitFireUpgraded(split_name)
{
    level flag::wait_till("rune_prison_spawned");
}

SplitVoidStart(split_name)
{
    while (!IsDefined(struct::get("quest_start_demon_gate").var_67b5dd94)) {
        wait 0.05;
    }
    for (;;) {
        struct::get("quest_start_demon_gate").var_67b5dd94 waittill("trigger", player);
        if (player BasicTrigPlayerCheck()) {
            return;
        }
    }
}

SplitVoidKnife(split_name)
{
    level flag::wait_till("demon_gate_seal");
}

SplitVoidSkulls(split_name)
{
    array::wait_till(GetEntArray("aq_dg_fossil", "script_noteworthy"), "returned");
}

SplitVoidCrawlers(split_name)
{
    level flag::wait_till("demon_gate_crawlers");
}

SplitVoidRunes(split_name)
{
    level flag::wait_till("demon_gate_runes");
}

SplitVoidUpgraded(split_name)
{
    level flag::wait_till("demon_gate_spawned");
}

SplitWolfStart(split_name)
{
    while (!IsDefined(struct::get("quest_start_wolf_howl").var_67b5dd94)) {
        wait 0.05;
    }
    for (;;) {
        struct::get("quest_start_wolf_howl").var_67b5dd94 waittill("trigger", player);
        if (player BasicTrigPlayerCheck()) {
            return;
        }
    }
}

SplitWolfShrine(split_name)
{
    while (level clientfield::get("quest_state_wolf") < 2) {
        wait 0.05;
    }
}

SplitWolfStartEscort(split_name)
{
    while (!IsDefined(GetEnt("aq_wh_skadi_skull", "targetname").var_67b5dd94)) {
        wait 0.05;
    }
    for (;;) {
        GetEnt("aq_wh_skadi_skull", "targetname").var_67b5dd94 waittill("trigger", player);
        if (player === level.var_52978d72) {
            return;
        }
    }
}

SplitWolfSoulsFilled(split_name)
{
    level flag::wait_till("wolf_howl_escort");
}

SplitWolfArrowForged(split_name)
{
    level flag::wait_till("wolf_howl_repaired");
}

SplitWolfUpgraded(split_name)
{
    level flag::wait_till("wolf_howl_spawned");
}

SplitCastleTeleport(split_name)
{
    for (;;) {
        level waittill("teleport_activated");
        if (split_name == "Normal TP") {
            return;
        }
        if (!level flag::get("time_travel_teleporter_ready")) {
            continue;
        }
        if (!level flag::get("mpd_canister_replacement") && split_name == "Time Travel 1") {
            return;
        }
        else if (level flag::get("mpd_canister_replacement") && split_name == "Time Travel 2") {
            return;
        }
    }
}

detour zm_castle_teleporter<scripts\zm\zm_castle_teleporter.gsc>::function_264f93ff(var_edc2ee2a = 0, var_66f7e6b9 = 0)
{
    level notify("teleport_activated");
    return self [[ @zm_castle_teleporter<scripts\zm\zm_castle_teleporter.gsc>::function_264f93ff ]](var_edc2ee2a, var_66f7e6b9);
}

SplitSafeCode(split_name)
{
    level flag::wait_till("ee_safe_open");
}

SplitSimon(split_name)
{
    if (split_name == "Simon 1") {
        num = 1;
    }
    else {
        num = 2;
    }
    terminals = struct::get_array("golden_key_slot");
    for (;;) {
        level waittill(#"hash_b0b992fb");
        completed = 0;
        if (terminals[0].var_cf5a713.b_done) {
            completed++;
        }
        if (terminals[1].var_cf5a713.b_done) {
            completed++;
        }
        if (completed >= num) {
            return;
        }
    }
}

SplitKeeper(split_name)
{
    switch (split_name) {
        case "Keeper Spawned": {
            struct::get("vril_generator_family") waittill("trigger_activated");
            break;
        }
        case "Keeper 1": {
            level waittill("keeper_done_1");
            break;
        }
        case "Keeper 2": {
            level waittill("keeper_done_2");
            break;
        }
        case "Keeper 3": {
            level waittill("keeper_done_3");
            break;
        }
        case "Keeper 4": {
            level waittill("keeper_done_4");
            break;
        }
        case "Keeper Trapped": {
            level flag::wait_till("see_keeper");
            break;
        }
        default: {
            return;
        }
    }
}

autoexec TrackKeeper()
{
    WaitFadeIn();
    if (level.mapName != "zm_castle" || !IsTrue(level.num_splits)) {
        return;
    }

    num = 1;
    for(i = 0; i < 4; i++) {
        level flag::wait_till("next_channeling_stone");
        level notify("keeper_done_" + num);
        num++;
        level flag::wait_till_clear("next_channeling_stone");
    }
}

SplitCastleBoss(split_name)
{
    if (split_name == "Boss Enter") {
        level flag::wait_till("boss_fight_begin");
    }
    else {
        level flag::wait_till("boss_fight_completed");
    }
}

SplitCastleEnd()
{
    while (!level IsScenePlaying("cin_cas_01_outro_3rd_sh010")) {
        wait 0.05;
    }
}