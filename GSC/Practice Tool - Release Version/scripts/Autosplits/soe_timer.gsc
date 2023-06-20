

WaitForRitual(name)
{
    level waittill("ritual_" + name + "_succeed");
}

// // Statue Order - Canals, Footlight, Initial, Waterfront, Rift
WaitForEgg(num)
{
    for(;;)
    {
        level.sword_quest.statues[num].trigger waittill("trigger", player);
        if(player.sword_quest.kills[num] >= 12) return;
    }
}

WaitForSword(upgraded)
{
    self waittill(#"hash_b29853d8");
    if(!upgraded && self.sword_quest.upgrade_stage == 1) return;
    else self waittill(#"hash_b29853d8");
}

// 0 = junction, 1 = canals, 2 = footlight, 3 = waterfront
WaitForOvum(num)
{
    for(;;)
    {
        level flag::wait_till("magic_circle_in_progress");
        wait 0.05;
        foreach(player in GetPlayers())
        {
            if(level clientfield::get("keeper_egg_location_" + player.characterindex) == num) return;
        }
        level flag::wait_till_clear("magic_circle_in_progress");
    }
}

WaitForBook()
{
    level flag::wait_till("ee_book");
}

WaitForFlag(num)
{
    for(;;)
    {
        level waittill("ee_keeper_resurrected");
        if([[ @zm_zod_ee<scripts\zm\zm_zod_ee.gsc>::function_832f1b2a ]]() >= num) return;
    }
}