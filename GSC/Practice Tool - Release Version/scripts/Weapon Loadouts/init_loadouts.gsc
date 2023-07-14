detour zm_weapons<scripts\zm\_zm_weapons.gsc>::load_weapon_spec_from_table(table, first_row)
{
    optic_convert = [];
    optic_convert["smg"] = Array("reflex", "dualoptic", "reddot", "holo", "acog");
    optic_convert["rifle"] = Array("reflex", "ir", "dualoptic", "reddot", "holo", "acog");
    optic_convert["shotgun"] = Array("reflex", "reddot", "holo");
    optic_convert["lmg"] = Array("reflex", "ir", "dualoptic", "reddot", "holo", "acog");
    optic_convert["sniper"] = Array("reddot", "ir", "dualoptic", "acog");
    optic_convert["pistol"] = Array("reflex", "reddot");

    attachment_convert = [];
    attachment_convert["smg"] = Array("quickdraw", "stalker", "steadyaim", "rf", "grip", "extbarrel", "fastreload");
    attachment_convert["rifle"] = Array("quickdraw", "stalker", "steadyaim", "rf", "grip", "extbarrel", "fastreload");
    attachment_convert["shotgun"] = Array("quickdraw", "stalker", "steadyaim", "rf", "extbarrel", "fastreload");
    attachment_convert["lmg"] = Array("quickdraw", "stalker", "steadyaim", "rf", "grip", "fastreload");
    attachment_convert["sniper"] = Array("swayreduc", "stalker", "rf", "fastreload");
    attachment_convert["pistol"] = Array("quickdraw", "steadyaim", "extbarrel", "fastreload");

	gametype = getdvarstring("ui_gametype");
	index = 1;
	row = tablelookuprow(table, index);
    custom_loadout = compiler::loadweaponjson();
    level.camo_lookup = [];
	while(IsDefined(row))
	{
		weapon_name = zm_weapons::checkstringvalid(row[0]);
		upgrade_name = zm_weapons::checkstringvalid(row[1]);
		hint = zm_weapons::checkstringvalid(row[2]);
		cost = int(row[3]);
		weaponvo = zm_weapons::checkstringvalid(row[4]);
		weaponvoresp = zm_weapons::checkstringvalid(row[5]);
		ammo_cost = undefined;
		if("" != row[6])
		{
			ammo_cost = int(row[6]);
		}
		create_vox = zm_weapons::checkstringvalid(row[7]);
		is_zcleansed = tolower(row[8]) == "true";
		in_box = tolower(row[9]) == "true";
		upgrade_in_box = tolower(row[10]) == "true";
		is_limited = tolower(row[11]) == "true";
		is_aat_exempt = tolower(row[17]) == "true";
		limit = int(row[12]);
		upgrade_limit = int(row[13]);
		content_restrict = row[14];
		wallbuy_autospawn = tolower(row[15]) == "true";
		weapon_class = zm_weapons::checkstringvalid(row[16]);
		is_wonder_weapon = tolower(row[18]) == "true";
		force_attachments = tolower(row[19]);
        if(custom_loadout)
        {
            local_name = MakeLocalizedString(GetWeapon(weapon_name).displayName);
            optic = compiler::getcustomoptic(weapon_class, local_name);
            if(optic >= -1)
            {
                attachments = [];
                custom_index = 0;
                attachment = compiler::getcustomattachment(weapon_class, local_name, custom_index);
                if(attachment >= 0)
                {
                    force_attachments = "";
                    while(attachment >= 0)
                    {
                        attachments[attachments.size] = attachment_convert[weapon_class][attachment];
                        custom_index++;
                        attachment = compiler::getcustomattachment(weapon_class, local_name, custom_index);
                    }
                }
                if(optic >= 0)
                {
                    ArrayInsert(attachments, optic_convert[weapon_class][optic], 0);
                }
                foreach(num, attachment in attachments)
                {
                    force_attachments += attachment;
                    if(num != attachments.size)
                    {
                        force_attachments += " ";
                    }
                }
                camo = compiler::getcustomcamo(weapon_class, local_name);
                if(camo > 0)
                {
                    level.camo_lookup[GetWeapon(weapon_name).name] = camo;
                }
            }
        }
		zm_utility::include_weapon(weapon_name, in_box);
		if(IsDefined(upgrade_name))
		{
			zm_utility::include_weapon(upgrade_name, upgrade_in_box);
		}
		zm_weapons::add_zombie_weapon(weapon_name, upgrade_name, hint, cost, weaponvo, weaponvoresp, ammo_cost, create_vox, is_wonder_weapon, force_attachments);
		if(is_limited)
		{
			if(IsDefined(limit))
			{
				zm_weapons::add_limited_weapon(weapon_name, limit);
			}
			if(IsDefined(upgrade_limit) && IsDefined(upgrade_name))
			{
				zm_weapons::add_limited_weapon(upgrade_name, upgrade_limit);
			}
		}
		if(is_aat_exempt && IsDefined(upgrade_name))
		{
			aat::register_aat_exemption(getweapon(upgrade_name));
		}
		index++;
		row = tablelookuprow(table, index);
	}
}

detour zm_weapons<scripts\zm\_zm_weapons.gsc>::give_build_kit_weapon(weapon)
{
	upgraded = 0;
	camo = undefined;
    if(IsDefined(level.camo_lookup[weapon.name])) camo = level.camo_lookup[weapon.name];
	base_weapon = weapon;
	if(zm_weapons::is_weapon_upgraded(weapon))
	{
		if(isdefined(weapon.pap_camo_to_use))
		{
			camo = weapon.pap_camo_to_use;
		}
		else
		{
			camo = zm_weapons::get_pack_a_punch_camo_index(undefined);
		}
		upgraded = 1;
		base_weapon = zm_weapons::get_base_weapon(weapon);
	}
	if(zm_weapons::is_weapon_included(base_weapon))
	{
		force_attachments = zm_weapons::get_force_attachments(base_weapon.rootweapon);
	}
	if(isdefined(force_attachments) && force_attachments.size)
	{
		if(upgraded)
		{
			packed_attachments = [];
			packed_attachments[packed_attachments.size] = "extclip";
			packed_attachments[packed_attachments.size] = "fmj";
			force_attachments = arraycombine(force_attachments, packed_attachments, 0, 0);
		}
		weapon = getweapon(weapon.rootweapon.name, force_attachments);
		if(!isdefined(camo))
		{
			camo = 0;
		}
		weapon_options = self calcweaponoptions(camo, 0, 0);
		acvi = 0;
	}
	else
	{
		weapon = self getbuildkitweapon(weapon, upgraded);
		weapon_options = self getbuildkitweaponoptions(weapon, camo);
		acvi = self getbuildkitattachmentcosmeticvariantindexes(weapon, upgraded);
	}
	self giveweapon(weapon, weapon_options, acvi);
	return weapon;
}