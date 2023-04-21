CompleteIceCrypt()
{
    level flag::set("chamber_puzzle_cheat");
    level flag::wait_till_clear("disc_rotation_active");
    while(!level flag::get("ice_puzzle_2_complete")) wait 0.05;
    wait 1;
    discs = GetEntArray("crypt_puzzle_disc", "script_noteworthy");
    foreach(disc in discs)
	{
        if(disc.targetname === "crypt_puzzle_disc_main") continue;
		self thread MoveDisc(disc, 0);
    }
    while(!IsDefined(level.damage_orbs)) wait 0.05;
    while(!IsDefined(level.damage_orbs["ice_orb_exit_path"])) wait 0.05;
    level.damage_orbs["ice_orb_exit_path"] notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "", "", "", "", GetWeapon("staff_water"));
    level flag::clear("chamber_puzzle_cheat");
    level flag::wait_till("staff_water_upgrade_unlocked");
}

CompleteLightningCrypt()
{
    level flag::set("chamber_puzzle_cheat");
    level flag::wait_till_clear("disc_rotation_active");
    while(!level flag::get("electric_puzzle_2_complete")) wait 0.05;
    wait 1;
    discs = GetEntArray("crypt_puzzle_disc", "script_noteworthy");
    foreach(disc in discs)
	{
        if(disc.targetname === "crypt_puzzle_disc_main") continue;
		self thread MoveDisc(disc, 1);
	}
    while(!IsDefined(level.damage_orbs)) wait 0.05;
    while(!IsDefined(level.damage_orbs["lightning_orb_exit_path"])) wait 0.05;
    level.damage_orbs["lightning_orb_exit_path"] notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "", "", "", "", GetWeapon("staff_lightning"));
    level flag::clear("chamber_puzzle_cheat");
    level flag::wait_till("staff_lightning_upgrade_unlocked");
}

CompleteFireCrypt()
{
    level flag::set("chamber_puzzle_cheat");
    level flag::wait_till_clear("disc_rotation_active");
    while(!level flag::get("fire_puzzle_2_complete")) wait 0.05;
    wait 1;
    discs = GetEntArray("crypt_puzzle_disc", "script_noteworthy");
    foreach(disc in discs)
	{
        if(disc.targetname === "crypt_puzzle_disc_main") continue;
		self thread MoveDisc(disc, 2);
	}
    while(!IsDefined(level.damage_orbs)) wait 0.05;
    while(!IsDefined(level.damage_orbs["fire_orb_exit_path"])) wait 0.05;
    level.damage_orbs["fire_orb_exit_path"] notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "", "", "", "", GetWeapon("staff_fire"));
    level flag::clear("chamber_puzzle_cheat");
    level flag::wait_till("staff_fire_upgrade_unlocked");
}

CompleteWindCrypt()
{
    level flag::set("chamber_puzzle_cheat");
    level flag::wait_till_clear("disc_rotation_active");
    while(!level flag::get("air_puzzle_2_complete")) wait 0.05;
    wait 1;
    discs = GetEntArray("crypt_puzzle_disc", "script_noteworthy");
    foreach(disc in discs)
	{
        if(disc.targetname === "crypt_puzzle_disc_main") continue;
		self thread MoveDisc(disc, 3);
	}
    while(!IsDefined(level.damage_orbs)) wait 0.05;
    while(!IsDefined(level.damage_orbs["air_orb_exit_path"])) wait 0.05;
    level.damage_orbs["air_orb_exit_path"] notify("damage", 1, self, (0, 0, 0), (0, 0, 0), "", "", "", "", GetWeapon("staff_air"));
    level flag::clear("chamber_puzzle_cheat");
    level flag::wait_till("staff_air_upgrade_unlocked");
}

MoveDisc(disc, num)
{
    while(disc.position != num)
    {
        lever = GetEnt(disc.target, "targetname");
        self thread BuildAndActivateTrigger(lever.trigger_stub);
        level waittill("crypt_disc_rotation");
        wait 0.25;
    }
}

detour zm_tomb_quest_crypt<scripts\zm\zm_tomb_quest_crypt.gsc>::run_crypt_gem_pos()
{
	str_weapon = undefined;
	complete_flag = undefined;
	str_orb_path = undefined;
	str_glow_fx = undefined;
	n_element = self.script_int;
	switch(self.targetname)
	{
		case "crypt_gem_air":
		{
			w_weapon = level.a_elemental_staffs["staff_air"].w_weapon;
			complete_flag = "staff_air_upgrade_unlocked";
			str_orb_path = "air_orb_exit_path";
			str_final_pos = "air_orb_plinth_final";
			break;
		}
		case "crypt_gem_ice":
		{
			w_weapon = level.a_elemental_staffs["staff_water"].w_weapon;
			complete_flag = "staff_water_upgrade_unlocked";
			str_orb_path = "ice_orb_exit_path";
			str_final_pos = "ice_orb_plinth_final";
			break;
		}
		case "crypt_gem_fire":
		{
			w_weapon = level.a_elemental_staffs["staff_fire"].w_weapon;
			complete_flag = "staff_fire_upgrade_unlocked";
			str_orb_path = "fire_orb_exit_path";
			str_final_pos = "fire_orb_plinth_final";
			break;
		}
		case "crypt_gem_elec":
		{
			w_weapon = level.a_elemental_staffs["staff_lightning"].w_weapon;
			complete_flag = "staff_lightning_upgrade_unlocked";
			str_orb_path = "lightning_orb_exit_path";
			str_final_pos = "lightning_orb_plinth_final";
			break;
		}
	}
	e_gem_model = [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_chamber_to_crypt ]](str_orb_path, self);
	e_main_disc = GetEnt("crypt_puzzle_disc_main", "targetname");
	e_gem_model LinkTo(e_main_disc);
	str_targetname = self.targetname;
	self Delete();
	e_gem_model SetCanDamage(1);
	while(true)
	{
        if(!IsDefined(level.damage_orbs)) level.damage_orbs = [];
        level.damage_orbs[str_orb_path] = e_gem_model;
		e_gem_model waittill("damage", damage, attacker, direction_vec, point, mod, tagname, modelname, partname, weapon);
		if(weapon == w_weapon)
		{
			break;
		}
	}
	e_gem_model clientfield::set("element_glow_fx", n_element);
	e_gem_model PlaySound("zmb_squest_crystal_charge");
	e_gem_model PlayLoopSound("zmb_squest_crystal_charge_loop", 2);
	while(true)
	{
		if([[ @zm_tomb_quest_crypt<scripts\zm\zm_tomb_quest_crypt.gsc>::chamber_disc_gem_has_clearance ]](str_targetname))
		{
			break;
		}
		level waittill("crypt_disc_rotation");
	}
	level flag::set("disc_rotation_active");
	level thread [[ @zm_tomb_amb<scripts\zm\zm_tomb_amb.gsc>::sndplaystinger ]]("side_sting_5");
	[[ @zm_tomb_quest_crypt<scripts\zm\zm_tomb_quest_crypt.gsc>::light_discs_bottom_to_top ]]();
	level thread [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_pillar_Show ]]();
	e_gem_model Unlink();
	s_ascent = struct::get("orb_crypt_ascent_path", "targetname");
	v_next_pos = (e_gem_model.origin[0], e_gem_model.origin[1], s_ascent.origin[2]);
	e_gem_model clientfield::set("element_glow_fx", n_element);
	PlayFXOnTag(level._effect["puzzle_orb_trail"], e_gem_model, "tag_origin");
	e_gem_model PlaySound("zmb_squest_crystal_leave");
	e_gem_model [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_move ]](v_next_pos);
	level flag::clear("disc_rotation_active");
	level thread [[ @zm_tomb_quest_crypt<scripts\zm\zm_tomb_quest_crypt.gsc>::chamber_discs_randomize ]]();
	e_gem_model [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_follow_path ]](s_ascent);
	v_next_pos = (e_gem_model.origin[0], e_gem_model.origin[1], e_gem_model.origin[2] + 2000);
	e_gem_model [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_move ]](v_next_pos);
	s_chamber_path = struct::get(str_orb_path, "targetname");
	str_model = e_gem_model.model;
	e_gem_model Delete();
	e_gem_model = [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_follow_return_path ]](s_chamber_path, n_element);
	s_final = struct::get(str_final_pos, "targetname");
	e_gem_model [[ @zm_tomb_utility<scripts\zm\zm_tomb_utility.gsc>::puzzle_orb_move ]](s_final.origin);
	e_new_gem = Spawn("script_model", s_final.origin);
	e_new_gem SetModel(e_gem_model.model);
	e_new_gem.script_int = n_element;
	e_new_gem clientfield::set("element_glow_fx", n_element);
	e_gem_model Delete();
	e_new_gem PlaySound("zmb_squest_crystal_arrive");
	e_new_gem PlayLoopSound("zmb_squest_crystal_charge_loop", 0.1);
	level flag::set(complete_flag);
}