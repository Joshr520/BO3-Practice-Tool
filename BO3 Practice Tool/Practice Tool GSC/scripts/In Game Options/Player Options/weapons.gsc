autoexec InitWeapons()
{
    foreach (player in GetPlayers()) {
        player thread VehicleWatcher();
    }
}

GivePlayerWeapon(weapon_name, upgraded = false)
{
    weapon = GetWeapon(weapon_name);

    if (!IsDefined(weapon) || weapon == level.weaponnone) {
        switch (weapon_name) {
            case "margwa_head_zod": {
                self GiveZodMargwaHead();
                return;
            }
            case "claw_helmet": {
                self GiveClawHelmet();
                return;
            }
            case "dragon_wings":
            case "raz_hat":
            case "sentinel_hat": {
                self GiveStalingradWearable(weapon_name);
                return;
            }
            case "s_weasels_hat":
            case "s_helm_of_siegfried":
            case "s_helm_of_the_king":
            case "s_dire_wolf_head":
            case "s_keeper_skull_head":
            case "s_margwa_head":
            case "s_apothicon_mask":
            case "s_fury_head": {
                self GiveGenesisWearable(weapon_name);
                return;
            }
            case "golden_shovel":
            case "golden_helmet": {
                self GiveTombWearable(weapon_name);
                return;
            }
            case "shield": {
                weapon = (upgraded && IsDefined(level.weaponriotshieldupgraded)) ? level.weaponriotshieldupgraded : level.weaponriotshield;
                if (!IsDefined(weapon) || weapon == level.weaponnone) {
                    thread WriteToScreen("Shield doesn't exist on this map");
                    return;
                }
                if (self HasWeapon(weapon)) {
                    health = self clientfield::get_player_uimodel("zmInventory.shield_health");
                    ammo = self GetWeaponAmmoClip(weapon);
                    if (health == 1 && ammo == weapon.maxammo) {
                        thread WriteToScreen("Shield Already New");
                        return;
                    }
                    thread WriteToScreen("Renewing Shield");
                    if (health < 1) {
                        self [[ @riotshield<scripts\zm\_zm_weap_riotshield.gsc>::player_set_shield_health ]](1, 1);
                    }
                    if (ammo < weapon.maxammo) {
                        self SetWeaponAmmoClip(weapon, weapon.maxammo);
                        self notify("equipment_ammo_changed", weapon);
                    }
                    return;
                }
                thread WriteToScreen("Giving Shield " + MakeLocalizedString(weapon.displayName));
                self zm_weapons::weapon_give(weapon);
                return;
            }
            case "idgun_": {
                if (level.mapname == "zm_zod") {
                    weapon = upgraded ? GetWeapon("idgun_upgraded_" + self.characterindex) : GetWeapon("idgun_" + self.characterindex);
                }
                else {
                    weapon = upgraded ? GetWeapon("idgun_genesis_0_upgraded") : GetWeapon("idgun_genesis_0");
                }
                skip_upgrade = true;
                break;
            }
            case "keeper_sword": {
                sword_level = upgraded ? 2 : 1;
                self WaitForWeaponAvailable("give");
                thread WriteToScreen("Giving Weapon: " + MakeLocalizedString(level.sword_quest.weapons[self.characterindex][sword_level].displayName));
                self [[ @zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::give_sword ]](sword_level, 1);
                return;
            }
            default: {
                thread WriteToScreen("Gun Not Found");
                return;
            }
        }
    }

    if (upgraded && !zm_weapons::is_weapon_upgraded(weapon) && !IsTrue(skip_upgrade)) {
        upgrade_weapon = zm_weapons::get_upgrade_weapon(weapon);
        if (!IsDefined(upgrade_weapon)) {
            upgrade_weapon = GetWeapon(weapon_name + "_upgraded");
        }
        if (!IsDefined(upgrade_weapon) || upgrade_weapon == level.weaponnone) {
            thread WriteToScreen("No Weapon Upgrade Available");
            return;
        }
        weapon = upgrade_weapon;
    }

    if (zm_utility::is_lethal_grenade(weapon)) {
        weapon = self zm_utility::get_player_lethal_grenade();
        if (self GetFractionMaxAmmo(weapon) == 1) {
            thread WriteToScreen("Already Have Max Lethal Grenades");
            return;
        }
        thread WriteToScreen("Giving Max Lethal Grenades");
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    else if (zm_utility::is_tactical_grenade(weapon)) {
        if (!self HasWeapon(weapon)) {
            thread WriteToScreen("Giving Weapon " + MakeLocalizedString(weapon.displayName));
            self zm_weapons::weapon_give(weapon);
            return;
        }
        if (self GetFractionMaxAmmo(weapon) == 1) {
            thread WriteToScreen("Already Have Max Tactical Grenades");
            return;
        }
        thread WriteToScreen("Giving Max Tactical Grenades");
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    else if (IsDefined(level.placeable_mines[weapon_name])) {
        displayName = (weapon_name == "launcher_dragon_strike" ? (upgraded ? "Dragon Strike Upgraded" : "Dragon Strike") : MakeLocalizedString(weapon.displayName));
        if (!self HasWeapon(weapon)) {
            thread WriteToScreen("Giving Weapon " + displayName);
            self zm_weapons::weapon_give(weapon);
            return;
        }
        if (self GetWeaponAmmoClip(weapon) == weapon.clipsize) {
            thread WriteToScreen("Already Have Max Mines");
            return;
        }
        thread WriteToScreen("Giving Max Mines");
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    else if (zm_utility::is_hero_weapon(weapon)) {
        if (!self HasWeapon(weapon)) {
            switch (weapon_name) {
                case "hero_annihilator": {
                    self WaitForWeaponAvailable("give");
                    self zm_weapons::weapon_give(weapon);
                    self GadgetPowerSet(0, 100);
                    break;
                }
                case "hero_gravityspikes_melee": {
                    hint = level.mapname == "zm_castle" ? &"ZM_CASTLE_GRAVITYSPIKE_USE_HINT" : &"ZM_GENESIS_GRAVITYSPIKE_USE_HINT";
                    self WaitForWeaponAvailable("give");
                    self zm_weapons::weapon_give(weapon, 0, 1);
                    self thread zm_equipment::show_hint_text(hint, 3);
                    self GadgetPowerSet(self GadgetGetSlot(weapon), 100);
                    self [[ @zm_weap_gravityspikes<scripts\zm\_zm_weap_gravityspikes.gsc>::update_gravityspikes_state ]](2);
                    break;
                }
                case "skull_gun": {
                    self WaitForWeaponAvailable("give");
                    [[ @zm_island_skullquest<scripts\zm\zm_island_skullweapon_quest.gsc>::function_458f50f2 ]]();
                    break;
                }
                case "dragon_gauntlet": {
                    self WaitForWeaponAvailable("give");
                    self [[ @zm_weap_dragon_gauntlet<scripts\zm\_zm_weap_dragon_gauntlet.gsc>::function_99a68dd ]]();
                    self notify(#"hash_4e21f047");
	                level flag::set("dragon_gauntlet_acquired");
                    break;
                }
            }
            thread WriteToScreen("Giving Weapon " + MakeLocalizedString(weapon.displayName));
            return;
        }
        if (self GadgetPowerGet(0) == 100) {
            thread WriteToScreen("Already Have Max Hero Power");
            return;
        }
        thread WriteToScreen("Giving Max Hero Power");
        self GadgetPowerSet(0, 100);
        self SetWeaponAmmoClip(weapon, weapon.clipsize);
        return;
    }
    else if (zm_utility::is_melee_weapon(weapon)) {
        displayName = weapon_name == "knife_plunger" ? "Plunger" : MakeLocalizedString(weapon.displayName);
        if (!self HasWeapon(weapon)) {
            self WaitForWeaponAvailable("give");
            thread WriteToScreen("Giving Weapon " + MakeLocalizedString(displayName));
            self zm_weapons::weapon_give(weapon);
            return;
        }
        else {
            thread WriteToScreen("Already Have Weapon " + MakeLocalizedString(displayName));
            return;
        }
    }
    else if (zm_equipment::is_equipment(weapon))
    {
        if (self zm_equipment::has_player_equipment(weapon)) {
            thread WriteToScreen("Already Have Equipment " + MakeLocalizedString(weapon.displayName));
            return;
        }
        thread WriteToScreen("Giving Equipment " + MakeLocalizedString(weapon.displayName));
        self zm_equipment::give(weapon);
        return;
    }
    else {
        if (self zm_weapons::has_weapon_or_attachments(weapon)) {
            weapon = zm_weapons::get_weapon_with_attachments(weapon);
            ammo_clip = self GetWeaponAmmoClip(weapon);
            ammo_stock = self GetWeaponAmmoStock(weapon);
            if (IsDefined(weapon.dualwieldweapon)) {
                ammo_stock += self GetWeaponAmmoClip(weapon.dualwieldweapon);
            }
            max_ammo = weapon.maxammo + weapon.clipsize;
            if ((ammo_clip + ammo_stock) >= max_ammo) {
                thread WriteToScreen("Already Max Ammo For " + MakeLocalizedString(weapon.displayName));
                return;
            }
            thread WriteToScreen("Giving Max Ammo For " + MakeLocalizedString(weapon.displayName));
            self zm_weapons::ammo_give(weapon);
            self SetWeaponAmmoClip(weapon, weapon.clipsize);
            return;
        }
        if (self zm_weapons::has_weapon_or_attachments(zm_weapons::get_upgrade_weapon(weapon))) {
            self WaitForWeaponAvailable("take");
            self zm_weapons::weapon_take(zm_weapons::get_upgrade_weapon(weapon));
        }
        self WaitForWeaponAvailable("give");
        thread WriteToScreen("Giving Weapon " + MakeLocalizedString(weapon.displayName));
        self zm_weapons::weapon_give(weapon);
        if (IsSubStr(weapon.name, "staff_") && upgraded) {
            [[ @zm_tomb_craftables<scripts\zm\zm_tomb_craftables.gsc>::set_player_staff ]](weapon.name, self);
        }
    }
}

TakePlayerWeapon(slot)
{
    switch (slot) {
        case "Primary Weapon": {
            primaries = self GetWeaponsListPrimaries();
            if (!primaries.size) {
                thread WriteToScreen("No primary weapon to take");
                return;
            }
            self WaitForWeaponAvailable("take");
            weapon = self GetCurrentWeapon();
            thread WriteToScreen("Taking Weapon " + MakeLocalizedString(weapon.displayName));
            self zm_weapons::weapon_take(weapon);
            break;
        }
        case "Hero Weapon": {
            weapon = self zm_utility::get_player_hero_weapon();
            if (weapon == level.weaponnone) {
                thread WriteToScreen("No Hero Slot To Take");
                return;
            }
            thread WriteToScreen("Taking Hero Slot");
            self zm_weapons::weapon_take(weapon);
            self.current_hero_weapon = level.weaponnone;
            break;
        }
        case "Lethals": {
            weapon = self zm_utility::get_player_lethal_grenade();
            if (!self GetWeaponAmmoClip(weapon)) {
                thread WriteToScreen("No Lethal Grenades To Take");
                return;
            }
            thread WriteToScreen("Taking Lethal Grenades");
            self SetWeaponAmmoClip(weapon, 0);
            break;
        }
        case "Tacticals": {
            weapon = self zm_utility::get_player_tactical_grenade();
            if (!self GetWeaponAmmoClip(weapon)) {
                thread WriteToScreen("No Tactical Grenades To Take");
                return;
            }
            thread WriteToScreen("Taking Tactical Grenades");
            self SetWeaponAmmoClip(weapon, 0);
            break;
        }
        case "Shield Slot": {
            shield = self.weaponriotshield;
            gasmask = GetWeapon("equip_gasmask");
            hacker = GetWeapon("equip_hacker");
            if (!self HasWeapon(shield) && !self HasWeapon(gasmask) && !self HasWeapon(hacker)) {
                thread WriteToScreen("No Shield Slot To Take");
                return;
            }
            thread WriteToScreen("Taking Shield Slot");
            if (self HasWeapon(shield)) {
                [[ self.player_shield_apply_damage ]](self.weaponriotshield.weaponstarthitpoints, 0);
            }
            else if (self HasWeapon(GetWeapon("equip_gasmask"))) {
                self zm_equipment::take(gasmask);
                //self notify(#"hash_5a02c845");
            }
            else if (self HasWeapon(GetWeapon("equip_hacker"))) {
                self zm_equipment::take(hacker);
            }
            break;
        }
        case "Mine": {
            weapon = self zm_utility::get_player_placeable_mine();
            if (weapon == level.weaponnone) {
                thread WriteToScreen("No Mine Slot To Take");
                return;
            }
            thread WriteToScreen("Taking Mine Slot");
            self zm_weapons::weapon_take(weapon);
            self.current_placeable_mine = level.weaponnone;
            break;
        }
        case "Wearable": {
            switch (level.mapname) {
                case "zm_zod": {
                    self RemoveZodMargwaHead();
                    break;
                }
                case "zm_castle": {
                    self RemoveClawHelmet();
                    break;
                }
                case "zm_stalingrad": {
                    self RemoveStalingradWearable();
                    break;
                }
                case "zm_genesis": {
                    self RemoveGenesisWearable();
                    break;
                }
                case "zm_tomb": {
                    self RemoveTombWearable();
                    break;
                }
                default: {
                    return;
                }
            }
        }
        default: {
            break;
        }
    }
}

WaitForWeaponAvailable(state)
{
    if (!IsDefined(self.using_vehicle)) {
        self thread VehicleWatcher();
    }

    excluded_weapons = array(level.weaponRiotshield, "launcher_dragon_strike", "launcher_dragon_strike_upgraded");
    for (;;) {
        current_weapon = self GetCurrentWeapon();
        primaries_size = IsTrue(self GetWeaponsListPrimaries().size);

        switching = self IsSwitchingWeapons();
        vehicle = IsTrue(self.using_vehicle);
        altbody = IsTrue(self.altbody);
        drinking = IsTrue(self.is_drinking);
        teleporting = IsTrue(self.teleporting) || IsTrue(self.b_teleporting);
        ballweapon = IsDefined(level.ballweapon) && IsTrue(self HasWeapon(level.ballweapon));
        offhand = (state == "take" ? primaries_size : false) && IsTrue(self zm_utility::is_player_offhand_weapon(current_weapon));
        weaponnone = (state == "take" ? primaries_size : false) && current_weapon == level.weaponnone;
        weaponbgb = state != "bgb" && (state == "take" ? primaries_size : false) && (current_weapon == level.weaponbgbuse || current_weapon == level.weaponbgbgrab);
        excluded_weapon = state == "give" && IsTrue(zm_utility::is_hero_weapon(current_weapon)) || IsInArray(excluded_weapons, current_weapon.rootweapon.name);

        weapon_available = !switching && !vehicle && !altbody && !drinking && !teleporting && !ballweapon && !offhand && !weaponnone && !weaponbgb && !excluded_weapon;

        if (weapon_available) {
            break;
        }

        wait 0.05;
    }

    self DisableWeaponCycling();
    self thread ReEnableWeaponCycling();
}

ReEnableWeaponCycling()
{
    self util::waittill_any("weapon_give", "weapon_take");
    self EnableWeaponCycling();
}

VehicleWatcher()
{
    self.using_vehicle = 0;
    for (;;) {
        self waittill("enter_vehicle");
        self.using_vehicle = 1;
        self waittill("exit_vehicle");
        self.using_vehicle = 0;
    }
}