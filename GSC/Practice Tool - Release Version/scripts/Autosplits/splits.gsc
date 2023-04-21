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

StartSplits()
{

    // Check for fade in to split
    SetDvar("xenon_maxVoicePacketsPerSec", 0);
    self StartOnFadeIn();
    // Check if round splits are enabled, else, start egg splits based on the map
    if(level.round_sr) SplitOnRoundEnd();
    else if(level.pap_sr) self PAPSplit();
    else
    {
        switch(level.script)
        {
            case "zm_zod":
                self ZodEggSplits();
                break;
            case "zm_factory":
                self FactoryEggSplits();
                break;
            case "zm_castle":
                self CastleEggSplits();
                break;
            case "zm_island":
                self IslandEggSplits();
                break;
            case "zm_stalingrad":
                self StalingradEggSplits();
                break;
            case "zm_genesis":
                self GenesisEggSplits();
                break;
        }
    }
}

PAPSplit()
{
    while(!self HasWeapon("pistol_revolver38_upgraded")) wait 0.05;
    SetDvar("xenon_maxVoicePacketsPerSec", 2);
}

StartOnFadeIn()
{
    // Wait until rounds have started, then wait accordingly for when fade in starts and split
    level flag::wait_till("start_zombie_round_logic");
    wait 2.15;
    SetDvar("xenon_maxVoicePacketsPerSec", 1);
    //compiler::livesplit("start");
    //thread RunTimer();
    //compiler::livesplit("pausegametime");
}

RunTimer()
{
    level.ticks = 0;
    for(;;)
    {
        wait 0.05;
        level.ticks++;
        compiler::livesplit("setgametime", level.ticks * 50);
    }
}

SplitOnRoundEnd()
{
    num = 2;
    // Wait for each round end to split
    for(;;)
    {
        level waittill("end_of_round");
        SetDvar("xenon_maxVoicePacketsPerSec", num);
        num++;
    }
}

ZodEggSplits()
{
    level endon("end_game");

    level.ovums_done = 0;

    // Check if the player has the sword, then notify detour to check for player passing through a portal
    level.look_for_sword = false;
    while(!self HasWeapon(level.var_15954023.weapons[self.characterindex][1])) wait 0.05;
    level.look_for_sword = true;

	/*vending_weapon_upgrade_trigger = zm_pap_util::get_triggers();
    array::thread_all(vending_weapon_upgrade_trigger, ::WaitPAP);*/

    // Check for each ovum to be activated and then finished. On the final ovum, don't wait to clear because we split when it's activated
    max = level.activePlayers.size * 4;
    for(i = 0; i < max; i++)
    {
        level flag::wait_till("magic_circle_in_progress");
        level.ovums_done++;
        if(i < max - level.activePlayers.size) level flag::wait_till_clear("magic_circle_in_progress");
        else 
        {
            SetDvar("xenon_maxVoicePacketsPerSec", 4);
            break;
        }
    }

    // Check if the totem is in state 2, which means it's been picked up. Then wait for round flip and start again - split 2nd and 4th flag
    while(level clientfield::get("ee_totem_state") != 2) wait 0.05;
    //SetDvar("xenon_maxVoicePacketsPerSec", 5);
    level waittill("end_of_round");
    while(level clientfield::get("ee_totem_state") != 2) wait 0.05;
    SetDvar("xenon_maxVoicePacketsPerSec", 6);
    level waittill("end_of_round");
    level waittill("end_of_round");
    while(level clientfield::get("ee_totem_state") != 2) wait 0.05;
    SetDvar("xenon_maxVoicePacketsPerSec", 7);

    // Wait for the shadowman to be defeated
    level flag::wait_till("ee_boss_defeated");
    SetDvar("xenon_maxVoicePacketsPerSec", 8);

    // If 4 players are in the game, wait for the egg to be complete
    if(level.activePlayers.size == 4)
    {
        level flag::wait_till("ee_complete");
        compiler::livesplit("split");
    }
}

WaitPAP()
{
    self util::waittill_any("pap_timeout", "pap_taken", "pap_player_disconnected");
    SetDvar("xenon_maxVoicePacketsPerSec", 3);
}

FactoryEggSplits()
{
    level endon("end_game");

    // Wait until egg is completed then split
    player = trigger::wait_till("flytrap_prize");
    compiler::livesplit("split");
}

CastleEggSplits()
{
    level endon("end_game");

    // Wait until the dragons are filled and then check to see if anyone has picked up the bow. Split if anyone has the bow.
    level flag::wait_till("soul_catchers_charged");
    break_loop = false;
    for(;;)
    {
        foreach(player in level.players)
        {
            if(player HasWeapon(GetWeapon("elemental_bow")))
            {
                SetDvar("xenon_maxVoicePacketsPerSec", 2);
                break_loop = true;
                break;
            }
        }
        if(break_loop) break;
        wait 0.05;
    }

    // Wait until round 6 round end to split
    level flag::wait_till_all(Array("elemental_storm_batteries", "elemental_storm_beacons_charged"));
    if(level.round_number < 7) level waittill("end_of_round");
    SetDvar("xenon_maxVoicePacketsPerSec", 3);

    // Monitor both teleporters to see if they are activated, and when they are, check if wisps are finished. Split if teleporter is activated and wisps are finished
    teleporters = GetEntArray("trigger_teleport_pad", "targetname");
    foreach(teleporter in teleporters) teleporter thread MonitorTeleportSplit();

    GetEnt("ee_clocktower_activation_switch", "targetname") waittill("trigger_activated");
    SetDvar("xenon_maxVoicePacketsPerSec", 5);

    GetEnt("ee_clocktower_activation_switch", "targetname") waittill("trigger_activated");
    SetDvar("xenon_maxVoicePacketsPerSec", 6);

    level flag::wait_till("start_channeling_stone_step");
    SetDvar("xenon_maxVoicePacketsPerSec", 4);

    // Wait for boss fight to begin then split
    level flag::wait_till("boss_fight_begin");
    wait 1;
    SetDvar("xenon_maxVoicePacketsPerSec", 7);

    // Wait for outro to start playing then split
    level flag::wait_till("ee_outro");
    SetDvar("xenon_maxVoicePacketsPerSec", 8);
}

MonitorTeleportSplit()
{
    level endon("first_teleport");
    for(;;)
    {
        // Wait for teleporter to be triggered, then check the according flags to see if wisps are finished
        self waittill("trigger", e_player);
        if(zm_utility::is_player_valid(e_player) && !level.is_cooldown && !level flag::get("rocket_firing") && level flag::get("time_travel_teleporter_ready"))
        {
            SetDvar("xenon_maxVoicePacketsPerSec", 4);
            level notify("first_teleport");
            break;
        }
    }
}

IslandEggSplits()
{
    level endon("end_game");

    // Wait for power to turn on then split
    level flag::wait_till("power_on");
    compiler::livesplit("split");

    // Wait for any player to obtain the skull then split
    break_loop = false;
    for(;;)
    {
        foreach(player in level.players)
        {
            if(player HasWeapon(level.var_c003f5b))
            {
                compiler::livesplit("split");
                break_loop = true;
                break;
            }
        }
        if(break_loop) break;
        wait 0.05;
    }

    // Wait for any player to obtain the kt4 then split
    break_loop = false;
    for(;;)
    {
        foreach(player in level.players)
        {
            if(player HasWeapon(level.var_5e75629a))
            {
                compiler::livesplit("split");
                break_loop = true;
                break;
            }
        }
        if(break_loop) break;
        wait 0.05;
    }

    // Wait for ending cutscene to play then split
    level flag::wait_till("flag_play_outro_cutscene");
    compiler::livesplit("split");


}

StalingradEggSplits()
{
    level endon("end_game");

    // Wait for dragon to timeout or be full, then wait 2 seconds because there's a slight delay for the dragon to fly
    level flag::wait_till_any(Array("dragon_rider_timeout", "dragon_full"));
    wait 2;
    compiler::livesplit("split");

    // Wait for dragon to be finished then look for the next fly
    level flag::wait_till_clear_all(Array("dragon_platform_arrive", "dragon_platform_one_rider"));
	level flag::wait_till_clear("dragon_console_global_disable");

    // Wait for dragon to timeout or be full, then wait 2 seconds because there's a slight delay for the dragon to fly - again
    level flag::wait_till_any(Array("dragon_rider_timeout", "dragon_full"));
    wait 2;
    compiler::livesplit("split");

    // Wait for dragon egg incubation
    level flag::wait_till("egg_placed_incubator");
    compiler::livesplit("split");

    // Wait for challenges to start
    level flag::wait_till("scenario_active");
    compiler::livesplit("split");

    // Wait for final download to start
    level flag::wait_till("lockdown_active");
    compiler::livesplit("split");

    // Wait for player to start boss
    sewer = GetEnt("ee_sewer_to_arena_trig", "targetname");
    sewer waittill("trigger", e_who);
    compiler::livesplit("split");
}

GenesisEggSplits()
{
    level endon("end_game");

    // Wait for second reel to be placed
    level flag::wait_till("placed_audio2");
    compiler::livesplit("split");

    // Wait for player to be teleported into sam's room
    level flag::wait_till("sophia_at_teleporter");
    wait 3.25;
    compiler::livesplit("split");

    // Wait for player to be teleported into 1st boss arena
    level flag::wait_till("arena_occupied_by_player");
    compiler::livesplit("split");

    // Wait for play to pickup key after 1st boss
    break_loop = false;
    level flag::wait_till("grand_tour");
    for(;;)
    {
        foreach(player in level.players)
        {
            if(player HasWeapon(level.ballWeapon))
            {
                compiler::livesplit("split");
                break_loop = true;
                break;
            }
        }
        if(break_loop) break;
        wait 0.05;
    }

    // Wait for play to be teleported into 2nd boss arena
    level flag::wait_till("boss_fight");
    compiler::livesplit("split");
}