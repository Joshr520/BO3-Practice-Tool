WaitRitual(name)
{
    level flag::wait_till("ritual_" + name + "_complete");
}

WaitRiftTP(str_areaname, str_pos)
{
    portal = GetEntArray(str_areaname + "_portal_" + str_pos, "script_noteworthy");
    portal_trig = GetEntArrayFromArray(portal, "teleport_trigger", "targetname")[0];
    for(;;)
    {
        portal_trig waittill("trigger", portee);
        if(!level clientfield::get("portal_state_" + str_areaname) || !IsPlayer(portee)) continue;
        return;
    }
}

// // Statue Order - Canals, Footlight, Initial, Waterfront, Rift
WaitEgg(num)
{
    for(;;)
    {
        level.sword_quest.statues[num].trigger waittill("trigger", player);
        if(player.sword_quest.kills[num] >= 12) return;
    }
}

WaitSword(sword_state)
{
    for(;;)
    {
        self waittill(#"hash_b29853d8");
        if(self.sword_quest.upgrade_stage == sword_state) return;
    }
}

// 0 = waterfront, 1 = canals, 2 = footlight, 3 = junction
WaitForOvum(num)
{
    for(;;)
    {
        level flag::wait_till("magic_circle_in_progress");
        wait 0.05;
        foreach(player in GetPlayers()) if(level clientfield::get("keeper_egg_location_" + player.characterindex) == num) return;
        level flag::wait_till_clear("magic_circle_in_progress");
    }
}

WaitBook()
{
    level flag::wait_till("ee_book");
}

WaitFlag(num)
{
    for(;;)
    {
        level waittill("ee_keeper_resurrected");
        if(level [[ @zm_zod_ee<scripts\zm\zm_zod_ee.gsc>::function_832f1b2a ]]() >= num) return;
    }
}

WaitZodEnd()
{
    if(GetPlayers().size < 4) level flag::wait_till("ee_boss_defeated");
    else while(!level IsScenePlaying("cin_zod_vign_summoning_key")) wait 0.05;
}