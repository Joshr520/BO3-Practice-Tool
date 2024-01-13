autoexec InitWeaponPresets()
{
	line = compiler::getline("Practice Tool/Weapon Preset.txt", true);
	while (line != "n/a" && line != "end") {
		tokens = StrTok(line, ",");
		weapon_name = array::pop_front(tokens, 0);
		weapon = GetWeapon(weapon_name);
		while (!IsDefined(level.zombie_weapons[weapon])) {
			wait 0.05;
		}
		weapon.force_camo = Int(array::pop(tokens, tokens.size));
		level.zombie_weapons[weapon].force_attachments = tokens;
		line = compiler::getline("Practice Tool/Weapon Preset.txt", false);
	}
}

detour zm_weapons<scripts\zm\_zm_weapons.gsc>::give_build_kit_weapon(weapon)
{
	upgraded = 0;
	camo = undefined;
    if (IsDefined(weapon.force_camo) && weapon.force_camo >= 0) {
		compiler::nprintln(weapon.force_camo);
		camo = weapon.force_camo;
	}
	base_weapon = weapon;
	if (zm_weapons::is_weapon_upgraded(weapon)) {
		if (isdefined(weapon.pap_camo_to_use)) {
			camo = weapon.pap_camo_to_use;
		}
		else {
			camo = zm_weapons::get_pack_a_punch_camo_index(undefined);
		}
		upgraded = 1;
		base_weapon = zm_weapons::get_base_weapon(weapon);
	}
	if (zm_weapons::is_weapon_included(base_weapon)) {
		force_attachments = zm_weapons::get_force_attachments(base_weapon.rootweapon);
	}
	if (isdefined(force_attachments) && force_attachments.size) {
		if (upgraded) {
			packed_attachments = [];
			packed_attachments[packed_attachments.size] = "extclip";
			packed_attachments[packed_attachments.size] = "fmj";
			force_attachments = ArrayCombine(force_attachments, packed_attachments, 0, 0);
		}
		weapon = GetWeapon(weapon.rootweapon.name, force_attachments);
		if (!isdefined(camo)) {
			camo = 0;
		}
		weapon_options = self CalcWeaponOptions(camo, 0, 0);
		acvi = 0;
	}
	else {
		weapon = self GetBuildKitWeapon(weapon, upgraded);
		weapon_options = self GetBuildKitWeaponOptions(weapon, camo);
		acvi = self GetBuildKitAttachmentCosmeticVariantIndexes(weapon, upgraded);
	}
	self GiveWeapon(weapon, weapon_options, acvi);
	return weapon;
}