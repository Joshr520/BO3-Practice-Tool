LoadSplits()
{
    if(!compiler::loadsplitjson()) return;
    maps = ["zm_zod", "zm_factory", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_prototype", "zm_asylum", "zm_sumpf", "zm_theater", "zm_cosmodrome", "zm_temple", "zm_moon", "zm_tomb"];
    map = compiler::getsplitvalue("Settings", "Map Index");

    split_size = compiler::getsplitvalue("Settings", "Amount of Splits");
    split_type = compiler::getsplitvalue("Settings", "Split Type");
    igt = compiler::getsplitvalue("Settings", "In Game Timer");
    igrt = compiler::getsplitvalue("Settings", "In Game Round Timer");

    if(igt) thread RunIGT();
    if(igrt) thread RunIGRT();

    if(maps[map] != level.script) return;

    level.num_splits = split_size + 1;

    split_funcs = [];
    split_labels = [];
    split_funcs["zm_zod"] = Array(::SplitMagicianRitual, ::SplitFemmeRitual, ::SplitDetectoveRitual, ::SplitBoxerRitual, ::SplitPAPRitual, ::SplitCanalsRift, ::SplitFootlightRift, ::SplitWaterfrontRift, ::SplitRiftCanals, ::SplitRiftFootlight,
    ::SplitRiftWaterfront, ::SplitCanalsEgg, ::SplitFootlightEgg, ::SplitWaterfrontEgg, ::SplitRiftEgg, ::SplitPickupSword, ::SplitJunctionOvum, ::SplitCanalsOvum, ::SplitFootlightOvum, ::SplitWaterfrontOvum, ::SplitUpgradedSword, ::SplitBook,
    ::SplitFlag1, ::SplitFlag2, ::SplitFlag3, ::SplitFlag4);
    split_funcs["zm_castle"] = Array(::SplitChurchDragon, ::SplitCourtyardDragon, ::SplitUndercroftDragon, ::SplitBow, ::SplitStartLightning, ::SplitBonfires, ::SplitWallRide, ::SplitCrackle, ::SplitUpgradeLightning, ::SplitStartFire, ::SplitObelisk,
    ::SplitCircles, ::SplitGolf, ::SplitFireUpgrade, ::SplitVoidStart, ::SplitActivateUrn, ::SplitSkulls, ::SplitCrawlers, ::SplitRunes, ::SplitVoidUpgrade, ::SplitWolfStart, ::SplitShrine, ::SplitStartEscort, ::SplitEscortFilled, ::SplitWolfForged,
    ::SplitWolfUpgrade, ::SplitTP, ::SplitTimeTravel1, ::SplitTimeTravel2, ::SplitCodeEntered, ::SplitSimon1, ::SplitSimon2, ::SplitKeeperSpawned, ::SplitKeeper1, ::SplitKeeper2, ::SplitKeeper3, ::SplitKeeper4, ::SplitKeeperTrapped, ::SplitBossEnter, ::SplitBossExit);
    split_funcs["zm_island"] = Array(::SplitSkull1, ::SplitSkull2, ::SplitSkull3, ::SplitSkull4, ::SplitSkullWeapon, ::SplitBunkerOpen, ::SplitPowerOn, ::SplitKTPickup, ::SplitMasamunePickup, ::SplitPoster, ::SplitBullet, ::SplitPlane, ::SplitElevator, ::SplitIslandBossEnter);
    split_funcs["zm_stalingrad"] = Array(::SplitGroph1, ::SplitGroph2, ::SplitGroph3, ::SplitPickupEgg, ::SplitEggBathed, ::SplitIncubationStart, ::SplitPickupGauntlet, ::SplitFlySupply, ::SplitFlyTank, ::SplitFlyDC, ::SplitLockdownStart, ::SplitLockdownEnd,
    ::SplitStartChallenges, ::SplitStartDownload, ::SplitStalingradBossEnter);
    split_funcs["zm_genesis"] = Array(::SplitGenSpawn, ::SplitGenDE, ::SplitGenVerruckt, ::SplitGenMob, ::SplitKeeperStart, ::SplitEnterBeast, ::SplitLeaveBeastMob, ::SplitLeaveBeastVerruckt, ::SplitLeaveBeastDE, ::SplitLeaveBeastSpawn, ::SplitReelPickup1,
    ::SplitReelPlace1, ::SplitReelPickup2, ::SplitReelPlace2, ::SplitReelPickup3, ::SplitReelPlace3, ::SplitHouseTP, ::SplitEgg1, ::SplitEgg2, ::SplitEgg3, ::SplitEgg4, ::SplitRune1, ::SplitRune2, ::SplitRune3, ::SplitRune4, ::SplitBoss1, ::SplitSymbols,
    ::SplitBossRush, ::SplitBoss2);
    split_funcs["zm_tomb"] = Array(::SplitIceCraft, ::SplitWindCraft, ::SplitFireCraft, ::SplitLightningCraft, ::SplitUpgrade, ::SplitBoxes, ::SplitFists);
    split_labels["zm_zod"] = Array(&"Magician Ritual: ", &"Femme Ritual: ", &"Detective Ritual: ", &"Boxer Ritual: ", &"PAP Ritual: ", &"Canals Rift: ", &"Footlight Rift: ", &"Waterfront Rift: ", &"Rift Canals: ", &"Rift Footlight: ", &"Rift Waterfront: ",
    &"Canals Egg: ", &"Footlight Egg: ", &"Waterfront Egg: ", &"Rift Egg: ", &"Sword: ", &"Junction Ovum: ", &"Canals Ovum: ", &"Footlight Ovum: ", &"Waterfront Ovum: ", &"Upgraded Sword: ", &"Book: ", &"Flag 1: ", &"Flag 2: ", &"Flag 3: ", &"Flag 4: ");
    split_labels["zm_castle"] = Array(&"Church Dragon: ", &"Courtyard Dragon: ", &"Undercroft Dragon: ", &"Pickup Bow: ", &"Start Lightning: ", &"Bonfires Shot: ", &"Wall Ride: ", &"Crackle: ", &"Upgrade Lightning: ", &"Start Fire: ", &"Obelisk Shot: ",
     &"Circles Filled: ", &"Golf: ", &"Upgrade Fire: ", &"Start Void: ", &"Activate Urn: ", &"Pickup Skulls: ", &"Crawler Kills: ", &"Runes: ", &"Upgrade Void: ", &"Start Wofl: ", &"Shrine Shot: ", &"Start Escort: ", &"Wolf Souls Filled: ", &"Wolf Arrow Forged: ",
     &"Upgrade Wolf: ", &"Normal TP: ", &"Time Travel 1: ", &"Time Travel 2: ", &"Safe Code Entered: ", &"Simon 1: ", &"Simon 2: ", &"Keeper Spawned: ", &"Keeper 1: ", &"Keeper 2: ", &"Keeper 3: ", &"Keeper 4: ", &"Keeper Trapped: ", &"Boss Enter: ", &"Boss Exit: ");
    split_labels["zm_island"] = Array(&"Skull 1: ", &"Skull 2: ", &"Skull 3: ", &"Skull 4: ", &"Skull Ritual: ", &"Bunker Open: ", &"Power On: ", &"KT-4: ", &"Masamune: ", &"Poster: ", &"Bullet: ", &"Plane Shot: ", &"Elevator On: ", &"Boss Enter: ");
    split_labels["zm_stalingrad"] = Array(&"Groph 1: ", &"Groph 2: ", &"Groph 3: ", &"Pickup Egg: ", &"Egg Bathed: ", &"Incubation Start: ", &"Pickup Gauntlet: ", &"Fly Supply: ", &"Fly Tank: ", &"Fly DC: ", &"Lockdown Start: ", &"Lockdown End: ",
    &"Start Challenges: ", &"Start Download: ", &"Boss Enter: ");
    split_labels["zm_genesis"] = Array(&"Spawn Gen: ", &"DE Gen: ", &"Verruckt Gen: ", &"Mob Gen: ", &"Keeper Start: ", &"Enter Beast: ", &"Exit Beast Mob: ", &"Exit Beast Verruckt: ", &"Exit Beast DE: ", &"Exit Beast Spawn: ", &"Pickup Reel 1: ",
    &"Place Reel 1: ", &"Pickup Reel 2: ", &"Place Reel 2: ", &"Pickup Reel 3: ", &"Place Reel 3: ", &"House: ", &"Egg 1: ", &"Egg 2: ", &"Egg 3: ", &"Egg 4: ", &"Rune 1: ", &"Rune 2: ", &"Rune 3: ", &"Rune 4: ",
    &"Boss 1: ", &"Symbols: ", &"Basketball: ", &"Boss 2: ");
    split_labels["zm_tomb"] = Array(&"Ice Staff: ", &"Wind Staff: ", &"Fire Staff: ", &"Lightning Staff: ", &"Upgrade: ", &"Boxes: ", &"Fists: ");
    active_funcs = [];
    active_labels = [];
    round_funcs = [];
    round_params = [];

    for(i = 0; i < split_size; i++)
    {
        func_num = compiler::getsplitvalue("Split Data", "Key", i);
        round = compiler::getsplitvalue("Split Data", "Value", i);

        if(!IsDefined(split_funcs[level.script][func_num])) return;
        if(!IsDefined(level.tracking_keeper) && IsSubStr(MakeLocalizedString(split_labels[level.script][func_num]), "Keeper")) thread TrackKeeper();

        active_funcs[active_funcs.size] = split_funcs[level.script][func_num];
        active_labels[active_labels.size] = split_labels[level.script][func_num];
        if(!round) round_funcs[i] = ::WaitSplitCurrentRound;
        else if(round > 0)
        {
            round_funcs[i] = ::WaitSplitRound;
            round_params[i] = round;
        }
    }

    if(split_type == 2)
    {
        active_funcs[active_funcs.size] = ::WaitPAP;
        active_labels[active_labels.size] = &"PAP: ";
    }
    else if(split_type == 1)
    {
        active_funcs[active_funcs.size] = ::WaitSong;
        active_labels[active_labels.size] = &"Song Started: ";
    }
    else
    {
        switch(maps[map])
        {
            case "zm_zod":
                active_funcs[active_funcs.size] = ::WaitZodEnd;
                break;
            case "zm_castle":
                active_funcs[active_funcs.size] = ::WaitCastleEnd;
                break;
            case "zm_island":
                active_funcs[active_funcs.size] = ::WaitIslandEnd;
                break;
            case "zm_stalingrad":
                active_funcs[active_funcs.size] = ::WaitStalingradEnd;
                thread MonitorLockdownTimer();
                break;
            case "zm_genesis":
                active_funcs[active_funcs.size] = ::WaitGenesisEnd;
                break;
            case "zm_tomb":
                active_funcs[active_funcs.size] = ::WaitTombEnd;
                break;
            default:
                break;
        }
        active_labels[active_labels.size] = &"Egg End: ";
    }

    level.in_game_splits = [];
    level.rendered_splits = [];
    level.split_top = 0;
    level.split_bottom = 8;
    level.last_split_added = 0;

    if(igt)
    {
        thread MonitorSplitLayoutChange();
        for(i = 0; i < 8; i++)
        {
            level.rendered_splits[i] = InitHud(0);
            InitSplitHud(level.rendered_splits[i], 1);
            if(igrt) level.rendered_splits[i].y += 15;
        }
    }

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
    timer.label = &"^3Time: ^2";
    timer.x = 15;
    timer.y = 15;
    timer.alpha = 1;
}

RunIGRT()
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
    level.in_game_splits[level.last_split_added].time = "";

    level.last_split_added++;

    if(level.last_split_added > level.split_bottom)
    {
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
    thread ExportSplits();
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

SplitHide()
{
    for(;;)
    {
        WaitEnd();
        foreach(rendered_split in level.rendered_splits) rendered_split.alpha = 0;
        WaitEnd();
        foreach(rendered_split in level.rendered_splits) rendered_split.alpha = 1;
    }
}

ExportSplits()
{
    for(;;)
    {
        WaitHome();
        data = "";
        foreach(split in level.in_game_splits)
        {
            if(split.text == "") break;
            data += MakeLocalizedString(split.label) + split.time + "\n";
        }
        compiler::setclipboard(data);
    }
}

RenderSplits()
{
    for(i = level.split_top; i < level.split_bottom; i++)
    {
        if(i - level.split_top >= level.last_split_added) return;
        level.rendered_splits[i - level.split_top].label = level.in_game_splits[i].label;
        level.rendered_splits[i - level.split_top] SetText(level.in_game_splits[i].text);
    }
}

InitSplitHud(hud_elem, alpha = 0)
{
    hud_elem.x = 15;
    hud_elem.y = (level.rendered_splits.size + 1) * 15;
    hud_elem.alpha = alpha;
}

Split()
{
    if(!IsDefined(level.current_split_num)) level.current_split_num = 0;
    SetDvar("probation_league_matchHistoryWindow", level.current_split_num);
    level.current_split_num++;
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

WaitPAP()
{
    self waittill("pap_taken");
}

WaitSong()
{
    level endon("plant_lullaby");
    if(level.script == "zm_island") thread WatchLullaby();
    songs = Array("snakeskinboots", "snakeskinboots_instr", "coldhardcash", "musicEasterEgg", "dead_again", "dead_flowers", "dead_ended", "ace_of_spades", "the_gift", "lullaby_for_a_dead_man", "the_one", "115", "abracadavre", "pareidolia",
    "cominghome", "archangel", "aether", "shepherd_of_fire");
    for(;;)
    {
        while(!IsTrue(level.musicsystem.currentstate)) wait 0.05;
        if(IsInArray(songs, level.musicsystem.currentstate)) return;
        state = level.musicsystem.currentstate;
        while(level.musicsystem.currentstate == state) wait 0.05;
    }
}

WatchLullaby()
{
    play = struct::get("plantMusicPlay", "targetname");
    pods = GetEntArray("plantMusicPods", "targetname");
    solution = array(1, 3, 5, 6, 7, 5);
    for(;;)
    {
        play waittill("trigger_activated");
        water_levels = [[ @zm_island_side_ee_song<scripts\zm\zm_island_side_ee_song.gsc>::function_c5359566 ]]();
        match = 1;
		for(i = 0; i < water_levels.size; i++)
		{
			if(water_levels[i] != solution[i])
			{
				match = 0;
				break;
			}
            if(IsTrue(match))
            {
                level notify("plant_lullaby");
                return;
            }
            else
            {
                for(i = 0; i < pods.size; i++)
                {
                    pods[i] playsound("mus_podegg_note_" + pods[i].var_b3a7fe6c);
                    wait(0.6);
                }
            }
		}
    }
}