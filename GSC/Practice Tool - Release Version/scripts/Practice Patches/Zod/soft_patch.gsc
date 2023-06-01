ZodSoftPatch()
{
    WaitFadeIn();
    if(level.script != "zm_zod")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: SOE");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
    self.bgb_pack_randomized = array::randomize(Array("zm_bgb_anywhere_but_here", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_perkaholic", "zm_bgb_wall_power"));
    while(self.bgb_pack_randomized[0] == "zm_bgb_anywhere_but_here") self.bgb_pack_randomized = array::randomize(self.bgb_pack_randomized);
    ArrayInsert(self.bgb_pack_randomized, "zm_bgb_reign_drops", 0);
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("SOE Soft Patch Loaded");

    self GiveZodCharacter();
    level.givecustomcharacters = ::GiveZodCharacter;
    self thread FixSwordPickups();

    level.next_wasp_round = 6;
}