InitWeapons()
{
    level.practice_weapon_list = [];
    level.practice_weapon_list["ar"] = [];
    level.practice_weapon_list["smg"] = [];
    level.practice_weapon_list["lmg"] = [];
    level.practice_weapon_list["shotgun"] = [];
    level.practice_weapon_list["sniper"] = [];
    level.practice_weapon_list["pistol"] = [];
    level.practice_weapon_list["launcher"] = [];
    level.practice_weapon_list["melee"] = Array("bowie_knife");
    level.practice_weapon_list["special"] = [];
    level.practice_weapon_list["equipment"] = [];
    level.practice_weapon_list["hero"] = [];
    includes = Array("ar", "smg", "lmg", "shotgun", "sniper", "pistol", "launcher", "melee");
    table = "gamedata/weapons/zm/" + level.script + "_weapons.csv";
    index = 1;
	row = TableLookupRow(table, index);
    while(IsDefined(row))
    {
        index++;
        tokens = StrToK(row[0], "_");
        if(!IsInArray(includes, tokens[0]) || IsSubStr(row[0], "dragon") || GetWeapon(row[0]) == level.weaponnone)
        {
            row = TableLookupRow(table, index);
            continue;
        }
        if(!IsDefined(level.practice_weapon_list[tokens[0]])) level.practice_weapon_list[tokens[0]] = [];
        ArrayInsert(level.practice_weapon_list[tokens[0]], row[0], level.practice_weapon_list[tokens[0]].size);
        row = TableLookupRow(table, index);
    }

    switch(level.script)
    {
        case "zm_zod":
            level.practice_weapon_list["special"] = Array("idgun_", "ray_gun");
            level.practice_weapon_list["hero"] = Array("glaive_apothicon_");
            break;
        case "zm_factory":
            level.practice_weapon_list["special"] = Array("tesla_gun", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_castle":
            ArrayInsert(level.practice_weapon_list["melee"], "knife_plunger", level.practice_weapon_list["melee"].size);
            level.practice_weapon_list["special"] = Array("elemental_bow", "elemental_bow_storm", "elemental_bow_rune_prison", "elemental_bow_demongate", "elemental_bow_wolf_howl", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_gravityspikes_melee");
            break;
        case "zm_island":
            level.practice_weapon_list["special"] = Array("hero_mirg2000", "ray_gun");
            level.practice_weapon_list["hero"] = Array("skull_gun");
            break;
        case "zm_stalingrad":
            level.practice_weapon_list["special"] = Array("raygun_mark3", "ray_gun", "special_crossbow_dw");
            level.practice_weapon_list["equipment"] = Array("launcher_dragon_strike");
            level.practice_weapon_list["hero"] = Array("dragon_gauntlet");
            break;
        case "zm_genesis":
            level.practice_weapon_list["special"] = Array("thundergun", "idgun_genesis_0", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_gravityspikes_melee");
            break;
        case "zm_prototype":
            level.practice_weapon_list["special"] = Array("raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_asylum":
            level.practice_weapon_list["special"] = Array("tesla_gun", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_sumpf":
            level.practice_weapon_list["special"] = Array("tesla_gun", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_theater":
            level.practice_weapon_list["special"] = Array("thundergun", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_cosmodrome":
            level.practice_weapon_list["special"] = Array("thundergun", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["melee"][0] = "sickle_knife";
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_temple":
            level.practice_weapon_list["special"] = Array("shrink_ray", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_moon":
            ArrayInsert(level.practice_weapon_list["equipment"], "equip_gasmask", level.practice_weapon_list["equipment"].size);
            ArrayInsert(level.practice_weapon_list["equipment"], "equip_hacker", level.practice_weapon_list["equipment"].size);
            level.practice_weapon_list["special"] = Array("microwavegundw", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
        case "zm_tomb":
            level.practice_weapon_list["special"] = Array("staff_water", "staff_lightning", "staff_fire", "staff_air", "raygun_mark2", "ray_gun");
            level.practice_weapon_list["hero"] = Array("hero_annihilator");
            break;
    }

    if(IsDefined(level.w_octobomb)) ArrayInsert(level.practice_weapon_list["equipment"], "octobomb", level.practice_weapon_list["equipment"].size);
    if(IsDefined(level.weaponzmcymbalmonkey)) ArrayInsert(level.practice_weapon_list["equipment"], "cymbal_monkey", level.practice_weapon_list["equipment"].size);
    if(IsDefined(level.w_beacon)) ArrayInsert(level.practice_weapon_list["equipment"], "beacon", level.practice_weapon_list["equipment"].size);
    if(IsDefined(level.w_black_hole_bomb)) ArrayInsert(level.practice_weapon_list["equipment"], "black_hole_bomb", level.practice_weapon_list["equipment"].size);
    if(IsDefined(level.w_nesting_dolls)) ArrayInsert(level.practice_weapon_list["equipment"], "nesting_dolls", level.practice_weapon_list["equipment"].size); // Needs weapon limit override
    if(IsDefined(level.w_quantum_bomb)) ArrayInsert(level.practice_weapon_list["equipment"], "quantum_bomb", level.practice_weapon_list["equipment"].size);
    if(IsDefined(level.placeable_mines["bouncingbetty"])) ArrayInsert(level.practice_weapon_list["equipment"], "bouncingbetty", level.practice_weapon_list["equipment"].size);
    ArrayInsert(level.practice_weapon_list["equipment"], "frag_grenade", level.practice_weapon_list["equipment"].size);
    if(IsDefined(level.weaponriotshield) && level.weaponriotshield != level.weaponnone) ArrayInsert(level.practice_weapon_list["equipment"], "shield", level.practice_weapon_list["equipment"].size);
    if(level.script == "zm_tomb") ArrayInsert(level.practice_weapon_list["equipment"], "shield", level.practice_weapon_list["equipment"].size);
    if(!zm_utility::is_melee_weapon(GetWeapon("bowie_knife"))) zm_utility::register_melee_weapon_for_level("bowie_knife");
}

GivePlayerWeapon(list, index, upgraded = 0)
{
    while(!self WeaponAvailableValidation()) wait 0.5;
    keys = [];
    foreach(key, arr in level.practice_weapon_list) keys[keys.size] = key;
    list = keys[list];
    wpn_name = level.practice_weapon_list[list][index];
    if(wpn_name == "shield")
    {
        weapon = level.weaponriotshield;
        if(upgraded)
        {
            if(IsDefined(level.weaponriotshieldupgraded)) weapon = level.weaponriotshieldupgraded;
            else
            {
                thread WriteToScreen("No Weapon Upgrade Available");
                return;
            }
        }
        if(self HasWeapon(weapon))
        {
            health = self clientfield::get_player_uimodel("zmInventory.shield_health");
            ammo = self GetWeaponAmmoClip(weapon);
            if(health == 1 && ammo == weapon.maxammo)
            {
                thread WriteToScreen("Shield Already Full");
                return;
            }
            if(health < 1) self clientfield::set_player_uimodel("zmInventory.shield_health", 1);
            if(ammo < weapon.maxammo)
            {
                self SetWeaponAmmoClip(weapon, weapon.maxammo);
                self notify("equipment_ammo_changed", weapon);
            }
            thread WriteToScreen("Refilling Shield");
            return;
        }
        thread WriteToScreen("Giving Shield " + MakeLocalizedString(weapon.displayName));
        self zm_weapons::weapon_give(weapon);
        return;
    }
    if(wpn_name == "idgun_")
    {
        wpn_name = wpn_name + self.characterindex;
        if(upgraded)
        {
            upgraded = 0;
            if(self HasWeapon(GetWeapon(wpn_name))) self TakeWeaponCheck(GetWeapon(wpn_name));
            wpn_name = "idgun_upgraded_" + self.characterindex;
        }
        if(self HasWeapon(GetWeapon("idgun_upgraded_" + self.characterindex))) self TakeWeaponCheck(GetWeapon("idgun_upgraded_" + self.characterindex));
        skip_check = 1;
    }
    else if(wpn_name == "glaive_apothicon_")
    {
        if(!upgraded) wpn_name = wpn_name + self.characterindex;
        else
        {
            wpn_name = "glaive_keeper_" + self.characterindex;
            upgraded = 0;
        }
    }
    else if(wpn_name == "hero_mirg2000")
    {
        if(upgraded) if(self HasWeapon(GetWeapon("hero_mirg2000"))) self TakeWeaponCheck(GetWeapon("hero_mirg2000"));
        if(!upgraded && self HasWeapon(GetWeapon("hero_mirg2000_upgraded")))
        {
            self TakeWeaponCheck(GetWeapon("hero_mirg2000_upgraded"));
        }
    }
    if(wpn_name == "idgun_genesis_0")
    {
        if(upgraded)
        {
            upgraded = 0;
            if(self HasWeapon(GetWeapon(wpn_name))) self TakeWeaponCheck(GetWeapon(wpn_name));
            wpn_name = wpn_name + "_upgraded";
        }
        if(self HasWeapon(GetWeapon(wpn_name + "_upgraded"))) self TakeWeaponCheck(GetWeapon(wpn_name + "_upgraded"));
        skip_check = 1;
    }
    if(IsSubStr(wpn_name, "staff") && upgraded && !self HasWeapon(GetWeapon("staff_revive"))) self zm_weapons::weapon_give(GetWeapon("staff_revive"));
    weapon = level.zombie_weapons[GetWeapon(wpn_name)].weapon;
    if(weapon == level.weaponnone || !IsDefined(weapon)) weapon = GetWeapon(wpn_name);
    upgrade_weapon = zm_weapons::get_upgrade_weapon(weapon);
    if(upgrade_weapon == level.weaponnone || !IsDefined(upgrade_weapon)) upgrade_weapon = GetWeapon(wpn_name + "_upgraded");
    if(upgraded && (upgrade_weapon == level.weaponnone || !IsDefined(upgrade_weapon)))
    {
        thread WriteToScreen("No Weapon Upgrade Available");
        return;
    }
    else if(upgraded) weapon = upgrade_weapon;
    if(zm_utility::is_lethal_grenade(weapon))
    {
        weapon = self zm_utility::get_player_lethal_grenade();
        frac = self GetFractionMaxAmmo(weapon);
        if(frac == 1)
        {
            thread WriteToScreen("Already Have Max Lethal Grenades");
            return;
        }
        thread WriteToScreen("Giving Max Lethal Grenades");
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    if(zm_utility::is_tactical_grenade(weapon))
    {
        if(!self HasWeapon(weapon))
        {
            thread WriteToScreen("Giving Weapon " + MakeLocalizedString(weapon.displayName));
            self zm_weapons::weapon_give(weapon);
            return;
        }
        frac = self GetFractionMaxAmmo(weapon);
        if(frac == 1)
        {
            thread WriteToScreen("Already Have Max Tactical Grenades");
            return;
        }
        thread WriteToScreen("Giving Max Tactical Grenades");
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    if(IsDefined(level.placeable_mines[wpn_name]))
    {
        displayName = MakeLocalizedString(weapon.displayName);
        if(wpn_name == "launcher_dragon_strike")
        {
            displayName = "Dragon Strike";
            if(upgraded) displayName = "Dragon Strike Upgraded";
        }
        if(!self HasWeapon(weapon))
        {
            thread WriteToScreen("Giving Weapon " + displayName);
            self zm_weapons::weapon_give(weapon);
            return;
        }
        ammo = self GetWeaponAmmoClip(weapon);
        if(ammo == weapon.clipsize)
        {
            thread WriteToScreen("Already Have Max Mines");
            return;
        }
        thread WriteToScreen("Giving Max Mines");
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    if(zm_utility::is_hero_weapon(weapon))
    {
        if(!self HasWeapon(weapon))
        {
            apoth = "glaive_apothicon_" + self.characterindex;
            keep = "glaive_keeper_" + self.characterindex;
            switch(wpn_name)
            {
                case apoth:
                    self thread GiveSword(weapon, 1);
                    break;
                case keep:
                    self thread GiveSword(weapon, 2);
                    break;
                case "hero_annihilator":
                    self thread GiveAnnihilator();
                    break;
                case "hero_gravityspikes_melee":
                    self thread GiveRags(weapon);
                    break;
                case "skull_gun":
                    self thread GiveSkull(weapon);
                    break;
                case "dragon_gauntlet":
                    self thread GiveGauntlet();
                    break;
            }
            thread WriteToScreen("Giving Weapon " + MakeLocalizedString(weapon.displayName));
            return;
        }
        power = self GadgetPowerGet(0);
        if(power == 100)
        {
            thread WriteToScreen("Already Have Max Hero Power");
            return;
        }
        thread WriteToScreen("Giving Max Hero Power");
        self GadgetPowerSet(0, 100);
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    if(zm_utility::is_melee_weapon(weapon))
    {
        displayName = weapon.displayName;
        if(wpn_name == "knife_plunger") displayName = "Plunger";
        if(!self HasWeapon(weapon))
        {
            thread WriteToScreen("Giving Weapon " + MakeLocalizedString(displayName));
            self zm_weapons::weapon_give(weapon);
            return;
        }
        else
        {
            thread WriteToScreen("Already Have Weapon " + MakeLocalizedString(displayName));
            return;
        }
    }
    if((WeaponInList(wpn_name) || IsDefined(skip_check)))
    {
        if(self zm_weapons::has_weapon_or_attachments(weapon))
        {
            weapon = self GetRootWeapon(weapon);
            ammo_clip = self GetWeaponAmmoClip(weapon);
            ammo_stock = self GetWeaponAmmoStock(weapon);
            if(IsDefined(weapon.dualwieldweapon)) ammo_stock += self GetWeaponAmmoClip(weapon.dualwieldweapon);
            max_ammo = weapon.maxammo + weapon.clipsize;
            if((ammo_clip + ammo_stock) >= max_ammo)
            {
                thread WriteToScreen("Already Max Ammo For " + MakeLocalizedString(weapon.displayName));
                return;
            }
            thread WriteToScreen("Giving Max Ammo For " + MakeLocalizedString(weapon.displayName));
            self SetWeaponAmmoClip(weapon, weapon.clipsize);
            self SetWeaponAmmoStock(weapon, weapon.maxammo);
            return;
        }
        if(self zm_weapons::has_weapon_or_attachments(weapon)) self TakeWeaponCheck(weapon);
        else if(self zm_weapons::has_weapon_or_attachments(zm_weapons::get_upgrade_weapon(weapon))) self TakeWeaponCheck(zm_weapons::get_upgrade_weapon(weapon));
        else if(self zm_weapons::has_weapon_or_attachments(zm_weapons::get_base_weapon(weapon))) self TakeWeaponCheck(zm_weapons::get_base_weapon(weapon));
        thread WriteToScreen("Giving Weapon " + MakeLocalizedString(weapon.displayName));
        if(IsSubStr(wpn_name, "ray") && IsSubStr(wpn_name, "gun"))
        {
            foreach(wpn in self GetWeaponsListPrimaries())
            {
                if(IsSubStr(wpn.name, "ray") && IsSubStr(wpn.name, "gun") && wpn != weapon)
                {
                    self TakeWeaponCheck(wpn);
                    break;
                }
            }
        }
        self zm_weapons::weapon_give(weapon);
        return;
    }
    if(zm_equipment::is_equipment(weapon))
    {
        if(self zm_equipment::has_player_equipment(weapon))
        {
            thread WriteToScreen("Already Have Equipment " + MakeLocalizedString(weapon.displayName));
            return;
        }
        thread WriteToScreen("Giving Equipment " + MakeLocalizedString(weapon.displayName));
        self zm_equipment::give(weapon);
        return;
    }
}

TakePlayerWeapon(num)
{
    switch(num)
    {
        case 0:
            weapon = self GetCurrentWeapon();
            if(!IsInArray(self GetWeaponsListPrimaries(), weapon))
            {
                thread WriteToScreen("No Weapon To Take");
                return;
            }
            while(!self WeaponAvailableValidation()) wait 0.5;
            thread WriteToScreen("Taking Weapon " + MakeLocalizedString(weapon.displayName));
            self TakeWeapon(weapon);
            break;
        case 1:
            weapon = self zm_utility::get_player_lethal_grenade();
            if(!self GetWeaponAmmoClip(weapon))
            {
                thread WriteToScreen("No Lethal Grenades To Take");
                return;
            }
            thread WriteToScreen("Taking Lethal Grenades");
            self SetWeaponAmmoClip(weapon, 0);
            break;
        case 2:
            weapon = self zm_utility::get_player_tactical_grenade();
            if(!self GetWeaponAmmoClip(weapon))
            {
                thread WriteToScreen("No Tactical Grenades To Take");
                return;
            }
            thread WriteToScreen("Taking Tactical Grenades");
            self SetWeaponAmmoClip(weapon, 0);
            break;
        case 3:
            weapon = self.weaponriotshield;
            if(!self HasWeapon(self.weaponriotshield) && !self HasWeapon(GetWeapon("equip_gasmask")) && !self HasWeapon(GetWeapon("equip_hacker")))
            {
                thread WriteToScreen("No Shield Slot To Take");
                return;
            }
            if(self HasWeapon(self.weaponriotshield))
            {
                self zm_equipment::take(self.weaponriotshield);
                self clientfield::set_player_uimodel("hudItems.showDpadDown", 0);
            }
            else if(self HasWeapon(GetWeapon("equip_gasmask"))) weapon = GetWeapon("equip_gasmask");
            else if(self HasWeapon(GetWeapon("equip_hacker"))) weapon = GetWeapon("equip_hacker");
            thread WriteToScreen("Taking Shield Slot");
            self zm_equipment::take(weapon);
            break;
        case 4:
            weapon = level.weaponbasemelee;
            if(self HasPerk("specialty_widowswine")) weapon = level.w_widows_wine_knife;
            if(self HasWeapon(weapon))
            {
                thread WriteToScreen("Melee Already Reset");
                return;
            }
            thread WriteToScreen("Resetting Melee");
            self zm_weapons::weapon_give(weapon);
            break;
        case 5:
            weapon = self.current_placeable_mine;
            if(weapon == level.weaponnone)
            {
                thread WriteToScreen("No Mine Slot To Take");
                return;
            }
            thread WriteToScreen("Taking Mine Slot");
            self zm_weapons::weapon_take(weapon);
            self.current_placeable_mine = level.weaponnone;
            break;
        case 6:
            weapon = self.current_hero_weapon;
            if(weapon == level.weaponnone || !IsDefined(weapon))
            {
                thread WriteToScreen("No Hero Slot To Take");
                return;
            }
            thread WriteToScreen("Taking Hero Slot");
            self zm_weapons::weapon_take(weapon);
            self.current_hero_weapon = level.weaponnone;
            break;
        case 7:
            if((level.script != "zm_moon" && level.script != "zm_tomb") || (!self HasWeapon(GetWeapon("staff_revive")) && !self HasWeapon(GetWeapon("microwavegun"))))
            {
                thread WriteToScreen("No Alt Slot To Take");
                return;
            }
            if(self HasWeapon(GetWeapon("staff_revive")))
            {
                thread WriteToScreen("Taking Alt Slot");
                self TakeWeapon(GetWeapon("staff_revive"));
                return;
            }
            if(self HasWeapon(GetWeapon("microwavegun")))
            {
                thread WriteToScreen("Taking Alt Slot");
                self TakeWeapon(GetWeapon("microwavegun"));
                return;
            }
            break;
    }
}

WeaponInList(wpn_name)
{
    if(IsDefined(level.zombie_weapons[GetWeapon(wpn_name)])) return true;
    else if(IsDefined(level.zombie_weapons_upgrade[zm_weapons::get_upgrade_weapon(GetWeapon(wpn_name))])) return true;
    return false;
}

TakeWeaponCheck(weapon)
{
    foreach(wpn in self GetWeaponsListPrimaries())
    {
        if(wpn.rootweapon == weapon.rootweapon)
        {
            weapon = wpn;
            break;
        }
    }
    while(!self WeaponAvailableValidation()) wait 0.5;
    self zm_weapons::weapon_take(weapon);
}

GetRootWeapon(weapon)
{
    foreach(wpn in self GetWeaponsListPrimaries())
    {
        if(wpn.rootweapon == weapon.rootweapon)
        {
            return wpn;
        }
    }
    return level.weaponnone;
}

GiveRiotshield(num)
{
    if(num) self zm_equipment::buy(level.weaponriotshieldupgraded);
    else self zm_equipment::buy(level.weaponRiotshield);
}

GiveSword(weapon, sword_level)
{
    prev_weapon = self GetCurrentWeapon();
    self zm_weapons::weapon_give(weapon, 0, 0, 1);
	self.current_sword = self.current_hero_weapon;
	self.sword_power = 1;
	self GadgetPowerSet(0, 100);
    self SwitchToweapon(weapon);
    self waittill("weapon_change_complete");
    wait(0.25);
    self thread SwordUseHint(sword_level);
    self SetLowReady(1);
    self SwitchToweapon(prev_weapon);
    self util::waittill_any_timeout(1.5, "weapon_change_complete", "disconnect");
    wait(0.25);
    self SetLowReady(0);
    self.sword_power = 1;
    self clientfield::set_player_uimodel("zmhud.swordEnergy", self.sword_power);
    self GadgetPowerSet(0, 100);
    self clientfield::increment_uimodel("zmhud.swordChargeUpdate");
}

SwordUseHint(sword_level)
{
	if(!IsDefined(self.var_75dcfb99))
	{
		self.var_75dcfb99 = 0;
	}
	if(self.var_75dcfb99 >= sword_level)
	{
		return;
	}
	self.var_75dcfb99++;
    while(self GetCurrentWeapon() == level.sword_quest.weapons[self.characterindex][sword_level])
    {
        self waittill("weapon_change_complete", weapon);
    }
	while(!self GetCurrentWeapon() == level.sword_quest.weapons[self.characterindex][sword_level])
	{
		self waittill("weapon_change_complete", weapon);
	}
	if(sword_level == 1)
	{
		zm_equipment::show_hint_text(&"ZM_ZOD_SWORD_1_INSTRUCTIONS", 5);
	}
	else
	{
		zm_equipment::show_hint_text(&"ZM_ZOD_SWORD_2_INSTRUCTIONS", 5);
	}
}

GiveRags(weapon)
{
    if(level.script == "zm_genesis") while(self HasWeapon(level.ballweapon)) wait 0.05;
    self zm_weapons::weapon_give(weapon, 0, 1);
    if(level.script == "zm_castle") self thread zm_equipment::show_hint_text(&"ZM_CASTLE_GRAVITYSPIKE_USE_HINT", 3);
    else self thread zm_equipment::show_hint_text(&"ZM_GENESIS_GRAVITYSPIKE_USE_HINT", 3);
    self GadgetPowerSet(self GadgetGetSlot(weapon), 100);
    self.gravityspikes_state = 2;
}

GiveSkull(weapon)
{
    if(!IsDefined(self.var_b319e777))
    {
        self.var_b319e777 = 1;
        self DisableWeaponCycling();
        self PlaySound("zmb_skullgun_pickup");
        prev_weapon = self GetCurrentWeapon();
        self SkullState(3, undefined, 1);
        self thread zm_audio::create_and_play_dialog("quest_skull", "weapon_success");
        self zm_weapons::weapon_give(weapon, undefined, undefined, 1);
        self GadgetPowerSet(0, 100);
        self SwitchToweapon(weapon);
        self notify(#"hash_ae5d6003");
        level flag::set("a_player_got_skullgun");
        self waittill("weapon_change_complete");
        wait(0.25);
        self SwitchToweapon(prev_weapon);
        self waittill("weapon_change_complete");
        wait(0.25);
        self GadgetPowerSet(0, 100);
        self SetWeaponAmmoClip(weapon, 0);
        self GiveAchievement("ZM_ISLAND_OBTAIN_SKULL");
        self EnableWeaponCycling();
        self flag::set("has_skull");
        self thread SkullUseHint();
    }
    else
    {
        self zm_weapons::weapon_give(weapon, undefined, undefined, 1);
        self GadgetPowerSet(0, 100);
        self SetWeaponAmmoClip(weapon, 0);
        self zm_hero_weapon::set_hero_weapon_state(weapon, 2);
        self thread SkullState(3, undefined, 1);
        self thread zm_hero_weapon::watch_hero_weapon_take();
    }
}

SkullState(skull_state, skull_type, show_ui = 0)
{
	self clientfield::set_to_player("skull_skull_state", skull_state);
	if(IsDefined(skull_type))
	{
		self clientfield::set_to_player("skull_skull_type", skull_type);
	}
	if(show_ui)
	{
		self thread zm_craftables::player_show_craftable_parts_ui(undefined, "zmInventory.widget_skull_parts", 0);
	}
}

SkullUseHint()
{
	self endon("disconnect");
	self.var_1ff2320b = 0;
	while(!self.var_1ff2320b)
	{
		self waittill("weapon_change");
		if(self GetCurrentWeapon() == level.var_c003f5b)
		{
			self.var_1ff2320b = 1;
			self zm_equipment::show_hint_text(&"ZM_ISLAND_SKULL_VAPORIZE", 4);
		}
	}
}

GiveGauntlet()
{
    self zm_weapons::weapon_give(self.weapon_dragon_gauntlet, 0, 1);
	self thread zm_equipment::show_hint_text(&"DLC3_WEAP_DRAGON_GAUNTLET_USE_HINT", 3);
	self.var_8afc8427 = 100;
	self.hero_power = 100;
	self GadgetPowerSet(0, 100);
	self zm_hero_weapon::set_hero_weapon_state(self.weapon_dragon_gauntlet, 2);
	self SetWeaponAmmoClip(self.weapon_dragon_gauntlet, self.weapon_dragon_gauntlet.clipsize);
	self.var_fd007e55 = 1;
    self notify(#"hash_4e21f047");
	level flag::set("dragon_gauntlet_acquired");
}

GiveAnnihilator()
{
    self zm_weapons::weapon_give(level.weaponannihilator);
    self GadgetPowerSet(0, 100);
}

GiveDragonStrike(num)
{
	if(num == 2)
	{
		weapon = getweapon("launcher_dragon_strike_upgraded");
		max_ammo = 2;
	}
	else
	{
		weapon = getweapon("launcher_dragon_strike");
		max_ammo = 1;
	}
	self thread zm_placeable_mine::setup_for_player(weapon, "hudItems.showDpadRight_DragonStrike");
	self thread SetUIModel("zmInventory.widget_dragon_strike");
	self zm_weapons::weapon_give(weapon);
	self setweaponammoclip(weapon, max_ammo);
	self thread zm_equipment::show_hint_text(&"ZM_STALINGRAD_DRAGON_STRIKE_EQUIP");
	self zm_audio::create_and_play_dialog("weapon_pickup", "controller");
}

SetUIModel(str_widget_clientuimodel)
{
	self endon("disconnect");
	self thread clientfield::set_player_uimodel(str_widget_clientuimodel, 1);
	wait(3);
	self thread clientfield::set_player_uimodel(str_widget_clientuimodel, 0);
}

WeaponAvailableValidation()
{
    if(!IsDefined(level.using_vehicle)) self thread VehicleWatcher();
    if(self IsSwitchingWeapons()) return 0;
    if(IsTrue(level.using_vehicle)) return 0;
    if(IsTrue(self.altbody)) return 0;
    if(IsDefined(level.zombie_hero_weapon_list[self GetCurrentWeapon()])) return 0;
    excluded_weapons = array(level.weaponRiotshield, "launcher_dragon_strike", "launcher_dragon_strike_upgraded");
    if(IsInArray(excluded_weapons, self GetCurrentWeapon().rootweapon.name)) return 0;
    if(IsTrue(self.is_drinking)) return 0;
    if(self GetWeaponsListPrimaries().size && IsTrue(self zm_utility::is_player_offhand_weapon(self GetCurrentWeapon()))) return 0;
    if(self GetWeaponsListPrimaries().size && (IsTrue(self GetCurrentWeapon() == level.weapon_none) || IsTrue(self GetCurrentWeapon() == level.weaponbgbuse) || IsTrue(self GetCurrentWeapon() == level.weaponbgbgrab))) return 0;
    if(IsTrue(self HasWeapon(level.ballweapon))) return 0;
    self DisableWeaponCycling();
    self thread util::delay(1, undefined, ::EnableWeaponCycling);
    return 1;
}

VehicleWatcher()
{
    level.using_vehicle = 0;
    for(;;)
    {
        self waittill("enter_vehicle");
        level.using_vehicle = 1;
        self waittill("exit_vehicle");
        level.using_vehicle = 0;
    }
}