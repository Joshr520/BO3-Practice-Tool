LoadSplits()
{
    compiler::loadsplitjson();
    maps = ["zm_zod", "zm_factory", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_prototype", "zm_asylum", "zm_sumpf", "zm_theater", "zm_cosmodrome", "zm_temple", "zm_moon", "zm_tomb"];
    map = compiler::getsplitvalue("Settings", "Map Index");
    if(maps[map] != level.script) return;

    split_size = compiler::getsplitvalue("Settings", "Amount of Splits");
    split_type = compiler::getsplitvalue("Settings", "Split Type");
    igt = compiler::getsplitvalue("Settings", "In Game Timer");
    igrt = compiler::getsplitvalue("Settings", "In Game Round Timer");
    round_interval = compiler::getsplitvalue("Settings", "Round Interval");

    compiler::nprintln("In Game Timer: " + igt);
    compiler::nprintln("In Game Round Timer: " + igrt);
    compiler::nprintln("Amount of Splits: " + split_size);
    compiler::nprintln("Map Index: " + maps[map]);
    compiler::nprintln("Split Type: " + split_type);
    compiler::nprintln("Round Interval: " + round_interval);

    for(i = 0; i < split_size; i++)
    {
        compiler::nprintln("Split Index: " + compiler::getsplitvalue("Split Data", "Key", i) + " Round: " + compiler::getsplitvalue("Split Data", "Value", i));
    }
}

StartInGameSplits()
{
    if(level.basic_timer)
    {
        level flag::wait_till("start_zombie_round_logic");
        wait 2.1;
        level.in_game_splits = [];
        level.start_time_custom = GetTime();
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

InitSplitHud(hud_elem)
{
    hud_elem.y += level.in_game_splits.size * 15;
    hud_elem.x = 15;
    level.in_game_splits[level.in_game_splits.size] = hud_elem;
    hud_elem.alpha = 1;
}

ManageInGameRoundTimer(round_timer)
{
    for(;;)
    {
        level waittill("end_of_round");
        round_timer SetTenthsTimerUp(0.05);
    }
}