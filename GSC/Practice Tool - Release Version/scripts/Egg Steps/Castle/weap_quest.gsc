// 0 = Lower Courtyard - 1 = Church - 2 = Undercroft
FillDragon(nums)
{   
    level.var_63e17dd5 = self;
    foreach(num in nums)
    {
        level.soul_catchers[num] notify("first_zombie_killed_in_zone", self);
        level.soul_catchers[num].var_98730ffa = 8;
    }
}

PickupBow()
{
    level flag::wait_till("soul_catchers_charged");
    wait 1;
    level.var_15acc392 = GetEnt("base_bow_pickup", "targetname");
    while(!IsDefined(level.var_15acc392))
    {
        level.var_15acc392 = GetEnt("base_bow_pickup", "targetname");
        wait 0.05;
    }
    bow = struct::get("base_bow_pickup_struct", "targetname");
    self BuildAndActivateTrigger(bow.var_67b5dd94);
}