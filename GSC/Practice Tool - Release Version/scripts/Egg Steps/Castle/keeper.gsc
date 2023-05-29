ActivateDempsey()
{
    if(level flag::get("start_channeling_stone_step")) return;
    while(!IsDefined(struct::get("death_ray_button").s_unitrigger)) wait 0.05;
    wait 0.5;
    button = struct::get("death_ray_button");
    button notify("trigger_activated");
}

ActivateKeeper()
{
    while(!level flag::get("start_channeling_stone_step")) wait 0.05;
    vril = struct::get("vril_generator");
    while(!IsDefined(vril.s_unitrigger)) wait 0.05;
    foreach(stub in level._unitriggers.trigger_stubs)
	{
        if(vril.origin == stub.origin)
        {
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
    wait 0.5;
    vril_family = struct::get("vril_generator_family");
    vril_family notify("trigger_activated");
}

FillKeeperSouls()
{
    if(level flag::get("see_keeper")) return;
    level flag::wait_till("start_channeling_stone_step");
    stone_replace = GetEnt("cs_stone_2", "targetname");
    stone_replace notify("trigger_activated");
    while(!IsDefined(GetEnt("keeper_archon_ai", "targetname"))) wait 0.05;
    wait 2;
    for(i = 0; i < 4; i++)
    {
        stone = struct::get("cs_keeper_pos_" + level.var_f1b0baba);
        keeper = GetEnt("keeper_archon_ai", "targetname");
        keeper ForceTeleport(stone.origin);
        wait 1;
        keeper notify("start_moving");
        level.var_8bdb0713 = 100;
        wait 1.5;
    }
    keeper waittill("start_moving");
    stone = struct::get("keeper_end_loc");
    keeper ForceTeleport(stone.origin);
}