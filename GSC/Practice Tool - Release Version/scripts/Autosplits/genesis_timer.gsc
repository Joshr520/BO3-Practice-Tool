RevInGameTimer()
{
    level endon("game_ended");

    WaitFadeIn();
    start_time = GetTime();
    timer = InitHud(0);
    timer SetTimerUp(0);
    timer.label = &"^3Time: ^2";
    InitSplitHud(timer);

    round_timer = InitHud(0);
    round_timer SetTenthsTimerUp(0.05);
    round_timer.label = &"^3Round Time: ^2";
    InitSplitHud(round_timer);
    thread ManageInGameRoundTimer(round_timer);

    keeper = InitHud(0);
    keeper.label = &"Keeper: ";
    InitSplitHud(keeper);
    WaitKeeperStart();
    keeper SetText("^2" + CalcTime(GetTime() - start_time));

    kino_reel = InitHud(0);
    kino_reel.label = &"Kino Reel: ";
    InitSplitHud(kino_reel);
    WaitReel(2, 0);
    kino_reel SetText("^2" + CalcTime(GetTime() - start_time));

    bones = InitHud(0);
    bones.label = &"Bones: ";
    InitSplitHud(bones);
    WaitReel(3, 0);
    bones SetText("^2" + CalcTime(GetTime() - start_time));

    house = InitHud(0);
    house.label = &"House: ";
    InitSplitHud(house);
    WaitSophiaTP();
    house SetText("^2" + CalcTime(GetTime() - start_time));

    boss_1 = InitHud(0);
    boss_1.label = &"Boss 1: ";
    InitSplitHud(boss_1);
    WaitBoss1();
    boss_1 SetText("^2" + CalcTime(GetTime() - start_time));

    symbols = InitHud(0);
    symbols.label = &"Symbols: ";
    InitSplitHud(symbols);
    WaitSymbols();
    symbols SetText("^2" + CalcTime(GetTime() - start_time));

    boss_2 = InitHud(0);
    boss_2.label = &"Boss 2: ";
    InitSplitHud(boss_2);
    WaitSophiaTP(1);
    boss_2 SetText("^2" + CalcTime(GetTime() - start_time));

    finish = InitHud(0);
    finish.label = &"Finish: ";
    InitSplitHud(finish);
    WaitRevFinish();
    finish SetText("^2" + CalcTime(GetTime() - start_time));
}

// 1 = mob, 2 = spawn, 3 = de, 4 = verruckt
WaitGenOn(num)
{
    level flag::wait_till("power_on" + num);
}

// 1 = mob, 2 = spawn, 3 = de, 4 = verruckt
WaitBeastLeave(num)
{
    for(;;)
    {
        while(!IsTrue(self.var_5aef0317)) wait 0.05;
        while(!IsTrue(self.streamer_hint)) wait 0.05;
        zone = zm_zonemgr::get_zone_from_position(self.streamer_hint.origin + VectorScale((0, 0, 1), 32), 0);
        switch(num)
        {
            case 1:
                if(zone == "zm_prison_power_zone") return;
                break;
            case 2:
                if(zone == "start_power_zone") return;
                break;
            case 3:
                if(zone == "zm_castle_power_zone") return;
                break;
            case 4:
                if(zone == "zm_asylum_power_zone") return;
                break;
        }
        while(IsTrue(self.var_5aef0317)) wait 0.05;
    }
}

WaitKeeperStart()
{
    while(!IsDefined(level.ai_companion)) wait 0.05;
    for(;;)
    {
        while(IsAlive(level.ai_companion) && !level.ai_companion IsScenePlaying("cin_zm_dlc4_keeper_prtctr_ee_idle")) wait 0.05;
        if(level.ai_companion IsScenePlaying("cin_zm_dlc4_keeper_prtctr_ee_idle")) return;
        while(!IsAlive(level.ai_companion)) wait 0.05;
        wait 0.05;
    }
}

WaitReel(num, pickup)
{
    if(pickup) level flag::wait_till("got_audio" + num);
    else level flag::wait_till("placed_audio" + num);
}

// 0 = house (default), 1 = boss 2
WaitSophiaTP(ee_state = 0)
{
    level flag::wait_till("sophia_at_teleporter");
    if(ee_state) level flag::wait_till("toys_collected");
    for(;;)
    {
        level.var_7d7ca0ea waittill("trigger", player);
        if(!zm_utility::is_player_valid(player) || level.var_18879020) continue;
        if([[ @zm_genesis_teleporter<scripts\zm\zm_genesis_teleporter.gsc>::function_7ae798cc ]](player)) return;
    }
}

WaitEggFill(num)
{
    if(!IsDefined(level.egg_fill_split)) thread EggFillSplit();
    level waittill("egg_fill_split" + num);
}

EggFillSplit()
{
    level.egg_fill_split = 1;
    foreach(pod in level.var_2a7689da) pod.var_165d49f6 thread WaitPodSouls();
}

WaitPodSouls()
{
    self waittill(#"hash_71f0e810");
    level waittill("egg_fill_split" + level.egg_fill_split);
    level.egg_fill_split++;
}

WaitRunePickup(num)
{
    if(!IsDefined(level.rune_pickup_split)) thread RunePickupSplit();
    level waittill("rune_pickup_split" + num);
}

RunePickupSplit()
{
    level.rune_pickup_split = 1;
    runes = Array("electricity_rq_started", "fire_rq_started", "light_rq_started", "shadow_rq_started");
    foreach(rune in runes) thread WaitRuneFlag(rune);
}

WaitRuneFlag(rune)
{
    level flag::wait_till(rune);
    level notify("rune_pickup_split" + level.rune_pickup_split);
    level.rune_pickup_split++;
}

WaitBoss1()
{
    level flag::wait_till("rift_entrance_open");
}

WaitSymbols()
{
    level flag::wait_till("book_runes_success");
}

WaitBossRush()
{
    level waittill(#"hash_f81a82d1");
}

WaitRevFinish()
{
    while(!level IsScenePlaying("genesis_ee_sams_room")) wait 0.05;
    wait 0.3;
}