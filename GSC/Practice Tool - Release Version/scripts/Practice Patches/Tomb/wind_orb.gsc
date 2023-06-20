WindOrbPracticeShots()
{
    WaitFadeIn();
    if(level.script != "zm_tomb")
    {
		level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Tomb");
        return;
    }

	level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Wind Orb Practice Starting");

    self thread DebugTesting();
    self thread WaypointSetup(1);
    level flag::clear("spawn_zombies");

    self zm_weapons::weapon_give(GetWeapon("staff_air"));

    wind_orbs = struct::get_array("puzzle_smoke_origin", "targetname");
	foreach(wind_orb in wind_orbs)
	{
		wind_orb thread WindOrbSmoke();
	}
}

WindOrbSmoke()
{
    self.e_fx = Spawn("script_model", self.origin);
	self.e_fx.angles = self.angles;
	self.e_fx SetModel("tag_origin");
	self.e_fx PlayLoopSound("zmb_squest_wind_incense_loop", 2);
	PlayFXonTag(level._effect["air_puzzle_smoke"], self.e_fx, "tag_origin");
	self thread WindOrbSmokeDirection();
}

WindOrbSmokeDirection()
{
    smoke_dest = struct::get("puzzle_smoke_dest", "targetname");
	v_to_dest = VectorNormalize(smoke_dest.origin - self.origin);
	f_min_dot = Cos(self.script_int);
	self.detector_brush SetCanDamage(1);
	while(true)
	{
		self.detector_brush waittill("damage", damage, attacker, direction_vec, point, mod, tagname, modelname, partname, weaponname);
		if(weaponname.name == "staff_air")
		{
			level notify("vo_try_puzzle_air2", attacker);
			f_dot = VectorDot(v_to_dest, direction_vec);
			if(f_dot > f_min_dot) IPrintLnBold("Success");
            else IPrintLnBold("Required Value: " + f_min_dot + " - Achieved Value: " + f_dot);
		}
	}
}