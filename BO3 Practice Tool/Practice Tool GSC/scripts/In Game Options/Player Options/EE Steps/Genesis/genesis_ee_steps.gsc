ShootCharacterStones()
{
    level.var_753f10ae[1] notify("trigger");
    wait 0.15;
    level.var_753f10ae[2] notify("trigger");
    wait 0.15;
    level.var_753f10ae[0] notify("trigger");
    wait 0.15;
    level.var_753f10ae[3] notify("trigger");
}

PickupKeeperPart(part)
{
    if (!level flag::get("character_stones_done")) {
        ShootCharacterStones();
        wait 1;
    }
    switch (part) {
        case "totem": {
            foreach (totem in struct::get_array("companion_totem_part", "targetname")) {
                if (IsDefined(totem.var_fdb628a4)) {
                    totem notify("trigger_activated", self);
                    return;
                }
            }
            break;
        }
        case "head": {
            foreach (head in struct::get_array("companion_head_part", "targetname")) {
                if (IsDefined(head.var_fdb628a4)) {
                    head notify("trigger_activated", self);
                    return;
                }
            }
            break;
        }
        case "gem": {
            foreach (gem in struct::get_array("companion_gem_part", "targetname")) {
                if (IsDefined(gem.var_fdb628a4)) {
                    gem notify("trigger_activated", self);
                    return;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}