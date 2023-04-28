GenesisSoftPatch()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_genesis")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Rev");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_shopping_free", "zm_bgb_immolation_liquidation", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_shopping_free", "zm_bgb_immolation_liquidation", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while(self.bgb_pack_randomized[0] == "zm_bgb_shopping_free" || (self.bgb_pack_randomized[0] == "zm_bgb_perkaholic" && self.bgb_pack_randomized[1] == "zm_bgb_immolation_liquidation")) self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Rev Soft Patch Loaded");
    level.customrandomweaponweights = ::GenesisBox;
}

GenesisBox(keys)
{
    pap_triggers = zm_pap_util::get_triggers();
    if(RandomInt(100) <= 5 && zm_magicbox::treasure_chest_canplayerreceiveweapon(self, level.idgun_weapons[0], pap_triggers) && zm_weapons::limited_weapon_below_quota(level.idgun_weapons[0]))
    {
        ArrayInsert(keys, level.idgun_weapons[0], 0);
        return keys;
    }
    if(RandomInt(100) <= 5 && zm_magicbox::treasure_chest_canplayerreceiveweapon(self, level.w_octobomb, pap_triggers))
    {
        ArrayInsert(keys, level.w_octobomb, 0);
        return keys;
    }
    return keys;
}