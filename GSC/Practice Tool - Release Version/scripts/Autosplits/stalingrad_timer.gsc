GKInGameTimer()
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

    fly_1 = InitHud(0);
    fly_1.label = &"Fly 1: ";
    fly_1 InitSplitHud(fly_1);
    WaitFly(2);
    fly_1 SetText("^2" + CalcTime(GetTime() - start_time));

    fly_2 = InitHud(0);
    fly_2.label = &"Fly 2: ";
    fly_2 InitSplitHud(fly_2);
    level waittill(#"hash_803aa6bf");
    WaitFly(1);
    fly_2 SetText("^2" + CalcTime(GetTime() - start_time));

    lockdown = InitHud(0);
    lockdown.label = &"Lockdown: ";
    lockdown InitSplitHud(lockdown);
    thread MonitorLockdownTimer();
    WaitPAPLockdown();
    lockdown SetText("^2" + CalcTime(GetTime() - start_time));

    challenges = InitHud(0);
    challenges.label = &"Challenges: ";
    challenges InitSplitHud(challenges);
    WaitChallenges();
    challenges SetText("^2" + CalcTime(GetTime() - start_time));

    download = InitHud(0);
    download.label = &"Download: ";
    download InitSplitHud(download);
    WaitDownload();
    download SetText("^2" + CalcTime(GetTime() - start_time));

    boss = InitHud(0);
    boss.label = &"Boss: ";
    boss InitSplitHud(boss);
    WaitStalingradBoss();
    boss SetText("^2" + CalcTime(GetTime() - start_time));

    finish = InitHud(0);
    finish.label = &"Finish: ";
    finish InitSplitHud(finish);
    WaitStalingradFinish();
    finish SetText("^2" + CalcTime(GetTime() - start_time));
}

WaitPAPGroph(num)
{
    switch(num)
    {
        case 0:
            piecename = "part_transmitter";
            break;
        case 1:
            piecename = "part_codes";
            break;
        case 2:
            piecename = "part_map";
            break;
    }
    level flag::wait_till("dragonride_" + piecename + "_found");
}

// tank = 1, supply = 2, dc = 3
WaitFly(num)
{
    while(!level IsScenePlaying("cin_t7_ai_zm_dlc3_dragon_transport_roost" + num + "_idle_2_pavlovs")) wait 0.05;
}

WaitPickupEgg()
{
    level flag::wait_till("dragon_egg_acquired");
}

WaitEggBathed()
{
    level flag::wait_till("egg_bathed_in_flame");
}

WaitIncubationStart()
{
    level flag::wait_till("egg_placed_incubator");
}

WaitGauntletPickup()
{
    level flag::wait_till("dragon_gauntlet_acquired");
}

MonitorLockdownTimer()
{
    lockdown_timer = InitHud(0);
    lockdown_timer.alignX = "center";
	lockdown_timer.alignY = "top";
	lockdown_timer.horzAlign = "user_center";
	lockdown_timer.vertAlign = "user_top";
    lockdown_timer.label = &"^3Lockdown Time: ";

    level flag::wait_till("dragon_strike_unlocked");

    start_time = GetTime();
    lockdown_timer.alpha = 1;
    lockdown_timer SetTimerUp(0);
    
    for(i = 0; i < 3; i++)
    {
        level waittill(#"hash_d2eac5fe");
        start_time = GetTime();
        lockdown_timer SetTimerUp(0);
    }

    level waittill(#"hash_d2eac5fe");
    lockdown_timer Destroy();
}

WaitPAPLockdown(num)
{
    level flag::wait_till("dragon_strike_unlocked");
    level flag::wait_till("lockdown_active");
    if(!num) return;
	level flag::wait_till("lockdown_complete");
}

WaitChallenges()
{
    level flag::wait_till("scenario_active");
}

WaitDownload()
{
    level waittill(#"hash_8cc49f44");
}

WaitStalingradBoss()
{
    GetEnt("ee_sewer_to_arena_trig", "targetname") waittill("trigger", player);
}

WaitStalingradFinish()
{
    level waittill(#"hash_9b1cee4c");
}