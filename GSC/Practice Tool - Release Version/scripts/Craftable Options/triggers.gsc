BuildAndActivateTrigger(stub, craft = 0)
{
    trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(stub, self);
	trig.delete_trigger = 0;
	if(IsDefined(trig.radius)) trig.radius = 1500;
    if(IsDefined(trig.script_height)) trig.script_height = 1500;
    if(IsDefined(trig.script_height)) trig.script_height = 1500;
    if(IsDefined(trig.script_length)) trig.script_length = 1500;
    if(IsDefined(trig.require_look_at)) trig.require_look_at = 0;
    if(IsDefined(trig.require_look_toward)) trig.require_look_toward = 0;
    trig zm_unitrigger::assess_and_apply_visibility(trig, trig.stub, self, 1);
	trig.parent_player = self;
	org = trig.origin;
	trig.origin = self.origin;
	wait 0.1;
	trig notify("trigger", self);
	if(craft)
	{
		self.usebar = 1;
		wait 0.2;
		trig notify("craft_succeed");
		self.usebar = undefined;
	}
	wait 0.1;
	if(IsDefined(trig))
	{
		trig.origin = org;
		zm_unitrigger::unregister_unitrigger(trig);
		//if(IsDefined(trig)) trig Delete();
	}
}

FindAndactivateEnt(origin)
{
	wait_var = 0;
    found = 0;
	skip = 0;
    while(!found)
    {
        ents = GetEntArray();
        foreach(ent in ents)
        {
			if(!IsDefined(ent) || !IsDefined(ent.origin)) continue;
            if(IsArray(origin))
			{
				foreach(org in origin)
				{
					if(DistanceSquared(org, ent.origin) <= 5)
					{
						ent notify("trigger_activated", self);
						found = 1;
						skip = 1;
						break;
					}
				}
			}
			else if(DistanceSquared(origin, ent.origin) <= 5)
			{
				ent notify("trigger_activated", self);
				found = 1;
				break;
			}
			if(skip) break;
            wait_var++;
            if(wait_var >= 100)
            {
                wait 0.05;
                wait_var = 0;
            }
        }
    }
}

detour zm_craftables<scripts\zm\craftables\_zm_craftables.gsc>::craftable_use_hold_think_internal(player, slot = self.stub.craftablespawn.inventory_slot)
{
	wait(0.01);
	if(!IsDefined(self))
	{
		if(IsDefined(player.craftableaudio))
		{
			player.craftableaudio delete();
			player.craftableaudio = undefined;
		}
		return;
	}
	if(self.stub.craftablespawn zm_craftables::craftable_can_use_shared_piece())
	{
		slot = undefined;
	}
	if(!IsDefined(self.usetime))
	{
		self.usetime = int(3000);
	}
	self.craft_time = self.usetime;
	self.craft_start_time = gettime();
	craft_time = self.craft_time;
	craft_start_time = self.craft_start_time;
	if(craft_time > 0)
	{
		player zm_utility::disable_player_move_states(1);
		player zm_utility::increment_is_drinking();
		orgweapon = player getcurrentweapon();
		build_weapon = getweapon("zombie_builder");
		player giveweapon(build_weapon);
		player switchtoweapon(build_weapon);
		if(IsDefined(slot))
		{
			self.stub.craftablespawn zm_craftables::craftable_set_piece_crafting(player.current_craftable_pieces[slot]);
		}
		else
		{
			player zm_craftables::start_crafting_shared_piece();
		}
		player thread zm_craftables::player_progress_bar(craft_start_time, craft_time);
		if(IsDefined(level.craftable_craft_custom_func))
		{
			player thread [[level.craftable_craft_custom_func]](self.stub);
		}
		while(IsDefined(self) && player zm_craftables::player_continue_crafting(self.stub.craftablespawn, slot) && (gettime() - self.craft_start_time) < self.craft_time)
		{
			wait(0.05);
		}
		player notify("craftable_progress_end");
		player zm_weapons::switch_back_primary_weapon(orgweapon);
		player takeweapon(build_weapon);
		if(IsDefined(player.is_drinking) && player.is_drinking)
		{
			player zm_utility::decrement_is_drinking();
		}
		player zm_utility::enable_player_move_states();
	}
	if(IsDefined(self) && (self.craft_time <= 0 || (gettime() - self.craft_start_time) >= self.craft_time))
	{
		if(IsDefined(slot))
		{
			self.stub.craftablespawn zm_craftables::craftable_clear_piece_crafting(player.current_craftable_pieces[slot]);
		}
		else
		{
			player zm_craftables::finish_crafting_shared_piece();
		}
		self notify("craft_succeed");
	}
	else
	{
		if(IsDefined(player.craftableaudio))
		{
			player.craftableaudio delete();
			player.craftableaudio = undefined;
		}
		if(IsDefined(slot))
		{
			self.stub.craftablespawn  zm_craftables::craftable_clear_piece_crafting(player.current_craftable_pieces[slot]);
		}
		else
		{
			player  zm_craftables::finish_crafting_shared_piece();
		}
		self notify("craft_failed");
	}
}