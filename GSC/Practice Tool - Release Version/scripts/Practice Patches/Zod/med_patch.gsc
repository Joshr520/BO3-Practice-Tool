ZodMedPatch()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_zod")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: SOE");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
    self.bgb_pack_randomized = Array("zm_bgb_reign_drops", "zm_bgb_perkaholic", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_wall_power", "zm_bgb_anywhere_but_here");

    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("SOE Med Patch Loaded");

    self GiveZodCharacter();
    level.givecustomcharacters = ::GiveZodCharacter;
    self thread FixSwordPickups();

    level.next_wasp_round = 6;
    level.n_next_raps_round = 10;

    if(IsDefined(level.var_2c12d9a6)) level.old_abh_func = level.var_2c12d9a6;
    level.var_2c12d9a6 = ::ZodABHManip;
    level.abh_array = Array(0, 9, 8, 6, 1);

    while(self.bgb_pack_randomized.size) wait 0.05;
    self.bgb_pack_randomized = Array("zm_bgb_wall_power", "zm_bgb_reign_drops", "zm_bgb_anywhere_but_here", "zm_bgb_perkaholic", "zm_bgb_dead_of_nuclear_winter");
    while(self.bgb_pack_randomized.size) wait 0.05;
    self.bgb_pack_randomized = Array("zm_bgb_wall_power", "zm_bgb_anywhere_but_here", "zm_bgb_reign_drops", "zm_bgb_perkaholic", "zm_bgb_dead_of_nuclear_winter");
}