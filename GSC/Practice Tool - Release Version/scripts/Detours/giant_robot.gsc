detour zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::robot_cycling()
{
	three_robot_round = 0;
	last_robot = -1;
	level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_intro_walk ]](1);
	level waittill("giant_robot_intro_complete");
	while(true)
	{
		if(!(level.round_number % 4) && three_robot_round != level.round_number)
		{
			level flag::set("three_robot_round");
		}
		if(level flag::get("ee_all_staffs_placed") && !level flag::get("ee_mech_zombie_hole_opened"))
		{
			level flag::set("three_robot_round");
		}
		if(level flag::get("three_robot_round"))
		{
			level.zombie_ai_limit = 22;
			random_number = RandomInt(3);
            if(level flag::get("tomb_soft_patch"))
            {
                switch(level.staff_part_count["elemental_staff_air"])
                {
                    case 3:
                        random_number = 1;
                        break;
                    case 2:
                        random_number = 2;
                        break;
                    case 1:
                        random_number = 0;
                        break;
                    default:
                        random_number = RandomInt(3);
                        break;
                }
                if(level flag::get("ee_all_staffs_placed")) random_number = 0;
            }
			if(random_number == 2 || level flag::get("all_robot_hatch"))
			{
				level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](2);
			}
			else
			{
				level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](2, 0);
			}
			wait(5);
			if(random_number == 0 || level flag::get("all_robot_hatch"))
			{
				level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](0);
			}
			else
			{
				level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](0, 0);
			}
			wait(5);
			if(random_number == 1 || level flag::get("all_robot_hatch"))
			{
				level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](1);
			}
			else
			{
				level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](1, 0);
			}
			level waittill("giant_robot_walk_cycle_complete");
			level waittill("giant_robot_walk_cycle_complete");
			level waittill("giant_robot_walk_cycle_complete");
			wait(5);
			level.zombie_ai_limit = 24;
			three_robot_round = level.round_number;
			last_robot = -1;
			level flag::clear("three_robot_round");
		}
		else
		{
			if(!level flag::get("activate_zone_nml")) random_number = RandomInt(2);
			else
            {
                while(random_number == last_robot)
                {
                    random_number = RandomInt(3);
                }
            }
            if(level flag::get("tomb_soft_patch"))
            {
                switch(level.staff_part_count["elemental_staff_air"])
                {
                    case 3:
                        random_number = 1;
                        break;
                    case 2:
                        random_number = 2;
                        break;
                    case 1:
                        random_number = 0;
                        break;
					default:
						random_number = RandomInt(3);
						while(random_number == last_robot)
						{
							random_number = RandomInt(3);
						}
						break;
                }
            }
			last_robot = random_number;
			level thread [[ @zm_tomb_giant_robot<scripts\zm\zm_tomb_giant_robot.gsc>::giant_robot_start_walk ]](random_number);
			level waittill("giant_robot_walk_cycle_complete");
			wait(5);
		}
	}
}