FinishFire()
{
    self ActivateFireQuest();
    self FireShootObelisk();
    self FireFillCircles();
    self FireFinishGolf();
    self FireFinishArrow();
    self FireBuildBow();
}

ActivateFireQuest()
{
    wall_damage = 0;
    while(!wall_damage)
    {
        wall_trig = GetEnt("aq_rp_clock_wall_trig", "targetname");
        if(!IsDefined(wall_trig)) continue;
        wall_damage = 1;
        wall_trig notify("damage", 1, self, (-0.414551, -8.45129, 20.6498), (-1066.59, 1648.45, 1082.35), "MOD_EXPLOSIVE", "", "", "", GetWeapon("elemental_bow"));
        wait 0.05;
    }

    wait 6;
    arrow = struct::get("quest_start_rune_prison");
    while(!IsDefined(arrow.var_67b5dd94)) wait 0.05;
    self BuildAndActivateTrigger(arrow.var_67b5dd94);
}

FireShootObelisk()
{
    while(level clientfield::get("quest_state_rune") != 1) wait 0.05;
    if(level.var_c62829c7 != self) level.var_c62829c7 = self;
    obelisk_damage = 0;
    level flag::set("rune_prison_obelisk_magma_enabled");
    while(!obelisk_damage)
    {
        obelisk_trig = GetEnt("aq_rp_obelisk_magma_trig", "targetname");
        if(!IsDefined(obelisk_trig)) continue;
        obelisk_damage = 1;
        obelisk_trig notify("damage", 1, self, (4.71924, -3.99817, 34.0994), (4977.28, -1801, -2069.1), "MOD_EXPLOSIVE", "", "", "", GetWeapon("elemental_bow"));
        wait 0.05;
    }
    level flag::clear("rune_prison_obelisk_magma_enabled");
    wait 0.2;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_rune_orb_trail_bundle", 0, 0, 0);
    wait 0.5;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_rune_orb_bundle", 0, 0, 0);
}

FireFillCircles()
{
    while(level clientfield::get("quest_state_rune") != 2) wait 0.05;
    level flag::wait_till("rune_prison_magma_ball");
    wait 1;
    if(level.var_c62829c7 != self) level.var_c62829c7 = self;
    circles = GetEntArray("aq_rp_runic_circle_volume", "script_noteworthy");
    self.is_flung = 1;
    foreach(circle in circles)
    {
        target = GetEnt(circle.target + "_trig", "targetname");
        switch(circle.script_label)
        {
            case "road":
                direction = (-45.8943, 15.8018, 27.7048);
                point = (1391.89, 1057.2, 170.295);
                break;
            case "roof":
                direction = (-4.6696, -1.59766, 30.875);
                point = (213.67, 1918.6, 880.125);
                break;
            case "rampart":
                direction = (0.633728, 2.04285, 31.875);
                point = (-600.634, 1425.96, 808.125);
                break;
        }
        target notify("damage", 1, self, direction, point, "MOD_EXPLOSIVE", "", "", "", GetWeapon("elemental_bow"));

        wait 0.1;
        
        for(i = 0; i < 9; i++)
        {
            circle notify("killed");
            wait 0.05;
        }
    }
    self.is_flung = 0;
}

FireFinishGolf()
{
    while(level clientfield::get("quest_state_rune") != 3) wait 0.05;
    wait 1;
    if(level.var_c62829c7 != self) level.var_c62829c7 = self;
    clock_trig = struct::get("aq_rp_clock_use_struct", "targetname");
    clock_trig.var_67b5dd94 notify("trigger", self);
    clock = GetEnt("aq_rp_clock_wheel", "targetname");
    clock waittill("rotatedone");
    volumes = GetEntArray("aq_rp_runic_circle_volume", "script_noteworthy");
    fireplace = volumes[0].var_336f1366;
    wait 1;
    level flag::set("rune_prison_golf");
    wait 1;
    fireplace.var_67b5dd94 notify("trigger", self);
    magma_ball = GetEnt("aq_rp_magma_ball_tag", "targetname");
    magma_ball flag::wait_till("magma_ball_move_done");
    level.var_c62829c7 notify("quest_swap");
}

FireFinishArrow()
{
    while(level clientfield::get("quest_state_rune") != 4) wait 0.05;
    if(level.var_c62829c7 != self) level.var_c62829c7 = self;
    magma_trig = struct::get("quest_reforge_rune_prison", "targetname");
    magma_trig.var_67b5dd94 notify("trigger", self);
    wait 0.2;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_rune_orb_crust_bundle", 0, 0, 0);
    wait 1;
    magma_trig.var_67b5dd94 notify("trigger", self);
}

FireBuildBow()
{
    soulbox = struct::get("upgraded_bow_struct_rune_prison", "targetname");
    while(!IsDefined(soulbox.var_67b5dd94)) wait 0.05;
    if(level.var_c62829c7 != self) level.var_c62829c7 = self;
    soulbox.var_67b5dd94 notify("trigger", self);
    level flag::set("rune_prison_upgraded");
    wait 3;
    soulbox.var_67b5dd94 notify("trigger", self);
    wait 0.5;
    /*level scene::skip_scene("p7_fxanim_zm_castle_quest_upgrade_bundle_rune_prison", 0, 0, 0);
    wait 0.5;
    soulbox.var_67b5dd94 notify("trigger", self);*/
}