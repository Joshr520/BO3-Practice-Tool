SkullFadeMap()
{
    map = GetEnt("mdl_main_ee_map", "targetname");
    self notify("someone_revealed_" + map.targetname);
}

PickupBullet()
{
    level flag::set("player_has_aa_gun_ammo");
}

ShootPlane()
{
    level flag::wait_till("trilogy_released");
    while(!GetVehicleArray("main_ee_aa_gun_plane_vh", "targetname").size) wait 0.05;
    level flag::set("aa_gun_ee_complete");
}

PickupGears()
{
    level flag::wait_till("trilogy_released");
    wait 1;
    gear_1 = struct::get("elevator_part_gear1");
    gear_2 = struct::get("elevator_part_gear2");
    gear_3 = struct::get_array("aa_gun_elevator_part_landing", "targetname");
    for(i = 0; i < 3; i++)
    {
        if(i == 2)
        {
            level flag::wait_till("aa_gun_ee_complete");
            wait 6;
        }
        ents = GetEntArray();
        wait_var = 0;
        for(j = ents.size - 1; j >= 0; j--)
        {
            wait_var++;
            if(wait_var == 100)
            {
                wait_var = 0;
                wait 0.05;
            }
            if(!IsDefined(ents[j])) continue;
            skip = 0;
            switch(i)
            {
                case 0:
                    if(gear_1.origin != ents[j].origin) skip = 1;
                    break;
                case 1:
                    if(gear_2.origin != ents[j].origin) skip = 1;
                    break;
                case 2:
                    if(gear_3[0].origin != ents[j].origin && gear_3[1].origin != ents[j].origin && gear_3[2].origin != ents[j].origin) skip = 1;
                    break;
            }
            if(skip) continue;
            ents[j].trigger notify("trigger", self);
        }
        wait 1;
    }
}

ElevatorFadeWall()
{
    level flag::wait_till("trilogy_released");
    wait 1;
    wall = GetEnt("main_ee_elevator_wall_metal", "targetname");
    self notify("someone_revealed_" + wall.targetname);
}

StartTakeoBoss()
{
    level flag::wait_till("trilogy_released");
    wait 1;
    self SetOrigin(GetEnt("takeo_arm_gate3", "targetname").origin);
    for(i = 1; i < 4; i++)
	{
		arm = GetEnt("takeo_arm_gate" + i, "targetname");
		e_clip = GetEnt(arm.target, "targetname");
        e_clip notify("damage", 1, self, (-1, 0, 0), (0, 0, 0), "MOD_GRENADE", "", "", "", level.var_a4052592);
        wait 0.1;
	}
    level flag::set("flag_init_takeo_fight");
}

DoAllZNSSteps()
{
    self thread FinishChallenges();
    self thread SkullFadeMap();
    self thread PickupBullet();
    self thread ShootPlane();
    self thread PickupGears();
    self thread ElevatorFadeWall();
    self thread StartTakeoBoss();
}