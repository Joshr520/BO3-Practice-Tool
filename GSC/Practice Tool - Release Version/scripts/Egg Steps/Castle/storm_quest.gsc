FinishStorm()
{
    self ActivateStormQuest();
    self StormShootBonfires();
    self StormWallrun();
    self StormFillUrns();
    self StormTriggerArrow();
    self StormBuildBow();
}

ActivateStormQuest()
{
    if(CheckQuestProgress("storm") >= 1) return;
    if(!IsDefined(struct::get("quest_start_elemental_storm").var_67b5dd94))
    {
        weather_damage = 0;
        while(!weather_damage)
        {
            weather_trig = GetEnt("aq_es_weather_vane_trig", "targetname");
            if(!IsDefined(weather_trig)) continue;
            weather_damage = 1;
            weather_trig notify("damage", 1, self, (-0.55191, 0.294189, -13.1041), (-183.448, 1878.71, 1270.1), "MOD_EXPLOSIVE", "", "", "", GetWeapon("elemental_bow"));
            wait 0.05;
        }

        level waittill(#"hash_4e123b5d");
        wait 1;
    }

    arrow = struct::get("quest_start_elemental_storm");
    self BuildAndActivateTrigger(arrow.var_67b5dd94);
}

StormShootBonfires()
{
    if(CheckQuestProgress("storm") >= 2) return;
    while(CheckQuestProgress("storm") < 1) wait 0.05;
    
    bonfires = GetEntArray("aq_es_beacon_trig", "script_noteworthy");
    foreach(bonfire in bonfires)
    {
        s_beacon = struct::get(bonfire.target);
        if(!IsDefined(s_beacon.var_41f52afd))
        {
            s_beacon.var_41f52afd = util::spawn_model("tag_origin", s_beacon.origin);
        }
        s_beacon.var_41f52afd clientfield::set("beacon_fx", 1);
        bonfire.b_lit = 1;
    }

    foreach(bonfire in bonfires)
    {
        bonfire notify("beacon_activated");
    }
}

StormWallrun()
{
    if(CheckQuestProgress("storm") >= 3) return;
    while(CheckQuestProgress("storm") < 2) wait 0.05;
    if(level.var_f8d1dc16 != self) level.var_f8d1dc16 = self;
    wallruns = GetEntArray("aq_es_wallrun_trigger", "targetname");
    self.var_a4f04654 = 4;
    level.var_49593fd9 = [];
    foreach(wallrun in wallruns)
    {
        wallrun notify("trigger", self);
    }
}

StormFillUrns()
{
    if(CheckQuestProgress("storm") >= 4) return;
    while(CheckQuestProgress("storm") < 3) wait 0.05;

    bonfires = GetEntArray("aq_es_beacon_trig", "script_noteworthy");
    foreach(bonfire in bonfires)
    {
        s_beacon = struct::get(bonfire.target);
        s_beacon.var_41f52afd clientfield::set("beacon_fx", 2);
        bonfire.b_charged = 1;
    }

    if(level.var_f8d1dc16 != self) level.var_f8d1dc16 = self;
    urn_souls = GetEntArray("aq_es_battery_volume", "script_noteworthy");
    foreach(urn_soul in urn_souls)
    {
        urn_soul.var_bb486f65 = 4;
        urn_soul notify("killed");
    }
    foreach(bonfire in bonfires)
    {
        bonfire notify("beacon_charged");
    }
}

StormTriggerArrow()
{
    if(CheckQuestProgress("storm") >= 5) return;
    while(CheckQuestProgress("storm") < 4) wait 0.05;
    if(level.var_f8d1dc16 != self) level.var_f8d1dc16 = self;
    arrow = struct::get("quest_reforge_elemental_storm");
    while(!IsDefined(arrow.var_67b5dd94)) wait 0.05;
    arrow.var_67b5dd94 notify("trigger", self);
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_storm_arrow_reform_bundle", 0, 0, 0);
    wait 0.5;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_storm_arrow_whole_bundle", 0, 0, 0);
    wait 0.5;
    arrow.var_67b5dd94 notify("trigger", self);
}

StormBuildBow()
{
    if(CheckQuestProgress("storm") >= 6) return;
    soulbox = struct::get("upgraded_bow_struct_elemental_storm", "targetname");
    while(!IsDefined(soulbox.var_67b5dd94)) wait 0.05;
    if(level.var_f8d1dc16 != self) level.var_f8d1dc16 = self;
    soulbox.var_67b5dd94 notify("trigger", self);
    level flag::set("elemental_storm_upgraded");
    wait 3;
    soulbox.var_67b5dd94 notify("trigger", self);
    wait 0.5;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_upgrade_bundle_elemental_storm", 0, 0, 0);
    wait 0.5;
    soulbox.var_67b5dd94 notify("trigger", self);
}