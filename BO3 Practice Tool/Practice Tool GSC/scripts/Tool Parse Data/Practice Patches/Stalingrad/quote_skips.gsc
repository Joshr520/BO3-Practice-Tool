StalingradQuoteSkipsPractice()
{
    if (level.mapname != "zm_stalingrad") {
        DebugPrint("error", "Boss Quote Skip Practice attempted to load on " + level.mapDisplayName + " but it requires Gorod Krovi");
        return;
    }
    level flag::set("stalingrad_quote_skips");
    RegisterHotkey(0x74, "Boss Quote Skip: ^9F5", undefined, ::BossQuoteCountdown);
    RegisterHotkey(0x75, "Gersh Quote Skip: ^9F6", undefined, ::GershQuoteSkip);
    level.display_messages = false;

    WaitFadeIn();
    level.players[0] thread OpenAllDoors();
    level.players[0] thread ActivateAllPower();
    foreach (player in GetPlayers()) {
        player.score = 50000;
        player SetOrigin((70 - (50 * player.characterindex), 4770, 144.125));
        player SetPlayerAngles((0, -90, 0));
        player InfiniteAmmo(true);
        player GiveAllPerks();
        player GivePlayerWeapon("dragon_gauntlet");
        player GivePlayerWeapon("launcher_dragon_strike");
    }
    
    level flag::clear("spawn_zombies");
    level flag::set("story_playing");

    level flag::wait_till("initial_blackscreen_passed");
    level.display_messages = true;
    thread WriteToScreen("Bow Shots Practice Loaded");

    wait 5;
    level.var_de98a8ad = 0;
}

BossQuoteCountdown()
{
    if (level flag::get("quote_skip_request")) {
        return;
    }
    level flag::set("quote_skip_request");

    timer = hud::createservertimer("objective", 2);
    timer hud::setpoint("CENTER", undefined, 0, -100);
    timer.label = &"Flash in: ";
    timer SetTenthsTimer(3);
    wait 3;
    timer Destroy();
    
    level.flash_time = GetTime();
    level [[ @zm_stalingrad_ee_main<scripts\zm\zm_stalingrad_ee_main.gsc>::function_fa7fbd4c ]](0);
    while(!level.var_a090a655 [[ @zm_stalingrad_util<scripts\zm\zm_stalingrad_util.gsc>::function_1af75b1b ]](500)) {
        IPrintLnBold("Waiting for player to be within 500 units of Sophia");
		wait(1);
	}
    level [[ @zm_stalingrad_ee_main<scripts\zm\zm_stalingrad_ee_main.gsc>::function_2868b6f4 ]]();
    wait 1;
    level flag::clear("quote_skip_request");
}

GershQuoteSkip()
{
    if (level flag::get("quote_skip_request")) {
        return;
    }
    level flag::set("quote_skip_request");
    level flag::set("gersh_quote_skip");
    level flag::clear("story_playing");

    if (!GetDvarInt("gersh_tutorial", 0)) {
        SetDvar("gersh_tutorial", 1);
        IPrintLnBold("Gersh spawned at bunker. Damage with any weapon to start quote skips");
    }

    gersh = util::spawn_model("p7_fxanim_zm_stal_ray_gun_ball_mod", (20, 2250, 100));
    gersh.takedamage = 1;
    gersh clientfield::set("ee_anomaly_loop", 1);

    gersh waittill("damage");
    gersh clientfield::set("ee_anomaly_loop", 0);
    gersh playsound("zmb_anomoly_dmg_hit");
    level [[ @zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::function_460341f9 ]]();
    gersh clientfield::set("ee_anomaly_loop", 1);
    gersh [[ @zm_stalingrad_ee_main<scripts\zm\zm_stalingrad_ee_main.gsc>::function_48dcad89 ]]();
    level flag::clear("quote_skip_request");
    level flag::clear("gersh_quote_skip");
    level flag::set("story_playing");
}

detour zm_stalingrad_pavlov_trap<scripts\zm\zm_stalingrad_pavlov_trap.gsc>::function_335dff5e()
{
    if (!level flag::get("stalingrad_quote_skips")) {
        return self [[ @zm_stalingrad_pavlov_trap<scripts\zm\zm_stalingrad_pavlov_trap.gsc>::function_335dff5e ]]();
    }

	for (;;) {
        self waittill("trigger", e_player);
        if (IsDefined(level.flash_time)) {
            trap_time = GetTime();
            diff_time = trap_time - level.flash_time;
            IPrintLnBold(((diff_time >= 500 && diff_time <= 1000) ? "Success: " : "Failure: ") + "Trap hit " + diff_time + " milliseconds after flash. Successful range is 500-1000 milliseconds");
        }
        else if (level flag::get("gersh_quote_skip")) {
            wait 1;
            e_player thread [[ @zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::function_96153834 ]]();
        }
        else {
            IPrintLnBold("No quote skip recognized");
        }
    }
}

detour zm_stalingrad_pavlov_trap<scripts\zm\zm_stalingrad_pavlov_trap.gsc>::function_fbea6a64(e_player)
{
    if (!level flag::get("stalingrad_quote_skips")) {
        return self [[ @zm_stalingrad_pavlov_trap<scripts\zm\zm_stalingrad_pavlov_trap.gsc>::function_fbea6a64 ]](e_player);
    }
    
    self SetHintString("Hold [{+activate}] to test quote skip");
    return true;
}

detour zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::function_7b697614(str_vo_alias, n_delay = 0, b_wait_if_busy = 0, n_priority = 0, var_d1295208 = 0)
{
    if (IsTrue(level.num_splits) || !level flag::get("stalingrad_quote_skips")) {
        return self [[ @zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::function_7b697614 ]](str_vo_alias, n_delay, b_wait_if_busy, n_priority, var_d1295208);
    }

	self endon("death");
	self endon("disconnect");
	self endon("stop_vo_convo");
    if (level flag::get("story_playing")) {
        return false;
    }
	if(zm_audio::arenearbyspeakersactive(10000) && !IsTrue(var_d1295208)) {
		return false;
	}
	if(IsTrue(self.isspeaking) || (IsTrue(level.sndvoxoverride) || (IsPlayer(self) && self IsPlayerUnderwater() && !level flag::get("abcd_speaking"))))
	{
		if(IsTrue(b_wait_if_busy)) {
			while(IsTrue(self.isspeaking) || IsTrue(level.sndvoxoverride) || (IsPlayer(self) && self IsPlayerUnderwater())) {
				wait(0.1);
			}
			wait(0.35);
		}
		else {
			return false;
		}
	}
	if(n_delay > 0) {
		wait(n_delay);
	}
	if(IsTrue(self.isspeaking) && IsTrue(self.b_wait_if_busy)) {
		while(IsTrue(self.isspeaking)) {
			wait(0.1);
		}
	}
	else if(IsTrue(self.isspeaking) && !IsTrue(self.b_wait_if_busy) || IsTrue(level.sndvoxoverride) || (IsPlayer(self) && self IsPlayerUnderwater() && !level flag::get("abcd_speaking"))) {
        IPrintLnBold("Quote: " + str_vo_alias + " skipped");
		return false;
	}
    self notify(str_vo_alias + "_vo_started");
	self.isspeaking = 1;
	level.sndvoxoverride = 1;
	self thread [[ @zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::function_b3baa665 ]]();
	self.n_vo_priority = n_priority;
	self.str_vo_being_spoken = str_vo_alias;
	array::add(level.a_e_speakers, self, 1);
	str_notify = str_vo_alias + "_vo_done";
	if(IsActor(self) || IsPlayer(self))
	{
		self PlaySoundWithNotify(str_vo_alias, str_notify, "J_head");
	}
	else
	{
		self PlaySoundWithNotify(str_vo_alias, str_notify);
	}
	self waittill(str_notify);
	self [[ @zm_stalingrad_vo<scripts\zm\zm_stalingrad_vo.gsc>::vo_clear ]]();
    return true;
}