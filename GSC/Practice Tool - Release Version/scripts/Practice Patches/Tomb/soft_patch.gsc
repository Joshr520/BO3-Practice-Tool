TombSoftPatch()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_tomb")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Origins");
        return;
    }
    thread WriteToScreen("Origins Soft Patch Loaded");
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_self_medication", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_extra_credit");
    level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_self_medication", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while(self.bgb_pack_randomized[0] != "zm_bgb_reign_drops" && self.bgb_pack_randomized[0] != "zm_bgb_extra_credit") self.bgb_pack_randomized = array::randomize(self.bgb_pack);

    level flag::wait_till("initial_blackscreen_passed");

    thread GuranteeIce();

    level.force_weather[2] = "rain";
    level.force_weather[3] = "snow";
	level.force_weather[4] = "snow";

    level flag::set("tomb_soft_patch");
}

GuranteeIce()
{
    foreach(s_staff in level.ice_staff_pieces)
    {
        s_staff.num_misses = 4;
    }
    level.ice_staff_pieces[1].num_misses = 0;
    while(!level.ice_staff_pieces[1].num_misses) wait 0.05;
    level.ice_staff_pieces[1].num_misses = 4;
}