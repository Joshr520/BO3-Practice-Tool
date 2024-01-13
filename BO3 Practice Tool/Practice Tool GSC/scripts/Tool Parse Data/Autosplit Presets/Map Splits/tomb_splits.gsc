SplitStaffCraft(split_name)
{
    name = StrTok(split_name, " ")[0];
    switch (name) {
        case "Ice": {
            staff = "water";
            break;
        }
        case "Wind": {
            staff = "air";
            break;
        }
        case "Fire":
        case "Lightning": {
            staff = ToLower(name);
            break;
        }
        default: {
            return;
        }
    }
    level waittill("elemental_staff_" + staff + "_crafted", player);
}

SplitTombTP(split_name)
{
    switch (StrTok(split_name, " ")[0]) {
        case "Ice": {
            num = 4;
            break;
        }
        case "Lightning": {
            num = 3;
            break;
        }
        case "Wind": {
            num = 2;
            break;
        }
        case "Fire": {
            num = 1;
            break;
        }
        default: {
            return;
        }
    }

    if (IsSubStr(split_name, "Enter")) {
        for (;;) {
            level waittill("player_teleported", e_player, script_int);
            if (num == script_int) {
                return;
            }
        }
    }
    name = StrTok(split_name, " ")[0];
    switch (name) {
        case "Ice":
        case "Fire": {
            staff = ToLower(name);
            break;
        }
        case "Wind": {
            staff = "air";
            break;
        }
        case "Lightning": {
            staff = "electric";
            break;
        }
        default: {
            return;
        }
    }
    flag = "portal_exit_frame_" + staff + "_building";
    level flag::wait_till(flag);
    level flag::wait_till_clear(flag);
    portal = level.a_teleport_exits[num];
    radius_sq = 120 * 120;
    for (;;) {
        foreach(player in GetPlayers()) {
			dist_sq = DistanceSquared(player.origin, portal.origin);
			if(dist_sq < radius_sq && player GetStance() != "prone" && !(IsTrue(player.teleporting))) {
				return;
			}
		}
        wait 0.05;
    }
}

SplitBoxes(split_name)
{
    level waittill("boxes_finished");
}

autoexec CountTombBoxes()
{
    system::wait_till("all");
    array::thread_all(GetEntArray("foot_box", "script_noteworthy"), ::WaitBoxFinish);
}

WaitBoxFinish()
{
    self waittill("movedone");
    if (level.n_soul_boxes_completed >= 4) {
        level notify("boxes_finished");
    }
}

SplitFists(split_name)
{
    level flag::wait_till("ee_all_players_upgraded_punch");
}

SplitTombEnd()
{
    level.players[0] waittill("_screen_fade_starting_ee_screen");
}