SplitEngine(split_name)
{
    switch (split_name) {
        case "Spawn Gen": {
            num = 2;
            break;
        }
        case "DE Gen": {
            num = 3;
            break;
        }
        case "Verruckt Gen": {
            num = 4;
            break;
        }
        case "Mob Gen": {
            num = 1;
            break;
        }
        default: {
            return;
        }
    }
    level flag::wait_till("power_on" + num);
}

SplitKeeperStart(split_name)
{
    while (!IsDefined(level.ai_companion)) {
        wait 0.05;
    }
    for (;;) {
        while (IsAlive(level.ai_companion) && !level.ai_companion IsScenePlaying("cin_zm_dlc4_keeper_prtctr_ee_idle")) {
            wait 0.05;
        }
        if (level.ai_companion IsScenePlaying("cin_zm_dlc4_keeper_prtctr_ee_idle")) {
            return;
        }
        while (!IsAlive(level.ai_companion)) {
            wait 0.05;
        }
        wait 0.05;
    }
}

SplitApothicon(split_name)
{
    if (split_name == "Enter Beast") {
        level waittill("apothicon_enter");
        return;
    }
    switch (StrTok(split_name, " ")[2]) {
        case "Mob": {
            zone = "prison";
            break;
        }
        case "Verruckt": {
            zone = "verrucht";
            break;
        }
        case "DE": {
            zone = "castle";
            break;
        }
        case "Spawn": {
            zone = "sheffield";
            break;
        }
        default: {
            return;
        }
    }
    IPrintLnBold("Waiting: " + "apothicon_leave_" + zone);
    level waittill("apothicon_leave_" + zone);
}

detour zm_genesis_apothican<scripts\zm\zm_genesis_apothican.gsc>::function_657f1d1(s_stub, str_zone, var_6d359b2e, var_db6533 = 0)
{
    IPrintLnBold("Notify: " + "apothicon_leave_" + s_stub.name);
    level notify("apothicon_leave_" + s_stub.name);
    return self [[ @zm_genesis_apothican<scripts\zm\zm_genesis_apothican.gsc>::function_657f1d1 ]](s_stub, str_zone, var_6d359b2e, var_db6533);
}

detour namespace_3ddd867f<scripts\zm\zm_genesis_pap_quest.gsc>::function_4ab898f4(nd_start)
{
    level notify("apothicon_enter");
    return self [[ @namespace_3ddd867f<scripts\zm\zm_genesis_pap_quest.gsc>::function_4ab898f4 ]](nd_start);
}

SplitReel(split_name)
{
    pickup = IsSubStr(split_name, "Pickup") ? true : false;
    num = StrTok(split_name, " ")[2];
    if (pickup) {
        level flag::wait_till("got_audio" + num);
        return;
    }
    level flag::wait_till("placed_audio" + num);
}

SplitGenesisTP(split_name)
{
    level flag::wait_till("sophia_at_teleporter");
    if (split_name == "Sophia TP") {
        level waittill("exploderee_teleporter_fx");
        return;
    }
    level flag::wait_till("toys_collected");
    level waittill("exploderee_teleporter_fx");
}

SplitGenesisEgg(split_name)
{
    level waittill("egg_filled_" + StrTok(split_name, " ")[1]);
}

autoexec CountGenesisEggs()
{
    num = 1;
    for (;;) {
        level waittill(#"hash_10ed65db");
        level notify("egg_filled_" + num);
        num++;
        if (num > 4) {
            break;
        }
    }
}

SplitRune(split_name)
{
    level waittill("rune_started_" + StrTok(split_name, " ")[1]);
}

autoexec CountGenesisRunes()
{
    num = 1;
    for (;;) {
        level flag::wait_till_any(Array("electricity_rq_started", "fire_rq_started", "light_rq_started", "shadow_rq_started"));
        level notify("rune_started_" + num);
        num++;
        if (num > 4) {
            break;
        }
    }
}

SplitBoss1(split_name)
{
    level flag::wait_till("rift_entrance_open");
}

SplitSymbols(split_name)
{
    level flag::wait_till("book_runes_success");
}

SplitBossRush(split_name)
{
    level waittill(#"hash_f81a82d1");
}

SplitGenesisEnd()
{
    while (!level IsScenePlaying("genesis_ee_sams_room")) {
        wait 0.05;
    }
    wait 0.3;
}