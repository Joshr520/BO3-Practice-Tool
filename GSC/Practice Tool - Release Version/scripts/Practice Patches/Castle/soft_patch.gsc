CastleSoftPatch()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_castle")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: DE");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(Array("zm_bgb_anywhere_but_here", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_perkaholic", "zm_bgb_extra_credit"));
    ArrayInsert(self.bgb_pack_randomized, "zm_bgb_reign_drops", 0);

    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("DE Soft Patch Loaded");

    self GiveCastleCharacter();
    level.givecustomcharacters = ::GiveCastleCharacter;
    level.next_dog_round = 5;
}