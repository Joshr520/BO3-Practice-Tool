TombSoftPatch()
{
    WaitFadeIn();
    level flag::set("tomb_soft_patch");
    if(level.script != "zm_tomb")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Origins");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_self_medication", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_extra_credit");
    level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_self_medication", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while(self.bgb_pack_randomized[0] != "zm_bgb_reign_drops" && self.bgb_pack_randomized[0] != "zm_bgb_extra_credit") self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    level.player_out_of_playable_area_monitor = 1;
    foreach(player in GetPlayers()) self thread zm::player_out_of_playable_area_monitor();

    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Origins Soft Patch Loaded");

    thread GuranteeIce();
    level.force_weather[2] = "rain";
    level.force_weather[3] = "snow";
	level.force_weather[4] = "snow";
}

GuranteeIce()
{
    level.ice_staff_pieces[2].num_misses = 4;
    while(level.ice_staff_pieces.size)
    {
        level waittill("sam_clue_dig", e_player);
        zone = zm_zonemgr::get_zone_from_position(e_player.origin + VectorScale((0, 0, 1), 32), 0);
        foreach(s_staff in level.ice_staff_pieces)
	    {
            if(IsSubStr(zone, s_staff.zone_substr))
            {
                s_staff.num_misses = 4;
                break;
            }
        }
    }
}