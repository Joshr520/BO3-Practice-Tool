Boss1Practice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_genesis")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Rev");
        return;
    }
    thread WriteToScreen("Boss 1 Practice Starting");
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    level flag::set("character_stones_done");
    level flag::set("phased_sophia_start");
    level flag::set("book_picked_up");
    level flag::set("book_placed");
    level flag::set("rune_circle_on");
    level.var_b1b99f8d[0] = 0;
    level.var_b1b99f8d[1] = 1;
    level.var_b1b99f8d[2] = 2;
    level.var_b1b99f8d[3] = 3;
    level clientfield::set("gen_rune_electricity", 1);
    level clientfield::set("gen_rune_fire", 1);
    level clientfield::set("gen_rune_light", 1);
    level clientfield::set("gen_rune_shadow", 1);
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self zm_weapons::weapon_give(GetWeapon("shotgun_pump_upgraded"), 1);
    self zm_weapons::weapon_give(GetWeapon("idgun_genesis_0_upgraded"), 1);
    self [[level.zombie_weapons_callbacks[level.w_octobomb]]]();
    self SetOrigin((-364.035, -8082.19, -1327.55));
    self SetPlayerAngles((2.47742, -132.885, 0));

    level waittill(#"hash_78e9c51c");
    book = struct::get("ee_book_arena", "targetname");
    wait 0.25;
    book notify("trigger_activated", self);
    while(level.var_6000c357.size != 4) wait 0.05;
    number_map = [];
    number_map[0] = 4;
    number_map[1] = 1;
    number_map[2] = 3;
    number_map[3] = 2;
    wait 0.25;
    IPrintLnBold("Symbol Order: " + number_map[level.var_6000c357[0]] + " - " + number_map[level.var_6000c357[1]] + " - " + number_map[level.var_6000c357[2]] + " - " + number_map[level.var_6000c357[3]]);
}