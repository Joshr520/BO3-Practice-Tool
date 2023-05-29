FinishWolf()
{
    self ActivateWolfQuest();
    self WolfShootShrine();
    self WolfEscortWolf();
    self WolfForgeArrow();
    self WolfBuildBow();
}

ActivateWolfQuest()
{
    if(CheckQuestProgress("wolf") >= 1) return;
    level flag::set("wolf_howl_paintings");
    if(!IsDefined(struct::get("quest_start_demon_gate").var_67b5dd94))
    {
        arrow_found = 0;
        while(!arrow_found)
        {
            arrow = struct::get("quest_start_wolf_howl");
            if(!IsDefined(arrow)) continue;
            foreach(stub in level._unitriggers.trigger_stubs)
            {
                if(stub.origin == (arrow.origin + (-12, -72, 0)))
                {
                    arrow_found = 1;
                    self BuildAndActivateTrigger(stub);
                    break;
                }
            }
            wait 0.05;
        }

        level waittill(#"hash_44c83018");
        wait 1;
    }

    arrow = struct::get("quest_start_wolf_howl");
    self BuildAndActivateTrigger(arrow.var_67b5dd94);
}

WolfShootShrine()
{
    if(CheckQuestProgress("wolf") >= 2) return;
    while(CheckQuestProgress("wolf") < 1) wait 0.05;
    if(level.var_52978d72 != self) level.var_52978d72 = self;
    shrine = GetEnt("aq_wh_skull_shrine_trig", "targetname");
    self notify("projectile_impact", GetWeapon("elemental_bow"), (5334.59, -1685.42, -1247.15), 0, undefined, (-0.817539, -0.298289, 0.492599));
    skull = GetEnt("wolf_skull_roll_down", "targetname");
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_wolf_skull_roll_down_bundle", 0, 0, 0);
    while(!IsDefined(skull.var_67b5dd94)) wait 0.05;
    wait 1;
    skull.var_67b5dd94 notify("trigger", self);
}

WolfEscortWolf()
{
    if(CheckQuestProgress("wolf") >= 4) return;
    while(IsDefined(GetEnt("wolf_skull_roll_down", "targetname"))) wait 0.05;
    if(level.var_52978d72 != self) level.var_52978d72 = self;
    wait 1;
    skull = GetEnt("aq_wh_skadi_skull", "targetname");
    skull.var_67b5dd94 notify("trigger", self);
    wait 0.1;
    org = self.origin;
    level.var_e6d07014 scene::skipto_end("ai_zm_dlc1_wolf_howl_entry", Array(level.var_e6d07014));
    digs = Array("aq_wh_dig_struct_courtyard", "aq_wh_dig_struct_road", "aq_wh_dig_struct_undercroft");
    foreach(dig in digs)
    {
        if(dig == "aq_wh_dig_struct_road" && !zm_zonemgr::zone_is_enabled("zone_tram_to_gatehouse"))
        {
            disable = 1;
            level.zones["zone_tram_to_gatehouse"].is_enabled = 1;
        }
        target = struct::get(dig, "targetname");
        level.var_e6d07014 ForceTeleport(target.origin);
        self SetOrigin(target.origin + VectorScale((1, 0, 0), 128));
        soul_dig = GetEnt("aq_wh_dig_volume_" + target.script_label, "targetname");
        soul_dig flag::set("dig_spot_complete");
        wait 0.1;
        bones = GetEnt("aq_wh_bones_" + target.script_label, "targetname");
        while(!IsDefined(bones.var_67b5dd94)) wait 0.05;
        bones.var_67b5dd94 notify("trigger", self);
        if(IsDefined(disable))
        {
            disable = undefined;
            level.zones["zone_tram_to_gatehouse"].is_enabled = 0;
        }
    }
    self Unlink();
    self SetOrigin(org);
}

WolfForgeArrow()
{
    if(CheckQuestProgress("wolf") >= 5) return;
    while(CheckQuestProgress("wolf") < 4) wait 0.05;
    if(level.var_52978d72 != self) level.var_52978d72 = self;
    level notify("stop_skip_scene");
    damage_trig = GetEnt("aq_wh_burial_chamber_damage_trig", "targetname");
    self.var_374fd3ef = 1;
    damage_trig notify("damage", 1, self, (-7.44025, 13.5862, 21.2339), (478.44, 2314.41, 441.766), "MOD_EXPLOSIVE", "", "", "", GetWeapon("elemental_bow"));
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_wolf_platform_rebuild_bundle", 0, 0, 0);
    arrow = struct::get("quest_reforge_wolf_howl", "targetname");
    while(!IsDefined(arrow.var_67b5dd94)) wait 0.05;
    wait 0.5;
    arrow.var_67b5dd94 notify("trigger", self);
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_wolf_king_skeleton_bundle", 0, 0, 0);
    wait 0.5;
    arrow.var_67b5dd94 notify("trigger", self);
}

WolfBuildBow()
{
    if(CheckQuestProgress("wolf") >= 6) return;
    soulbox = struct::get("upgraded_bow_struct_wolf_howl", "targetname");
    while(!IsDefined(soulbox.var_67b5dd94)) wait 0.05;
    if(level.var_52978d72 != self) level.var_52978d72 = self;
    soulbox.var_67b5dd94 notify("trigger", self);
    level flag::set("wolf_howl_upgraded");
    wait 3;
    soulbox.var_67b5dd94 notify("trigger", self);
    wait 0.5;
    level scene::skip_scene("p7_fxanim_zm_castle_quest_upgrade_bundle_wolf_howl", 0, 0, 0);
    wait 0.5;
    soulbox.var_67b5dd94 notify("trigger", self);
}