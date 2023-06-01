WaitStaffCraft(num)
{
    stalves = Array("water", "air", "fire", "lightning");
    level waittill("elemental_staff_" + stalves[num] + "_crafted", player);
}

WaitUpgrade()
{
    level flag::wait_till("ee_all_staffs_upgraded");
    for(;;)
    {
        foreach(player in GetPlayers()) if(player.teleporting) return;
        wait 0.05;
    }
}

WaitBoxes()
{
	array::thread_all(GetEntArray("foot_box", "script_noteworthy"), ::WaitBoxDone);
    level waittill("all_boxes_completed");
}

WaitBoxDone()
{
    level endon("all_boxes_completed");
    if(!IsDefined(self)) return;
    self waittill("movedone");
    if(level.n_soul_boxes_completed == 4) level notify("all_boxes_completed");
}

WaitFists()
{
    level flag::wait_till("ee_all_players_upgraded_punch");
}

WaitTombEnd()
{
    self waittill("_screen_fade_starting_ee_screen");
}