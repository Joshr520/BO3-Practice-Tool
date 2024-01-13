autoexec InitCustomFlags()
{
    system::wait_till("all");

    level flag::init("round_skip_request");
    level flag::init("zombie_health_override");
    level flag::init("zombie_tp_override");
    
    switch (level.mapname) {
        case "zm_zod": {
            level flag::init("margwa_ovums_practice");
        }
        case "zm_castle": {
            level flag::init("tracking_tp");
            level flag::init("tracking_keeper");
            break;
        }
        case "zm_island": {
            level flag::init("island_soft_patch");
            break;
        }
        case "zm_stalingrad": {
            level flag::init("stalingrad_soft_patch");
            level flag::init("stalingrad_2nd_fly");
            level flag::init("stalingrad_quote_skips");
            level flag::init("quote_skip_request");
            level flag::init("gersh_quote_skip");
            break;
        }
        case "zm_tomb": {
            level flag::init("puzzle_practice");
            level flag::init("tomb_soft_patch");
            break;
        }
        case "zm_genesis": {
            level flag::init("basketball_practice");
            level flag::init("boss_practice");
        }
        default: {
            break;
        }
    }
}

init()
{
    SetDvar("sv_cheats", 1);
    SetDvar("scr_firstGumFree", 1);
	SetDvar("zm_private_rankedmatch", 1);
    //level.player_out_of_playable_area_monitor = 0; //ADD THIS AS AN OPTION 
    level.givecustomloadout = ::GiveLoadout;
    level.onlinegame = true;
    level.zm_disable_recording_stats = true;
    level.rankedmatch = 1;
    level.var_dfc343e9 = 0;
    level.display_messages = true;
    level.messages_prompt = [];
}

on_player_connect()
{
    if (!self IsHost()) {
        return;
    }

    WaitFadeIn();

    message = hud::createserverfontstring("big", 1.1);
    message hud::setpoint("TOP", "TOP", 0, 0);
    if (!IsTrue(level.num_splits)) {
        message SetText("BO3 Practice Tool Loaded: ^6github.com/Joshr520/BO3-Practice-Tool");
    }
    else {
        message SetText("BO3 GSC Autosplits Via: ^6github.com/Joshr520/BO3-Practice-Tool");
    }
    message SetTypewriterFX(50, 15000, 0);
    wait 5;
    message FadeOverTime(5);
    message.alpha = 0;
    wait 5;
    message Destroy();
}

on_player_spawned()
{
    if (self IsTestClient()) {
        return;
    }

    level flag::wait_till("initial_blackscreen_passed");
}

DebugTesting()
{
    self thread GodMode(1);
    self thread InfiniteAmmo(1);
    self thread GiveAllPerks();
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    self.score = 50000;
}