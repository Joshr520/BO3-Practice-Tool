PickupKT()
{
    self thread PickupWWPart("ww1_found");
    self thread PickupWWPart("ww2_found");
    self thread PickupWWPart("ww3_found");

    ww_station = struct::get("wonder_weapon_loc");
    level flag::wait_till_all(Array("ww1_found", "ww2_found", "ww3_found"));
    ww_station.trigger notify("trigger", self);
    wait 0.5;
    station_scene = GetEnt("ww_station", "targetname");
    station_scene scene::skipto_end("p7_fxanim_zm_island_mirg_centrifuge_table_gun_up_bundle", station_scene);
    wait 0.5;
    station_scene scene::skipto_end("p7_fxanim_zm_island_mirg_centrifuge_table_turn_on_bundle", station_scene);
    wait 0.5;
    station_scene scene::skipto_end("p7_fxanim_zm_island_mirg_centrifuge_table_gun_down_bundle", station_scene);
    wait 0.5;
    ww_station.trigger notify("trigger", self);
}

PickupMasamune()
{
    self thread PickupWWPart("wwup1_found");
    self thread PickupWWPart("wwup2_found");
    self thread PickupWWPart("wwup3_found");

    self.get_player_weapon_limit = ::TradeWeaponLimit;
    level flag::set("ww_obtained");
    if(!self HasWeapon(level.var_5e75629a))
    {
        self GiveWeapon(level.var_5e75629a);
        self SwitchToWeapon(level.var_5e75629a);
        wait 1.5;
    }
    ww_station = struct::get("ww_up_pos");
    level flag::wait_till_all(Array("wwup1_found", "wwup2_found", "wwup3_found"));
    ww_station.trigger notify("trigger", self);
    wait 0.5;
    self.get_player_weapon_limit = undefined;
    station_scene = GetEnt("wwup_station", "targetname");
    station_scene scene::skipto_end("p7_fxanim_zm_island_mirg_centrifuge_table_gun_up_bundle", station_scene);
    wait 0.5;
    station_scene scene::skipto_end("p7_fxanim_zm_island_mirg_centrifuge_table_turn_on_bundle", station_scene);
    wait 0.5;
    station_scene scene::skipto_end("p7_fxanim_zm_island_mirg_centrifuge_table_gun_down_bundle", station_scene);
    wait 0.5;
    ww_station.trigger notify("trigger", self);
}