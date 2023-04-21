BonesPractice()
{
    if(level.script != "zm_genesis")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Rev");
        return;
    }
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    level flag::set("character_stones_done");
    level flag::set("placed_audio1");
    level flag::set("placed_audio2");
    level flag::clear("spawn_zombies");
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("ar_cqb_upgraded"), 1);
    self zm_weapons::weapon_give(GetWeapon("idgun_genesis_0_upgraded"), 1);
    self thread Godmode(1);
    self thread InfiniteAmmo(1);
    self SetOrigin((5635.8, -2286.01, 195.221));
    self SetPlayerAngles((0.928345, -172.408, 0));

    bones = getentArray("b_target", "targetname");
	array::thread_all(bones, ::WatchDamage);

    level flag::wait_till("b_targets_collected");
    IPrintLnBold("All Bones Collected");
}

WatchDamage()
{
    self SetCanDamage(1);
    for(;;)
    {
        self waittill("damage", n_damage, e_attacker, v_dir, v_loc, str_type, str_model, str_tag, str_part, w_weapon, n_flags);
        if(IsDefined(w_weapon) && w_weapon.name != "idgun_genesis_0_upgraded")
        {
            b_upgraded = zm_weapons::is_weapon_upgraded(w_weapon);
            if(b_upgraded)
            {
                IPrintLnBold("Damaged By PAP Gun");
                break;
            }
            else IPrintLnBold("Damaged By Normal Gun");
        }
    }
    for(;;)
    {
        level waittill(#"hash_2751215d", v_position, w_weapon, e_shooter);
		if(w_weapon != level.var_ed2646a1)
		{
            IPrintLnBold("Hit By Wrong Weapon");
			continue;
		}
		n_dist_sq = distancesquared(v_position, self.origin);
		if(n_dist_sq <= 10000)
		{
			IPrintLnBold("Hit By Apothicon");
            break;
		}
    }
}