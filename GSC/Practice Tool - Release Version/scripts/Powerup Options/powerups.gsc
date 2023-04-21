InitPowerups()
{
    level.custom_powerups = Array("full_ammo", "double_points", "insta_kill", "nuke", "minigun", "carpenter", "fire_sale", "free_perk", "bonus_points_player", "bonus_points_team", "ww_grenade");
    if(!IsDefined(level.zombie_powerups["bonus_points_team"])) ArrayRemoveValue(level.custom_powerups, "bonus_points_team");
    if(level.script == "zm_factory") ArrayRemoveValue(level.custom_powerups, "ww_grenade");
    if(IsDefined(level.zombie_powerups["shield_charge"])) ArrayInsert(level.custom_powerups, "shield_charge", level.custom_powerups.size);
    if(level.script == "zm_castle")
    {
        ArrayInsert(level.custom_powerups, "castle_tram_token", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "demonic_rune_lor", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "demonic_rune_ulla", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "demonic_rune_uja", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "demonic_rune_oth", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "demonic_rune_zor", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "demonic_rune_mar", level.custom_powerups.size);
    }
    if(IsDefined(level.zombie_powerups["empty_perk"])) ArrayInsert(level.custom_powerups, "empty_perk", level.custom_powerups.size);
    if(level.script == "zm_island") ArrayInsert(level.custom_powerups, "island_seed", level.custom_powerups.size);
    if(level.script == "zm_stalingrad")
    {
        ArrayInsert(level.custom_powerups, "code_cylinder_red", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "code_cylinder_yellow", level.custom_powerups.size);
        ArrayInsert(level.custom_powerups, "code_cylinder_blue", level.custom_powerups.size);
    }
    if(level.script == "zm_genesis") ArrayInsert(level.custom_powerups, "genesis_random_weapon", level.custom_powerups.size);
    if(level.script == "zm_tomb") ArrayInsert(level.custom_powerups, "zombie_blood", level.custom_powerups.size);
    level.powerup_names = []; level.powerup_names["full_ammo"] = "Max Ammo"; level.powerup_names["double_points"] = "Double Points"; level.powerup_names["insta_kill"] = "Insta Kill"; level.powerup_names["nuke"] = "Nuke"; level.powerup_names["minigun"] = "Death Machine";
    level.powerup_names["carpenter"] = "Carpenter"; level.powerup_names["fire_sale"] = "Fire Sale"; level.powerup_names["free_perk"] = "Free Perk"; level.powerup_names["bonus_points_player"] = "Bonus Points Player"; level.powerup_names["bonus_points_team"] = "Bonus Points Team";
    level.powerup_names["ww_grenade"] = "Widows Wine Grenade"; level.powerup_names["shield_charge"] = "Shield Charge"; level.powerup_names["castle_tram_token"] = "Tram Token"; level.powerup_names["demonic_rune_lor"] = "Void Symbol Tears";
    level.powerup_names["demonic_rune_ulla"] = "Void Symbol Rib"; level.powerup_names["demonic_rune_uja"] = "Void Symbol Triangles"; level.powerup_names["demonic_rune_oth"] = "Void Symbol Claw"; level.powerup_names["demonic_rune_zor"] = "Void Symbol Tear";
    level.powerup_names["demonic_rune_mar"] = "Void Symbol Ribbon"; level.powerup_names["empty_perk"] = "Perk Slot"; level.powerup_names["island_seed"] = "Seed"; level.powerup_names["code_cylinder_red"] = "Tank Factory Code Cylinder";
    level.powerup_names["code_cylinder_yellow"] = "Supply Dept Code Cylinder"; level.powerup_names["code_cylinder_blue"] = "Dragon Command Code Cylinder"; level.powerup_names["genesis_random_weapon"] = "Random Weapon Powerup"; level.powerup_names["zombie_blood"] = "Zombie Blood";
}

GivePowerup(num, insta_grab = 1)
{
    if(num >= level.custom_powerups.size)
    {
        thread WriteToScreen("Invalid Powerup Given");
        return;
    }
    powerup = level.custom_powerups[num];
    if(IsSubStr(powerup, "code_cylinder") && ! level flag::get("power_on"))
    {
        thread WriteToScreen("Power Required For Code Cylinders");
        return;
    }
    origin = self.origin;
    if(!insta_grab) origin = self.origin + VectorScale(AnglesToForward((0, self GetPlayerAngles()[1], 0)), 60) + VectorScale((0, 0, 1), 5);
    zm_powerups::specific_powerup_drop(powerup, origin);
    thread WriteToScreen("Spawning Powerup " + level.powerup_names[powerup]);
}