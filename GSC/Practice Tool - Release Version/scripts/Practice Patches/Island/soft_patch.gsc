IslandSoftPatch()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_island")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: ZNS");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while((self.bgb_pack_randomized[4] != "zm_bgb_anywhere_but_here" && self.bgb_pack_randomized[5] != "zm_bgb_anywhere_but_here") || (self.bgb_pack_randomized[0] != "zm_bgb_reign_drops" && self.bgb_pack_randomized[0] != "zm_bgb_extra_credit")) self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("ZNS Soft Patch Loaded");
    level.n_next_spider_round = 6;
}