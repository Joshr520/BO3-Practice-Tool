RevInGameTimer()
{
    level endon("game_ended");

    level flag::wait_till("start_zombie_round_logic");
    wait 2.1;
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
    WaitForKeeperStart();
    keeper SetText("^2" + CalcTime(GetTime() - start_time));

    kino_reel = InitHud(0);
    kino_reel.label = &"Kino Reel: ";
    InitSplitHud(kino_reel);
    WaitForReel2();
    kino_reel SetText("^2" + CalcTime(GetTime() - start_time));

    bones = InitHud(0);
    bones.label = &"Bones: ";
    InitSplitHud(bones);
    WaitForReel3();
    bones SetText("^2" + CalcTime(GetTime() - start_time));

    house = InitHud(0);
    house.label = &"House: ";
    InitSplitHud(house);
    WaitForTP1();
    house SetText("^2" + CalcTime(GetTime() - start_time));

    boss_1 = InitHud(0);
    boss_1.label = &"Boss 1: ";
    InitSplitHud(boss_1);
    WaitForBoss1();
    boss_1 SetText("^2" + CalcTime(GetTime() - start_time));

    symbols = InitHud(0);
    symbols.label = &"Symbols: ";
    InitSplitHud(symbols);
    WaitForSymbols();
    symbols SetText("^2" + CalcTime(GetTime() - start_time));

    boss_2 = InitHud(0);
    boss_2.label = &"Boss 2: ";
    InitSplitHud(boss_2);
    WaitForBoss2();
    boss_2 SetText("^2" + CalcTime(GetTime() - start_time));

    finish = InitHud(0);
    finish.label = &"Finish: ";
    InitSplitHud(finish);
    WaitForRevFinish();
    finish SetText("^2" + CalcTime(GetTime() - start_time));
}

WaitForKeeperStart()
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

WaitForReel2()
{
    level flag::wait_till("placed_audio2");
}

WaitForReel3()
{
    level flag::wait_till("placed_audio3");
}

WaitForTP1()
{
    level waittill("sophia_at_teleporter");
    for(;;)
    {
        level.var_7d7ca0ea waittill("trigger", player);
        if([[ @zm_genesis_teleporter<scripts\zm\zm_genesis_teleporter.gsc>::function_7ae798cc ]](player)) break;
    }
}

WaitForBoss1()
{
    level flag::wait_till("rift_entrance_open");
}

WaitForSymbols()
{
    level flag::wait_till("book_runes_success");
}

WaitForBoss2()
{
    level flag::wait_till("boss_fight");
    level.var_7d7ca0ea waittill("trigger", player);
}

WaitForRevFinish()
{
    while(!level IsScenePlaying("genesis_ee_sams_room")) wait 0.05;
    wait 0.3;
}