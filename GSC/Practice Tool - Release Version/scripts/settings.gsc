LoadSettings()
{
    InitMenuFuncs();
    InitPracticePatches();
    self thread LoadGums();
    self thread LoadPracticePatches();

    self thread WatchInterface();
}

InitMenuFuncs()
{
    /*
        0 = Player Options
        1 = Zombie Options
        2 = Round Options
        3 = Powerup Options
        4 = Egg Step Options
        5 = Craftable Options
        6 = Blocker Options
    */
    level.menu_functions = [];
    level.menu_functions[0] = Array(::SetPlayerPoints, ::GivePlayerWeapon, ::TakePlayerWeapon, ::GivePlayerGum, ::TakePlayerGum, ::TakePlayerGumCharge, ::UsePlayerGumCharge, ::GiveAllPerks, ::TakeAllPerks, ::GivePerk, ::TakePerk, ::Godmode, ::InfiniteAmmo, ::Timescale);
    level.menu_functions[1] = Array(::IgnorePlayers, ::KillHorde, ::FreezeZombies, ::ClearSpawns, ::SetZombieSpeed);
    level.menu_functions[2] = Array(::EndRound, ::SkipToRound, ::SetZombieCount);
    level.menu_functions[3] = Array(::GivePowerup);
    level.egg_funcs["zm_zod"] = Array(::FinishRitual, ::FillAllEggs, ::FillEgg, ::FinishAllMagicCircles, ::FinishMagicCircle, ::FinishAllFlags, ::FinishFlag);
    level.menu_functions[4] = level.egg_funcs[level.script];
    level.menu_functions[5] = Array(::PickupAllPieces, ::ProcessPiecePickup);
    level.menu_functions[6] = Array(::OpenAllDoors, ::OpenSpecificDoor, ::ActivateAllPower, ::ActivateSpecificPower, ::ActivateMapSpecificObject, ::SetZBarrierState);
    level.menu_functions[7] = Array(::LoadMap);
}

InitPracticePatches()
{
    level.practice_patches = Array();
    level.practice_patches["zm_zod"] = Array(::ZodSoftPatch, ::InfiniteBeastStart, ::OvumsPractice, ::FlagsPractice);
    level.practice_patches["zm_factory"] = Array(::FlytrapPractice);
    level.practice_patches["zm_castle"] = Array(::CastleSoftPatch, ::Wisp2Practice, ::KeeperPractice, ::LightningBowShotsPractice, ::FireBowShotsPractice);
    level.practice_patches["zm_island"] = Array(::IslandSoftPatch);
    level.practice_patches["zm_stalingrad"] = Array(::StalingradSoftPatch, ::LockdownPractice, ::ChallengesPractice, ::GershQuoteSkipPractice, ::BossQuoteSkipPractice);
    level.practice_patches["zm_genesis"] = Array(::GenesisSoftPatch, ::BonesPractice, ::Boss1Practice, ::Boss2Practice, ::BasketballPractice, ::SquidShardsPractice);
    level.practice_patches["zm_moon"] = Array();
    level.practice_patches["zm_tomb"] = Array(::WindOrbPracticeShots);
}

WatchInterface()
{
    level endon("disconnect");

    file = "Practice Tool/Tool-Game Interface.txt";
    compiler::resetfile(file);
    level flag::wait_till("initial_blackscreen_passed");
    compiler::resetfile(file);
    for(;;)
    {
        wait 0.05;
        array_num = compiler::readsettings(file, 1, 0);
        if(array_num < 0 || !IsDefined(level.menu_functions[array_num])) continue;
        func_num = compiler::readsettings(file, 2, 0);
        if(func_num < 0 || !IsDefined(level.menu_functions[array_num][func_num])) continue;
        params = [];
        param = 0;
        num = 3;
        param = compiler::readsettings(file, num, 0);
        while(param >= 0)
        {
            params[params.size] = param;
            num++;
            param = compiler::readsettings(file, num, 0);
        }
        func = level.menu_functions[array_num][func_num];
        switch(params.size)
        {
            case 0:
                self thread [[func]]();
                break;
            case 1:
                self thread [[func]](params[0]);
                break;
            case 2:
                self thread [[func]](params[0], params[1]);
                break;
            case 3:
                self thread [[func]](params[0], params[1], params[2]);
                break;
        }
        compiler::resetfile(file);
    }
}

LoadGums()
{
    file = "Practice Tool/Settings/Active Gum Preset.txt";
    gum_nums = [];
    gum_nums[gum_nums.size] = compiler::readsettings(file, 1, 0);
    gum_nums[gum_nums.size] = compiler::readsettings(file, 2, 0);
    gum_nums[gum_nums.size] = compiler::readsettings(file, 3, 0);
    gum_nums[gum_nums.size] = compiler::readsettings(file, 4, 0);
    gum_nums[gum_nums.size] = compiler::readsettings(file, 5, 0);
    gum_pack = [];
    if(!gum_nums.size)
    {
        return;
    }
    foreach(num in gum_nums)
    {
        if(num < 0) return;
        gum_pack[gum_pack.size] = level.custom_bgb_list[num];
    }
    self.bgb_pack = gum_pack;
    level.players[0].bgb_pack = gum_pack;
    pack_string = "";
    foreach(index, gum in gum_pack)
    {
        tokens = StrToK(gum, "_");
        for(i = 2; i < tokens.size; i++)
        {
            pack_string += ToUpper(GetSubStr(tokens[i], 0, 1)) + GetSubStr(tokens[i], 1);
            if(i != tokens.size - 1) pack_string += " ";
        }
        if(index != gum_pack.size - 1) pack_string += " - ";
    }
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Custom BGB Loadout Active");
    thread WriteToScreen("Loadout: " + pack_string);
}

LoadPracticePatches()
{
    file = "Practice Tool/Settings/Practice Presets.txt";
    practice_patch = -1;
    switch(level.script)
    {
        case "zm_zod":
            practice_patch = compiler::readsettings(file, 1, 0);
            break;
        case "zm_factory":
            practice_patch = compiler::readsettings(file, 2, 0);
            break;
        case "zm_castle":
            practice_patch = compiler::readsettings(file, 3, 0);
            break;
        case "zm_island":
            practice_patch = compiler::readsettings(file, 4, 0);
            break;
        case "zm_stalingrad":
            practice_patch = compiler::readsettings(file, 5, 0);
            break;
        case "zm_genesis":
            practice_patch = compiler::readsettings(file, 6, 0);
            break;
        case "zm_moon":
            practice_patch = compiler::readsettings(file, 7, 0);
            break;
        case "zm_tomb":
            practice_patch = compiler::readsettings(file, 8, 0);
            break;
    }
    if(practice_patch < 0) return;
    self thread [[level.practice_patches[level.script][practice_patch]]]();
}