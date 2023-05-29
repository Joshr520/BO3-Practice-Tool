finishVoid()
{
    self ActivateVoidQuest();
    self VoidActivateUrn();
    self VoidTriggerSkulls();
    self VoidFillCrawlers();
    self VoidCollectRunes();
    self VoidBuildBow();
}

ActivateVoidQuest()
{
    if(CheckQuestProgress("demon") >= 1) return;
    if(!IsDefined(struct::get("quest_start_demon_gate").var_67b5dd94))
    {
        wall_damage = 0;
        while(!wall_damage)
        {
            wall_trig = GetEnt("aq_dg_gatehouse_symbol_trig", "targetname");
            if(!IsDefined(wall_trig)) continue;
            wall_damage = 1;
            wall_trig notify("damage", 1, self, (-4.04236, -9.22034, 30.3611), (1435.04, 1775.22, 526.639), "MOD_EXPLOSIVE", "", "", "", GetWeapon("elemental_bow"));
            thread DisableOverride();
            wait 0.05;
        }

        level waittill(#"hash_c8347a07");
        wait 1;
    }

    arrow = struct::get("quest_start_demon_gate");
    self BuildAndActivateTrigger(arrow.var_67b5dd94);
    level notify("override_finish");
}

VoidActivateUrn()
{
    if(CheckQuestProgress("demon") >= 2) return;
    while(CheckQuestProgress("demon") < 1) wait 0.05;
    if(level.var_6e68c0d8 != self) level.var_6e68c0d8 = self;
    level flag::set("demon_gate_seal");
    thread DisableOverride();
    urn = struct::get("aq_dg_urn_struct", "targetname");
    while(!IsDefined(urn.var_67b5dd94)) wait 0.05; 
    urn.var_67b5dd94 notify("trigger", self);
    level notify("override_finish");
}

VoidTriggerSkulls()
{
    if(CheckQuestProgress("demon") >= 3) return;
    while(CheckQuestProgress("demon") < 2) wait 0.05;
    if(level.var_6e68c0d8 != self) level.var_6e68c0d8 = self;
    skulls = getentArray("aq_dg_fossil", "script_noteworthy");
    foreach(skull in skulls)
    {
        skull.var_67b5dd94 notify("trigger", self);
    }
}

VoidFillCrawlers()
{
    if(CheckQuestProgress("demon") >= 4) return;
    while(CheckQuestProgress("demon") < 3) wait 0.05;
    if(level.var_6e68c0d8 != self) level.var_6e68c0d8 = self;
    thread DisableOverride();
    room_trig =  GetEnt("aq_dg_trophy_room_trig", "targetname");
    room_trig notify("trigger", self);
    level flag::set("demon_gate_crawlers");
    level notify("override_finish");
}

VoidCollectRunes()
{
    if(CheckQuestProgress("demon") >= 5) return;
    while(CheckQuestProgress("demon") < 4) wait 0.05;
    if(level.var_6e68c0d8 != self) level.var_6e68c0d8 = self;
    thread DisableOverride();
    level flag::set("demon_gate_runes");
    arrow = struct::get("quest_reforge_demon_gate", "targetname");
    while(!IsDefined(arrow.var_67b5dd94)) wait 0.05;
    arrow.var_67b5dd94 notify("trigger", self);
    wait 0.5;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_demon_arrow_broken_reform_bundle", 0, 0, 0);
    level waittill(#"hash_66b2458c");
    wait 1;
    arrow.var_67b5dd94 notify("trigger", self);
    level notify("override_finish");
}

VoidBuildBow()
{
    if(CheckQuestProgress("demon") >= 6) return;
    soulbox = struct::get("upgraded_bow_struct_demon_gate", "targetname");
    while(!IsDefined(soulbox.var_67b5dd94)) wait 0.05;
    if(level.var_6e68c0d8 != self) level.var_6e68c0d8 = self;
    soulbox.var_67b5dd94 notify("trigger", self);
    level flag::set("demon_gate_upgraded");
    wait 3;
    soulbox.var_67b5dd94 notify("trigger", self);
    wait 0.5;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_upgrade_bundle_demon_gate", 0, 0, 0);
    wait 0.5;
    soulbox.var_67b5dd94 notify("trigger", self);
}

DisableOverride()
{
    level endon("override_finish");
    for(;;)
    {
        level.sndvoxoverride = 0;
        wait 0.05;
    }
}