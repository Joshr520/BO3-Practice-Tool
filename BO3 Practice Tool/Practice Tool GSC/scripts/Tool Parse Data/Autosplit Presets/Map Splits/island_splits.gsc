autoexec WatchSkullRitual()
{
    system::wait_till("all");
    for (i = 1; i <= 4; i++) {
        thread WatchSkullFlag(i);
    }
}

WatchSkullFlag(i)
{
    level flag::wait_till("skullquest_ritual_complete" + i);
    level notify("skull_ritual_complete_" + level.var_3846d9a8);
}

SplitSkullRitual(split_name)
{
    if (split_name == "Skull Ritual") {
        level flag::wait_till("a_player_got_skullgun");
        return;
    }
    level waittill("skull_ritual_complete_" + StrTok(split_name, " ")[1]);
}

SplitBunkerOpen(split_name)
{
    level flag::wait_till("connect_bunker_exterior_to_bunker_interior");
}

SplitPowerOn(split_name)
{
    level flag::wait_till("power_on");
}

SplitKT(split_name)
{
    if (split_name == "KT-4") {
        level flag::wait_till("ww_obtained");
        return;
    }
    level flag::wait_till("wwup_ready");
    level flag::wait_till_clear("wwup_ready");
}

SplitPoster(split_name)
{
    level flag::wait_till("trilogy_released");
}

SplitBullet(split_name)
{
    level flag::wait_till("player_has_aa_gun_ammo");
}

SplitPlane(split_name)
{
    level flag::wait_till("aa_gun_ee_complete");
}

SplitElevator(split_name)
{
    level flag::wait_till("elevator_in_use");
}

SplitIslandBossEnter(split_name)
{
    level waittill(#"hash_add73e69");
}

SplitIslandEnd()
{
    level flag::wait_till("flag_play_outro_cutscene");
}