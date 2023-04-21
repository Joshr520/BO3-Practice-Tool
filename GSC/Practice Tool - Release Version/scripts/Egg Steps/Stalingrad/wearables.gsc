PickupWings()
{
    level flag::set("dragon_platforms_all_used");
    level flag::set("dragon_wings_items_aquired");

    wait 2;

    wings = struct::get("wearable_dragon_wings", "targetname").s_unitrigger;
    self thread BuildAndActivateTrigger(wings);
    if(!IsDefined(level.var_9d19c7e)) level.var_9d19c7e = "judicial";
}