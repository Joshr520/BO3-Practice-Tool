StalingradSoftPatch()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_stalingrad")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Stalingrad");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_shopping_free");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_shopping_free");
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while(self.bgb_pack_randomized[0] != "zm_bgb_reign_drops" && self.bgb_pack_randomized[0] != "zm_bgb_shopping_free") self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("GK Soft Patch Loaded");
}