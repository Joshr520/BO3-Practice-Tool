// detour so bow script error doesn't occur
detour zm_weap_elemental_bow<scripts\zm\_zm_weap_elemental_bow.gsc>::function_7bc6b9d(var_6ab83514, var_8f9bdf29, var_332bb697 = undefined)
{
	self endon("death");
	while(true)
	{
		self waittill("projectile_impact", weapon, v_position, radius, e_projectile, normal);
		var_48369d98 = [[ @zm_weap_elemental_bow<scripts\zm\_zm_weap_elemental_bow.gsc>::function_1796e73 ]](weapon.name);
		if(var_48369d98 == var_6ab83514 || var_48369d98 == var_8f9bdf29)
		{
			if(var_48369d98 != "elemental_bow" && var_48369d98 != "elemental_bow_wolf_howl4" && IsDefined(e_projectile.birthtime))
			{
				if((GetTime() - e_projectile.birthtime) <= 150)
				{
					RadiusDamage(v_position, 32, level.zombie_health, level.zombie_health, self, "MOD_UNKNOWN", weapon);
				}
			}
			self thread [[ @zm_weap_elemental_bow<scripts\zm\_zm_weap_elemental_bow.gsc>::function_d2e32ed2 ]](var_48369d98, v_position);
			if(IsDefined(var_332bb697))
			{
				if(IsDefined(e_projectile)) self thread [[var_332bb697]](weapon, v_position, radius, e_projectile, normal);
			}
			self thread [[ @zm_weap_elemental_bow<scripts\zm\_zm_weap_elemental_bow.gsc>::function_9c5946ba ]](weapon, v_position);
		}
	}
}