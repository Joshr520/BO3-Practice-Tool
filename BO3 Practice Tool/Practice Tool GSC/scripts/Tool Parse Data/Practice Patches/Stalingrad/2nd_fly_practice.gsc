Fly2Practice()
{
    if (level.mapname != "zm_stalingrad") {
        DebugPrint("error", "2nd Fly Practice attempted to load on " + level.mapDisplayName + " but it requires Gorod Krovi");
        return;
    }
    level flag::set("stalingrad_2nd_fly");
    level flag::clear("spawn_zombies");
    level.display_messages = false;

    WaitFadein();

    valves = GetEntArray("ee_tube_terminal", "targetname");
    messages = [];
    messages["Dept"] = valves[2] == level.var_57f8b6c5[5] ? "P" : valves[2].var_1f3c0ca7 + 1;
    messages["Dragon"] = valves[0] == level.var_57f8b6c5[5] ? "P" : valves[0].var_1f3c0ca7 + 1;
    messages["Arm"] = valves[5] == level.var_57f8b6c5[5] ? "P" : valves[5].var_1f3c0ca7 + 1;
    messages["Supply"] = valves[1] == level.var_57f8b6c5[5] ? "P" : valves[1].var_1f3c0ca7 + 1;
    messages["Inf"] = valves[3] == level.var_57f8b6c5[5] ? "P" : valves[3].var_1f3c0ca7 + 1;
    messages["Tank"] = valves[4] == level.var_57f8b6c5[5] ? "P" : valves[4].var_1f3c0ca7 + 1;
    message = "";
    foreach (index, msg in messages) {
        message += msg;
        if (index != "Tank") {
            message += "\t";
        }
    }
    IPrintLnBold("Valves copied to clipboard");
    compiler::setclipboard(message);

    self.score = 5000;
    self.bgb_pack_randomized = Array("zm_bgb_shopping_free", "zm_bgb_anywhere_but_here", "zm_bgb_reign_drops", "zm_bgb_anywhere_but_here");
    self GiveAllPerks();
    foreach (weapon in self GetWeaponsListPrimaries()) {
        self TakeWeapon(weapon);
    }
    self GivePlayerWeapon("raygun_mark3", false);
    self GivePlayerWeapon("ar_cqb", false);
    self GivePlayerWeapon("shotgun_precision", false);
    self GivePlayerWeapon("shield", false);
    self bgb::give("zm_bgb_dead_of_nuclear_winter");
    self OpenAllDoors();
    self ActivateAllPower();
    self thread BuildDragonride();
    thread IncreaseDragonOdds();
    thread ZombieCounter();
    self GetDragonEgg();
    self ActivateEggNest(2);
    level flag::set("egg_bathed_in_flame");
    level flag::set("egg_cooled_hazard");
    self SetOrigin(struct::get_array("egg_cook_loc")[2].origin);
    level.display_messages = true;

    IPrintLnBold("Press Enter to continue");
    WaitEnter();
    GotoRound(7);
    ActivateTrophy("935");
    PickupTrophy("gersh");
    PickupTrophy("mangler");
    level flag::set("spawn_zombies");
    level.disable_nuke_delay_spawning = 1;
    for (i = 39; i <= 45; i++) {
        barrier = level.exterior_goals[i];
        if(IsDefined(barrier.zbarrier)) {
			for(x = 0; x < barrier.zbarrier GetNumZBarrierPieces(); x++) {
				barrier.zbarrier SetZBarrierPieceState(x, "opening");
			}
		}
		if(IsDefined(barrier.clip)) {
			barrier.clip TriggerEnable(0);
			barrier.clip ConnectPaths();
			continue;
		}
    }
}