GenesisBossPractice()
{
    if (level.mapname != "zm_genesis") {
        DebugPrint("error", "Boss Practice attempted to load on " + level.mapDisplayName + " but it requires Revelations");
        return;
    }
    RegisterHotkey(0x74, "Start Boss 1: ^9F5", undefined, ::StartBoss1);
    RegisterHotkey(0x75, "Start Boss 2: ^9F6", undefined, ::StartBoss2);
    level.display_messages = false;
    level.var_6000c357 = Array(0, 1, 2, 3);
    level flag::set("boss_practice");
    level flag::init("boss_active");

    WaitFadeIn();
    level.players[0] thread OpenAllDoors();
    level.players[0] thread ActivateAllPower();
    foreach (player in GetPlayers()) {
        player.score = 50000;
        player GiveAllPerks();
        foreach (weapon in player GetWeaponsListPrimaries()) {
            player TakeWeapon(weapon);
        }
        player GivePlayerWeapon("shotgun_pump", true);
        player GivePlayerWeapon("ar_cqb", true);
        if (player IsHost()) {
            player GivePlayerWeapon("idgun_", true);
        }
        player thread GivePerksAndRevive();
    }

    level flag::wait_till("initial_blackscreen_passed");
    level.display_messages = true;
    thread WriteToScreen("Boss Practice Loaded");
    level flag::clear("spawn_zombies");
}

StartBoss1()
{
    if (level flag::get("boss_active")) {
        thread WriteToScreen("Boss already in progress");
        return;
    }
    level flag::set("boss_active");
    level flag::clear("final_boss_started");
    TrueMaxAmmo();
    level.var_6000c357 = array::randomize(level.var_6000c357);
    level [[ @zm_genesis_ee_quest<scripts\zm\zm_genesis_ee_quest.gsc>::function_1bb1bf93 ]]();
    level flag::clear("boss_active");
}

StartBoss2()
{
    if (level flag::get("boss_active")) {
        thread WriteToScreen("Boss already in progress");
        return;
    }
    level flag::set("boss_active");
    TrueMaxAmmo();
    for(i = 0; i < level.activeplayers.size; i++) {
		level.activeplayers[i] thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_56668973 ]](struct::get_array("dark_arena_teleport_hijack", "targetname")[i]);
	}
    [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_386f30f4 ]]();
    level flag::clear("boss_active");
}