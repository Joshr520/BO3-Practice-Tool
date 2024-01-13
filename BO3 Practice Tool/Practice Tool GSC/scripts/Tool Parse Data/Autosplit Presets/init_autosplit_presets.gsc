autoexec InitAutosplitPresets()
{
    if (!GetTime()) {
        SetDvar("split_count", 0);
    }
    else if (GetDvarInt("split_count", 0) >= 1000) {
        thread PrintSplitsResetLobby();
    }
    if (!GetDvarInt("split_tutorial", 0)) {
        thread PrintSplitTutorial();
    }

    line = compiler::getline("Practice Tool/Autosplit Preset.txt", true);
    tokens = StrTok(line, ",");

    map = array::pop_front(tokens, 0);
    while (!IsDefined(level.mapname) || !IsDefined(level.mapname)) {
        wait 0.05;
    }
    if (map != level.mapname) {
        return;
    }

    split_funcs = [];

    switch (level.mapname) {
        case "zm_zod": {
            split_funcs["Magician Ritual"] = ::SplitRitual; split_funcs["Femme Ritual"] = ::SplitRitual; split_funcs["Detective Ritual"] = ::SplitRitual; split_funcs["Boxer Ritual"] = ::SplitRitual; split_funcs["PAP Ritual"] = ::SplitRitual;
            split_funcs["Canals - Rift"] = ::SplitRiftTP; split_funcs["Footlight - Rift"] = ::SplitRiftTP; split_funcs["Waterfront - Rift"] = ::SplitRiftTP; split_funcs["Rift - Canals"] = ::SplitRiftTP; split_funcs["Rift - Footlight"] = ::SplitRiftTP;
            split_funcs["Rift - Waterfront"] = ::SplitRiftTP; split_funcs["Canals Egg"] = ::SplitZodEgg; split_funcs["Footlight Egg"] = ::SplitZodEgg; split_funcs["Waterfront Egg"] = ::SplitZodEgg; split_funcs["Rift Egg"] = ::SplitZodEgg;
            split_funcs["Pickup Sword"] = ::SplitSwordPickup; split_funcs["Junction Ovum"] = ::SplitOvumStart; split_funcs["Canals Ovum"] = ::SplitOvumStart; split_funcs["Footlight Ovum"] = ::SplitOvumStart; split_funcs["Waterfront Ovum"] = ::SplitOvumStart;
            split_funcs["Pickup Upgraded Sword"] = ::SplitSwordPickup; split_funcs["Activate Book"] = ::SplitBook; split_funcs["Flag 1"] = ::SplitFlag; split_funcs["Flag 2"] = ::SplitFlag; split_funcs["Flag 3"] = ::SplitFlag; split_funcs["Flag 4"] = ::SplitFlag;
            break;
        }
        case "zm_factory": {
            break;
        }
        case "zm_castle": {
            split_funcs["Church Dragon"] = ::SplitDragonFilled; split_funcs["Courtyard Dragon"] = ::SplitDragonFilled; split_funcs["Undercroft Dragon"] = ::SplitDragonFilled; split_funcs["Pickup Bow"] = ::SplitBowPickup; split_funcs["Start Lightning"] = ::SplitStormStart;
            split_funcs["Bonfires Shot"] = ::SplitStormBonfires; split_funcs["Wall Ride"] = ::SplitStormWallrun; split_funcs["Crackle"] = ::SplitStormCrackle; split_funcs["Upgrade Lightning"] = ::SplitStormUpgraded; split_funcs["Start Fire"] = ::SplitFireStart;
            split_funcs["Obelisk Shot"] = ::SplitFireObelisk; split_funcs["Circles Filled"] = ::SplitFireCircles; split_funcs["Golf"] = ::SplitFireGolf; split_funcs["Upgrade Fire"] = ::SplitFireUpgraded; split_funcs["Start Void"] = ::SplitVoidStart; split_funcs["Activate Urn"] = ::SplitVoidKnife;
            split_funcs["Pickup Skulls"] = ::SplitVoidSkulls; split_funcs["Crawler Kills"] = ::SplitVoidCrawlers; split_funcs["Runes"] = ::SplitVoidRunes; split_funcs["Upgrade Void"] = ::SplitVoidUpgraded; split_funcs["Start Wolf"] = ::SplitWolfStart; split_funcs["Shrine Shot"] = ::SplitWolfShrine;
            split_funcs["Start Escort"] = ::SplitWolfStartEscort; split_funcs["Escort Finished"] = ::SplitWolfSoulsFilled; split_funcs["Arrow Forged"] = ::SplitWolfArrowForged; split_funcs["Upgrade Wolf"] = ::SplitWolfUpgraded; split_funcs["Normal TP"] = ::SplitCastleTeleport;
            split_funcs["Time Travel 1"] = ::SplitCastleTeleport; split_funcs["Time Travel 2"] = ::SplitCastleTeleport; split_funcs["Safe Code Entered"] = ::SplitSafeCode; split_funcs["Simon 1"] = ::SplitSimon; split_funcs["Simon 2"] = ::SplitSimon;
            split_funcs["Keeper Spawned"] = ::SplitKeeper; split_funcs["Keeper 1"] = ::SplitKeeper; split_funcs["Keeper 2"] = ::SplitKeeper; split_funcs["Keeper 3"] = ::SplitKeeper; split_funcs["Keeper 4"] = ::SplitKeeper; split_funcs["Keeper Trapped"] = ::SplitKeeper;
            split_funcs["Boss Enter"] = ::SplitCastleBoss; split_funcs["Boss Exit"] = ::SplitCastleBoss;
            break;
        }
        case "zm_island": {
            split_funcs["Skull 1"] = ::SplitSkullRitual; split_funcs["Skull 2"] = ::SplitSkullRitual; split_funcs["Skull 3"] = ::SplitSkullRitual; split_funcs["Skull 4"] = ::SplitSkullRitual; split_funcs["Skull Ritual"] = ::SplitSkullRitual;
            split_funcs["Bunker Open"] = ::SplitBunkerOpen; split_funcs["Power On"] = ::SplitPowerOn; split_funcs["KT-4"] = ::SplitKT; split_funcs["Masamune"] = ::SplitKT; split_funcs["Poster"] = ::SplitPoster;
            split_funcs["Bullet"] = ::SplitBullet; split_funcs["Plane Shot"] = ::SplitPlane; split_funcs["Elevator On"] = ::SplitElevator; split_funcs["Boss Enter"] = ::SplitIslandBossEnter;
            break;
        }
        case "zm_stalingrad": {
            split_funcs["Groph 1"] = ::SplitPAPGroph; split_funcs["Groph 2"] = ::SplitPAPGroph; split_funcs["Groph 3"] = ::SplitPAPGroph; split_funcs["Pickup Egg"] = ::SplitPickupEgg; split_funcs["Egg On Fire"] = ::SplitEggBathed; split_funcs["Incubation Start"] = ::SplitIncubationStart;
            split_funcs["Pickup Gauntlet"] = ::SplitGauntletPickup; split_funcs["Fly Supply"] = ::SplitFly; split_funcs["Fly Tank"] = ::SplitFly; split_funcs["Fly DC"] = ::SplitFly; split_funcs["Lockdown Start"] = ::SplitPAPLockdown;
            split_funcs["Lockdown End"] = ::SplitPAPLockdown; split_funcs["Start Challenges"] = ::SplitChallenges; split_funcs["Start Download"] = ::SplitDownload; split_funcs["Boss Enter"] = ::SplitStalingradBoss;
            break;
        }
        case "zm_genesis": {
            split_funcs["Spawn Gen"] = ::SplitEngine; split_funcs["DE Gen"] = ::SplitEngine; split_funcs["Verruckt Gen"] = ::SplitEngine; split_funcs["Mob Gen"] = ::SplitEngine; split_funcs["Keeper Start"] = ::SplitKeeperStart;
            split_funcs["Enter Beast"] = ::SplitApothicon; split_funcs["Exit Beast Mob"] = ::SplitApothicon; split_funcs["Exit Beast Verruckt"] = ::SplitApothicon; split_funcs["Exit Beast DE"] = ::SplitApothicon; split_funcs["Exit Beast Spawn"] = ::SplitApothicon;
            split_funcs["Pickup Reel 1"] = ::SplitReel; split_funcs["Place Reel 1"] = ::SplitReel; split_funcs["Pickup Reel 2"] = ::SplitReel; split_funcs["Place Reel 2"] = ::SplitReel; split_funcs["Pickup Reel 3"] = ::SplitReel; split_funcs["Place Reel 3"] = ::SplitReel;
            split_funcs["Sophia TP"] = ::SplitGenesisTP; split_funcs["Egg 1"] = ::SplitGenesisEgg; split_funcs["Egg 2"] = ::SplitGenesisEgg; split_funcs["Egg 3"] = ::SplitGenesisEgg; split_funcs["Egg 4"] = ::SplitGenesisEgg;
            split_funcs["Rune 1"] = ::SplitRune; split_funcs["Rune 2"] = ::SplitRune; split_funcs["Rune 3"] = ::SplitRune; split_funcs["Rune 4"] = ::SplitRune; split_funcs["Boss 1 Enter"] = ::SplitBoss1; split_funcs["Symbols"] = ::SplitSymbols;
            split_funcs["Basketball"] = ::SplitBossRush; split_funcs["Boss 2"] = ::SplitGenesisTP;
            break;
        }
        case "zm_tomb": {
            split_funcs["Ice Craft"] = ::SplitStaffCraft; split_funcs["Wind Craft"] = ::SplitStaffCraft; split_funcs["Fire Craft"] = ::SplitStaffCraft; split_funcs["Lightning Craft"] = ::SplitStaffCraft; split_funcs["Fire Enter"] = ::SplitTombTP;
            split_funcs["Fire Leave"] = ::SplitTombTP; split_funcs["Wind Enter"] = ::SplitTombTP; split_funcs["Wind Leave"] = ::SplitTombTP; split_funcs["Lightning Leave"] = ::SplitTombTP; split_funcs["Lightning Enter"] = ::SplitTombTP;
            split_funcs["Ice Leave"] = ::SplitTombTP; split_funcs["Ice Enter"] = ::SplitTombTP; split_funcs["4 Boxes Done"] = ::SplitBoxes; split_funcs["Fists Done"] = ::SplitFists;
            break;
        }
        default: {
            return;
        }
    }

    igt = Int(array::pop_front(tokens, 0));
    igrt = Int(array::pop_front(tokens, 0));
    level.num_splits = Int(array::pop_front(tokens, 0));
    split_type = Int(array::pop_front(tokens, 0));

    if (!GetDvarFloat("split_fontscale_mult", 0)) {
        SetDvar("split_fontscale_mult", 1.5);
    }

    if (igt) {
        thread RunIGT();
    }
    if (igrt) {
        thread RunIGRT();
    }

    splits = [];
    line = compiler::getline("Practice Tool/Autosplit Preset.txt", false);
    while (line != "n/a" && line != "end") {
        tokens = StrTok(line, ",");

        split = SpawnStruct();
        split.name = array::pop_front(tokens, 0);
        split.custom_name = array::pop_front(tokens, 0);
        split.round = Int(array::pop_front(tokens, 0));

        if (!IsSubStr(split.name, "Round") && !IsDefined(split_funcs[split.name])) {
            DebugPrint("error", "Split: " + split.name + " not found");
            line = compiler::getline("Practice Tool/Autosplit Preset.txt", false);
            continue;
        }

        if (IsDefined(split_funcs[split.name])) {
            split.split_func = split_funcs[split.name];
        }
        split.split_label = IString(split.custom_name + ": ");
        splits[splits.size] = split;

        line = compiler::getline("Practice Tool/Autosplit Preset.txt", false);
    }

    final_split = SpawnStruct();
    if (split_type == 2) {
        final_split.split_func = ::WaitPAP;
        final_split.split_label = &"PAP: ";
    }
    else if (split_type == 1) {
        final_split.split_func = ::WaitSong;
        final_split.split_label = &"Song Started: ";
    }
    else {
        switch (map) {
            case "zm_zod": {
                final_split.split_func = ::SplitZodEnd;
                break;
            }
            case "zm_factory": {
                final_split.split_func = ::SplitFactoryEnd;
                break;
            }
            case "zm_castle": {
                final_split.split_func = ::SplitCastleEnd;
                break;
            }
            case "zm_island": {
                final_split.split_func = ::SplitIslandEnd;
                break;
            }
            case "zm_stalingrad": {
                final_split.split_func = ::SplitStalingradEnd;
                break;
            }
            case "zm_genesis": {
                final_split.split_func = ::SplitGenesisEnd;
                break;
            }
            case "zm_tomb": {
                final_split.split_func = ::SplitTombEnd;
                break;
            }
            default: {
                break;
            }
        }
        final_split.split_label = &"Egg End: ";
    }

    final_split.round = -1;
    splits[splits.size] = final_split;

    level.in_game_splits = [];
    level.rendered_splits = [];
    level.split_top = 0;
    level.split_bottom = 8;
    level.last_split_added = 0;
    level.current_split_num = 0;

    if (igt) {
        thread MonitorSplitLayoutChange();
        for (i = 0; i < 8; i++) {
            level.rendered_splits[i] = InitHud(0);
            InitSplitHud(level.rendered_splits[i], 1);
            if (igrt) {
                level.rendered_splits[i].y += (10 * level.rendered_splits[i].fontScale);
            }
        }
    }

    thread Split(0);
    WaitFadeIn();
    thread Split();
    level notify("end_server");
    SetDvar("sv_cheats", 0);
    start_time = GetTime();

    foreach (index, split in splits) {
        if (igt) {
            AddIGTSplit(split.split_label);
        }
        if (IsDefined(split.split_func)) {
            if (split == final_split) {
                [[ split.split_func ]]();
            }
            else {
                [[ split.split_func ]](split.name);
            }
        }
        WaitForRound(split.round);
        thread Split();
        level.in_game_splits[level.last_split_added - 1].text = "^2" + CalcTime(GetTime() - start_time);
        level.in_game_splits[level.last_split_added - 1].time = CalcTime(GetTime() - start_time);
    }

    RenderSplits();
}

RunIGT()
{
    WaitFadeIn();
    timer = InitHud(0);
    timer SetTimerUp(0);
    timer.fontScale = GetDvarFloat("split_fontscale_mult", 1.5);
    timer.label = &"^3Time: ^2";
    timer.x = 15;
    timer.y = 15;
    timer.alpha = 1;

    for (;;) {
        level waittill("split_font_size_change", new_fontScale);
        timer.fontScale = new_fontScale;
    }
}

RunIGRT()
{
    WaitFadeIn();
    round_timer = InitHud(0);
    round_timer SetTenthsTimerUp(0.05);
    round_timer.fontScale = GetDvarFloat("split_fontscale_mult", 1.5);
    round_timer.label = &"^3Round Time: ^2";
    round_timer.x = 15;
    round_timer.y = 15 + (10 * round_timer.fontScale);
    round_timer.alpha = 1;
    thread ManageInGameRoundTimer(round_timer);

    for (;;) {
        level waittill("split_font_size_change", new_fontScale);
        round_timer.fontScale = new_fontScale;
        round_timer.y = 15 + (10 * round_timer.fontScale);
    }
}

ManageInGameRoundTimer(round_timer)
{
    for (;;) {
        level waittill("end_of_round");
        round_timer SetTenthsTimerUp(0.05);
    }
}

WaitForRound(round)
{
    level flag::wait_till("begin_spawning");

    if (round < 0) {
        return;
    }
    if (!round) {
        level waittill("end_of_round");
        return;
    }

    if (round == 1) {
        level waittill("end_of_round");
    }

    while (level.round_number < round) {
        level waittill("end_of_round");
    }
}

AddIGTSplit(label)
{
    level.in_game_splits[level.last_split_added] = SpawnStruct();
    level.in_game_splits[level.last_split_added].label = label;
    level.in_game_splits[level.last_split_added].text = "";
    level.in_game_splits[level.last_split_added].time = "";

    level.last_split_added++;

    if (level.last_split_added > level.split_bottom) {
        diff = level.last_split_added - level.split_bottom;
        level.split_top += diff;
        level.split_bottom += diff;
    }

    RenderSplits();
}

MonitorSplitLayoutChange()
{
    thread SplitMoveUp();
    thread SplitMoveDown();
    thread SplitHide();
    thread SplitFontSizeUp();
    thread SplitFontSizeDown();
    thread ExportSplits();
}

Split(force_num)
{
    if (IsDefined(force_num)) {
        SetDvar("probation_league_matchHistoryWindow", force_num);
        return;
    }
    SetDvar("split_count", GetDvarInt("split_count", 0) + 1);
    level.current_split_num++;
    SetDvar("probation_league_matchHistoryWindow", level.current_split_num);
}

SplitMoveUp()
{
    for (;;) {
        WaitPgUp();
        if (!level.split_top) {
            continue;
        }
        level.split_top--;
        level.split_bottom--;
        RenderSplits();
    }
}

SplitMoveDown()
{
    for (;;) {
        WaitPgDown();
        if (level.last_split_added <= level.split_bottom || level.split_bottom == level.num_splits) {
            continue;
        }
        level.split_top++;
        level.split_bottom++;
        RenderSplits();
    }
}

SplitFontSizeUp()
{
    for (;;) {
        WaitPlus();
        fontScale = GetDvarFloat("split_fontscale_mult", 1.5);
        if (fontScale >= 2) {
            continue;
        }
        fontScale += 0.25;
        SetDvar("split_fontscale_mult", fontScale);
        level notify("split_font_size_change", fontScale);
        UpdateSplitFontSize(fontScale);
    }
}

SplitFontSizeDown()
{
    for (;;) {
        WaitMinus();
        fontScale = GetDvarFloat("split_fontscale_mult", 1.5);
        if (fontScale <= 1) {
            continue;
        }
        fontScale -= 0.25;
        SetDvar("split_fontscale_mult", fontScale);
        level notify("split_font_size_change", fontScale);
        UpdateSplitFontSize(fontScale);
    }
}

SplitHide()
{
    for (;;) {
        WaitEnd();
        foreach (rendered_split in level.rendered_splits) {
            rendered_split.alpha = 0;
        }
        WaitEnd();
        foreach (rendered_split in level.rendered_splits) {
            rendered_split.alpha = 1;
        }
    }
}

RenderSplits()
{
    for (i = level.split_top; i < level.split_bottom; i++) {
        if (i - level.split_top >= level.last_split_added) {
            return;
        }
        level.rendered_splits[i - level.split_top].label = level.in_game_splits[i].label;
        level.rendered_splits[i - level.split_top] SetText(level.in_game_splits[i].text);
    }
}

UpdateSplitFontSize(new_fontScale)
{
    foreach (index, split in level.rendered_splits) {
        old_y = split.y;
        difference = (10 * new_fontScale) - (10 * split.fontScale);
        split.y += difference + ((index + 1) * difference); 
        split.fontScale = new_fontScale;
    }
}

ExportSplits()
{
    for (;;) {
        WaitHome();
        data = "";
        foreach (split in level.in_game_splits) {
            if (split.text == "") {
                break;
            }
            data += MakeLocalizedString(split.label) + split.time + "\n";
        }
        compiler::setclipboard(data);
    }
}

PrintSplitsResetLobby()
{
    WaitFadeIn();
    IPrintLnBold("Split count reached 1000+ this session. Reset the lobby to avoid a crash in the near future");
}

PrintSplitTutorial()
{
    WaitFadeIn();
    if (!IsTrue(level.num_splits)) {
        return;
    }
    SetDvar("split_tutorial", 1);
    message = hud::createserverfontstring("big", 1.1);
    message hud::setpoint("TOP", "TOP", 0, 0);
    wait 10;
    message SetText("^3Autosplit Controls^7: ^9PgUp^7: Scroll up, ^9PgDown^7: Scroll down, ^9End^7: Hide splits\n^9Plus^7: Increase font size, ^9Minus^7: Decrease font size, ^9Home^7: Copy splits to clipboard");
    wait 5;
    message FadeOverTime(5);
    message.alpha = 0;
    wait 5;
    message Destroy();
}

InitSplitHud(hud_elem, alpha = 0)
{
    hud_elem.fontScale = GetDvarFloat("split_fontscale_mult", 0);
    hud_elem.x = 15;
    hud_elem.y = 15 + ((level.rendered_splits.size) * (10 * hud_elem.fontScale));
    hud_elem.alpha = alpha;
}

WaitPAP()
{
    foreach (trigger in zm_pap_util::get_triggers()) {
        trigger thread WatchPAPTrigger();
    }
    level waittill("pap_taken");
}

WatchPAPTrigger()
{
    level endon("pap_taken");
    self waittill("pap_taken");
    level notify("pap_taken");
}

WaitSong()
{
    level endon("plant_lullaby");
    if (level.mapname == "zm_island") {
        thread WatchLullaby();
    }
    songs = Array("snakeskinboots", "snakeskinboots_instr", "coldhardcash", "musicEasterEgg", "dead_again", "dead_flowers", "dead_ended", "ace_of_spades", "the_gift", "lullaby_for_a_dead_man", "the_one", "115", "abracadavre", "pareidolia",
    "cominghome", "archangel", "aether", "shepherd_of_fire");
    for (;;) {
        while (!IsTrue(level.musicsystem.currentstate)) {
            wait 0.05;
        }
        if (IsInArray(songs, level.musicsystem.currentstate)) {
            return;
        }
        state = level.musicsystem.currentstate;
        while (level.musicsystem.currentstate == state) {
            wait 0.05;
        }
    }
}

WatchLullaby()
{
    play = struct::get("plantMusicPlay", "targetname");
    pods = GetEntArray("plantMusicPods", "targetname");
    solution = Array(1, 3, 5, 6, 7, 5);
    for (;;)
    {
        play waittill("trigger_activated");
        water_levels = [[ @zm_island_side_ee_song<scripts\zm\zm_island_side_ee_song.gsc>::function_c5359566 ]]();
        match = 1;
		for (i = 0; i < water_levels.size; i++) {
			if (water_levels[i] != solution[i]) {
				match = 0;
				break;
			}
            if (IsTrue(match)) {
                level notify("plant_lullaby");
                return;
            }
		}
    }
}