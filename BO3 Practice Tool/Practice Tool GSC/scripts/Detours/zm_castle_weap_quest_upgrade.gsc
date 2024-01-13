// stop infinite loop if skipping steps
detour zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_1c758ab0()
{
	level.var_f8d1dc16 endon("death");
	level.var_f8d1dc16 endon("quest_swap");
	s_beacon = struct::get(self.target);
	for (;;) {
		level.var_f8d1dc16 waittill("projectile_impact", weapon, point, radius, projectile, normal);
        //if (CheckQuestProgress("storm") >= 4) return;
		if (IsDefined(projectile.var_e4594d27) && projectile.var_e4594d27) {
			if ([[ @zm_castle_weap_quest_upgrade<scripts\zm\zm_castle_weap_quest_upgrade.gsc>::function_51a90202 ]](weapon, 1, point, self)) {
				s_beacon.var_41f52afd clientfield::set("beacon_fx", 2);
				self.b_charged = 1;
				if (IsDefined(projectile.var_8f88d1fd)) {
					projectile.var_8f88d1fd.b_used = 1;
					var_d186cfae = struct::get(projectile.var_8f88d1fd.target, "targetname");
					if (IsDefined(var_d186cfae.var_41f52afd)) {
						var_d186cfae.var_41f52afd clientfield::set("battery_fx", 0);
					}
					wait(0.05);
					if (IsDefined(var_d186cfae.var_41f52afd)) {
						var_d186cfae.var_41f52afd Delete();
					}
				}
				self notify("beacon_charged");
				return;
			}
		}
	}
}