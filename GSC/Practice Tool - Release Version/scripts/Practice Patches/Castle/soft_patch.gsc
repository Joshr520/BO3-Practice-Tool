CastleSoftPatch()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_castle")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: DE");
        return;
    }
    thread WriteToScreen("DE Soft Patch Loaded");
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(Array("zm_bgb_anywhere_but_here", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_perkaholic", "zm_bgb_extra_credit"));
    ArrayInsert(self.bgb_pack_randomized, "zm_bgb_reign_drops", 0);

    self GiveCastleCharacter();
    level.givecustomcharacters = ::GiveCastleCharacter;

    level.next_dog_round = 5;
}