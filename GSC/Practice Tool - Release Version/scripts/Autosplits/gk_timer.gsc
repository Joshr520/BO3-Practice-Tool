GKInGameTimer()
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

    fly_1 = InitHud(0);
    fly_1.label = &"Fly 1: ";
    fly_1 InitSplitHud(fly_1);
    WaitForFly();
    fly_1 SetText("^2" + CalcTime(GetTime() - start_time));

    fly_2 = InitHud(0);
    fly_2.label = &"Fly 2: ";
    fly_2 InitSplitHud(fly_2);
    level waittill(#"hash_803aa6bf");
    WaitForFly();
    fly_2 SetText("^2" + CalcTime(GetTime() - start_time));

    lockdown = InitHud(0);
    lockdown.label = &"Lockdown: ";
    lockdown InitSplitHud(lockdown);
    thread MonitorLockdownTimer();
    WaitForLockdown();
    lockdown SetText("^2" + CalcTime(GetTime() - start_time));

    challenges = InitHud(0);
    challenges.label = &"Challenges: ";
    challenges InitSplitHud(challenges);
    WaitForChallenges();
    challenges SetText("^2" + CalcTime(GetTime() - start_time));

    download = InitHud(0);
    download.label = &"Download: ";
    download InitSplitHud(download);
    WaitForDownload();
    download SetText("^2" + CalcTime(GetTime() - start_time));

    boss = InitHud(0);
    boss.label = &"Boss: ";
    boss InitSplitHud(boss);
    WaitForGKBoss();
    boss SetText("^2" + CalcTime(GetTime() - start_time));

    finish = InitHud(0);
    finish.label = &"Finish: ";
    finish InitSplitHud(finish);
    WaitForGKFinish();
    finish SetText("^2" + CalcTime(GetTime() - start_time));
}

WaitForFly()
{
    thread WaitForDragonFly("cin_t7_ai_zm_dlc3_dragon_transport_roost2");
    thread WaitForDragonFly("cin_t7_ai_zm_dlc3_dragon_transport_roost1");
    thread WaitForDragonFly("cin_t7_ai_zm_dlc3_dragon_transport_roost3");
    level waittill("dragon_flying");
}

WaitForDragonFly(scene)
{
    level endon("dragon_flying");
    while(!level IsScenePlaying(scene + "_idle_2_pavlovs")) wait 0.05;
    level notify("dragon_flying");
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

KillAll()
{
    for(;;)
    {
        foreach(zombie in GetAITeamArray(level.zombie_team))
        {
            zombie Kill();
        }
        wait 0.05;
    }
}

WaitForLockdown()
{
    level flag::wait_till("dragon_strike_unlocked");
    level flag::wait_till("lockdown_active");
	level flag::wait_till("lockdown_complete");
}

WaitForChallenges()
{
    level flag::wait_till("scenario_active");
}

WaitForDownload()
{
    level waittill(#"hash_8cc49f44");
}

WaitForGKBoss()
{
    GetEnt("ee_sewer_to_arena_trig", "targetname") waittill("trigger", player);
}

WaitForGKFinish()
{
    level waittill(#"hash_9b1cee4c");
}