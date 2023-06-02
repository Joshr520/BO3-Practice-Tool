LoadSplits()
{
    if(!compiler::loadsplitjson()) return;
    maps = ["zm_zod", "zm_factory", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_prototype", "zm_asylum", "zm_sumpf", "zm_theater", "zm_cosmodrome", "zm_temple", "zm_moon", "zm_tomb"];
    map = compiler::getsplitvalue("Settings", "Map Index");
    if(maps[map] != level.script) return;

    split_size = compiler::getsplitvalue("Settings", "Amount of Splits");
    split_type = compiler::getsplitvalue("Settings", "Split Type");
    igt = compiler::getsplitvalue("Settings", "In Game Timer");
    igrt = compiler::getsplitvalue("Settings", "In Game Round Timer");
    round_interval = compiler::getsplitvalue("Settings", "Round Interval");

    level.num_splits = split_size + 1;

    split_funcs = [];
    split_labels = [];
    split_funcs["zm_zod"] = Array(::SplitMagicianRitual, ::SplitFemmeRitual, ::SplitDetectoveRitual, ::SplitBoxerRitual, ::SplitPAPRitual, ::SplitCanalsRift, ::SplitFootlightRift, ::SplitWaterfrontRift, ::SplitRiftCanals, ::SplitRiftFootlight,
    ::SplitRiftWaterfront, ::SplitCanalsEgg, ::SplitFootlightEgg, ::SplitWaterfrontEgg, ::SplitRiftEgg, ::SplitPickupSword, ::SplitJunctionOvum, ::SplitCanalsOvum, ::SplitFootlightOvum, ::SplitWaterfrontOvum, ::SplitUpgradedSword, ::SplitBook,
    ::SplitFlag1, ::SplitFlag2, ::SplitFlag3, ::SplitFlag4);
    split_labels["zm_zod"] = Array(&"Magician Ritual: ", &"Femme Ritual: ", &"Detective Ritual: ", &"Boxer Ritual: ", &"PAP Ritual: ", &"Canals Rift: ", &"Footlight Rift: ", &"Waterfront Rift: ", &"Rift Canals: ", &"Rift Footlight: ", &"Rift Waterfront: ",
    &"Canals Egg: ", &"Footlight Egg: ", &"Waterfront Egg: ", &"Rift Egg: ", &"Sword: ", &"Junction Ovum: ", &"Canals Ovum: ", &"Footlight Ovum: ", &"Waterfront Ovum: ", &"Upgraded Sword: ", &"Book: ", &"Flag 1: ", &"Flag 2: ", &"Flag 3: ", &"Flag 4: ");
    active_funcs = [];
    active_labels = [];
    round_funcs = [];
    round_params = [];

    for(i = 0; i < split_size; i++)
    {
        func_num = compiler::getsplitvalue("Split Data", "Key", i);
        round = compiler::getsplitvalue("Split Data", "Value", i);

        if(!IsDefined(split_funcs[level.script][func_num])) return;

        active_funcs[active_funcs.size] = split_funcs[level.script][func_num];
        active_labels[active_labels.size] = split_labels[level.script][func_num];
        if(!round) round_funcs[i] = ::WaitSplitCurrentRound;
        else if(round > 0)
        {
            round_funcs[i] = ::WaitSplitRound;
            round_params[i] = round;
        }
    }

    switch(maps[map])
    {
        case "zm_zod":
            active_funcs[active_funcs.size] = ::WaitZodEnd;
            break;
        default:
            break;
    }

    active_labels[active_labels.size] = &"End: ";

    level.in_game_splits = [];
    level.rendered_splits = [];
    level.split_top = 0;
    level.split_bottom = 8;
    level.last_split_added = 0;

    if(igt)
    {
        thread RunIGT();
        thread MonitorSplitLayoutChange();
        for(i = 0; i < 8; i++)
        {
            level.rendered_splits[i] = InitHud(0);
            InitSplitHud(level.rendered_splits[i]);
        }
    }
    if(igrt) thread RunIGRT(round_interval);

    thread Split();
    WaitFadeIn();
    thread Split();
    start_time = GetTime();

    foreach(index, func in active_funcs)
    {
        if(igt) AddIGTSplit(active_labels[index]);
        [[func]]();
        thread Split();
        if(IsDefined(round_funcs[index]))
        {
            if(IsDefined(round_params[index])) [[round_funcs[index]]](round_params[index]);
            else [[round_funcs[index]]]();
        }
        if(igt) level.in_game_splits[level.last_split_added - 1].text = "^2" + CalcTime(GetTime() - start_time);
    }

    RenderSplits();
}

RunIGT()
{
    WaitFadeIn();
    timer = InitHud(0);
    timer SetTimerUp(0);
    timer.label = &"^3Time: ^2";
    timer.x = 15;
    timer.y = 15;
    timer.alpha = 1;
}

RunIGRT(round_interval)
{
    WaitFadeIn();
    round_timer = InitHud(0);
    round_timer SetTenthsTimerUp(0.05);
    round_timer.label = &"^3Round Time: ^2";
    round_timer.x = 15;
    round_timer.y = 30;
    round_timer.alpha = 1;
    thread ManageInGameRoundTimer(round_timer);
}

AddIGTSplit(label)
{
    level.in_game_splits[level.last_split_added] = SpawnStruct();
    level.in_game_splits[level.last_split_added].label = label;
    level.in_game_splits[level.last_split_added].text = "";

    level.last_split_added++;

    if(level.last_split_added > level.split_bottom)
    {
        diff = level.last_split_added - level.split_bottom;
        level.split_top += diff;
        level.split_bottom += diff;
    }

    RenderSplits();
}

// TODO: ADD BUTTON TO EXPORT SPLITS
MonitorSplitLayoutChange()
{
    thread SplitMoveUp();
    thread SplitMoveDown();
}

SplitMoveUp()
{
    for(;;)
    {
        WaitPgUp();
        if(!level.split_top) continue;
        level.split_top--;
        level.split_bottom--;
        RenderSplits();
    }
}

SplitMoveDown()
{
    for(;;)
    {
        WaitPgDown();
        if(level.last_split_added <= level.split_bottom || level.split_bottom == level.num_splits) continue;
        level.split_top++;
        level.split_bottom++;
        RenderSplits();
    }
}

RenderSplits()
{
    for(i = level.split_top; i < level.split_bottom; i++)
    {
        if(i - level.split_top >= level.last_split_added) return;
        level.rendered_splits[i - level.split_top].label = level.in_game_splits[i].label;
        level.rendered_splits[i - level.split_top] SetText(level.in_game_splits[i].text);
        level.rendered_splits[i - level.split_top].alpha = 1;
    }
}

InitSplitHud(hud_elem, alpha = 0)
{
    hud_elem.x = 15;
    hud_elem.y = (level.rendered_splits.size + 2) * 15;
    hud_elem.alpha = alpha;
}

Split()
{
    if(!IsDefined(level.current_split_num)) level.current_split_num = 0;
    SetDvar("probation_league_matchHistoryWindow", level.current_split_num);
    level.current_split_num++;
}

StartInGameSplits()
{
    if(level.basic_timer)
    {
        WaitFadeIn();
        level.in_game_splits = [];
        timer = InitHud(0);
        timer SetTimerUp(0);
        timer.label = &"^3Time: ^2";
        InitSplitHud(timer);

        round_timer = InitHud(0);
        round_timer SetTenthsTimerUp(0.05);
        round_timer.label = &"^3Round Time: ^2";
        InitSplitHud(round_timer);
        thread ManageInGameRoundTimer(round_timer);
        return;
    }
    if(!level.map_specific_timer) return;
    switch(level.script)
    {
        case "zm_zod":

            break;
        case "zm_factory":

            break;
        case "zm_castle":

            break;
        case "zm_island":

            break;
        case "zm_stalingrad":
            self GKInGameTimer();
            break;
        case "zm_genesis":
            self RevInGameTimer();
            break;
    }
}

ManageInGameRoundTimer(round_timer)
{
    for(;;)
    {
        WaitSplitCurrentRound();
        round_timer SetTenthsTimerUp(0.05);
    }
}

WaitSplitCurrentRound()
{
    level waittill("end_of_round");
}

WaitSplitRound(round)
{
    while(level.round_number < round) level waittill("end_of_round");
}