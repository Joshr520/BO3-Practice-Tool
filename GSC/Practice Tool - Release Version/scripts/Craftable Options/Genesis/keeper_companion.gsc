PickupKeeperParts()
{
    if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Keeper Parts");

    ShootCharacterStones();
    wait 1;

    totem_locs = struct::get_array("companion_totem_part", "targetname");
    foreach(totem in totem_locs)
    {
        if(IsDefined(totem.var_fdb628a4)) totem notify("trigger_activated", self);
    }

    head_locs = struct::get_array("companion_head_part", "targetname");
    foreach(head in head_locs)
    {
        if(IsDefined(head.var_fdb628a4)) head notify("trigger_activated", self);
    }

    gem_locs = struct::get_array("companion_gem_part", "targetname");
    foreach(gem in gem_locs)
    {
        if(IsDefined(gem.var_fdb628a4)) gem notify("trigger_activated", self);
    }
}

PickupKeeperPart(part)
{
    if(!level flag::get("character_stones_done"))
    {
        ShootCharacterStones();
    }

    wait 1;

    thread WriteToScreen("Picking Up Keeper " + ToUpper(GetSubStr(part, 0, 1)) + GetSubStr(part, 1));

    locs = struct::get_array("companion_" + part + "_part", "targetname");
    foreach(loc in locs)
    {
        if(IsDefined(loc.var_fdb628a4)) loc notify("trigger_activated", self);
    }
}