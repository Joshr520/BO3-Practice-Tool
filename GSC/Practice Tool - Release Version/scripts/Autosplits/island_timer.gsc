WaitSkull(num)
{
    while(level.var_3846d9a8 < num) wait 0.05;
}

WaitSkullWeapon()
{
    level flag::wait_till("a_player_got_skullgun");
}

WaitBunkerOpen()
{
    level flag::wait_till("connect_bunker_exterior_to_bunker_interior");
}

WaitPowerOn()
{
    level flag::wait_till("power_on");
}

WaitKT(upgraded)
{
    if(!upgraded) level flag::wait_till("ww_obtained");
    else
    {
        foreach(player in GetPlayers()) player thread WaitKTUpgraded();
        level waittill("wwup_obtained");
    }
}

WaitKTUpgraded()
{
    level endon("wwup_obtained");
    self waittill("player_upgraded_ww");
    level notify("wwup_obtained");
}

WaitPoster()
{
    level flag::wait_till("trilogy_released");
}

WaitBullet()
{
    level flag::wait_till("player_has_aa_gun_ammo");
}

WaitPlane()
{
    level flag::wait_till("aa_gun_ee_complete");
}

WaitElevator()
{
    level flag::wait_till("elevator_in_use");
}

WaitZNSBossEnter()
{
    level waittill(#"hash_add73e69");
}

WaitIslandEnd()
{
    level flag::wait_till("flag_play_outro_cutscene");
}