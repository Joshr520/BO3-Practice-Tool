autoexec InitBlockerArrays()
{

	system::wait_till("all");

	level flag::wait_till("start_zombie_round_logic");

	level.blockers = [];

	types = Array("zombie_door", "zombie_airlock_buy", "zombie_debris");
    foreach (type in types) {
        level.blockers = ArrayCombine(level.blockers, GetEntArray(type, "targetname"), 0, 0);
	}
}

OpenAllDoors(map_objects = true)
{	
	thread WriteToScreen("Opening All Doors");
	SetDvar("zombie_unlock_all", 1);
    types = Array("zombie_door", "zombie_airlock_buy", "zombie_debris");
    foreach (type in types) {
        doors = GetEntArray(type, "targetname");
        foreach (door in doors) {
			switch (type) {
				case "zombie_airlock_buy": {
					door thread [[ @zm_moon_utility<scripts\zm\zm_moon_utility.gsc>::moon_door_opened ]]();
					break;
				}
				default: {
					if (IsDefined(door) && !IsTrue(door.has_been_opened)) {
						door notify("trigger", self, 1);
					}
					wait 0.1;
					break;
				}
			}
        }
    }
	SetDvar("zombie_unlock_all", 0);

	foreach (zone in GetArrayKeys(level.zones)) {
		zm_zonemgr::enable_zone(zone);
	}

	if (map_objects) {
		self OpenAllMapObjects();
	}
}

OpenAllMapObjects()
{
	switch (level.mapname) {
		case "zm_zod": {
			self thread OpenBeastDoors();
			break;
		}
		case "zm_castle": {
			self thread EnablePads();
			break;
		}
		case "zm_island": {
			gate = GetEnt("pap_gate", "targetname");
			gate MoveTo(gate.s_pos.origin, 3);
			level flag::set("power_on1");
			level flag::set("power_on2");
			level zm_power::turn_power_on_and_open_doors(3);
			level notify("override_bunker_door_string");
			break;
		}
		case "zm_genesis": {
			self thread TrapApothicon();
		}
		case "zm_tomb": {
			self thread EnableTeleporters();
			self thread TriggerCrypt();
			break;
		}
		default: {
			break;
		}
	}
}

OpenBeastDoors()
{
	if (level.mapname != "zm_zod") {
		return;
	}
	thread WriteToScreen("Opening All Beast Breakables");
	foreach (smashable in level.zod_smashables) {
		smashable.m_e_trigger UseBy(self);
	}
	self thread OpenRifts();
}

OpenRifts()
{
	areas = Array("canal", "slums", "theater");
	foreach (area in areas) {
		self thread OpenZodPortal(area);
	}
}

OpenZodPortal(str_area)
{
	portal = [[ @zm_zod_portals<scripts\zm\zm_zod_portals.gsc>::function_42ed55f2 ]](str_area);
	if (!IsDefined(portal.unitrigger_stub)) {
		level.zod_smashables[str_area + "_portal"].m_e_trigger UseBy(self);
		while (!IsDefined(portal.unitrigger_stub)) {
			wait 0.05;
		}
		wait 0.1;
		trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(portal.unitrigger_stub, self);
		zm_unitrigger::assess_and_apply_visibility(trig, trig.stub, self, 0);
		trig notify("trigger", self);
		wait 0.1;
		trig Delete();
	}
}

EnablePads()
{
	pads = struct::get_array("115_flinger_landing_pad", "targetname");
	foreach (pad in pads) {
		foreach (stub in level._unitriggers.trigger_stubs) {
			if (IsDefined(stub.flag_name) && stub.flag_name == pad.script_noteworthy) {
				stub notify("trigger", self);
				break;
			}
		}
	}
}

TrapApothicon()
{
	trig = struct::get("apothicon_trap_trig", "targetname");
	if (!level flag::get("all_power_on")) {
		self ActivateAllPower();
	}
	level thread scene::skipto_end("cin_genesis_apothicon_flightpath", level.var_e7e8e5d6);
	level flag::wait_till("apothicon_near_trap");
	trig notify("trigger_activated", self);
}

EnableTeleporters()
{
	level flag::set("enable_teleporter_" + 1);
    level flag::set("enable_teleporter_" + 2);
    level flag::set("enable_teleporter_" + 3);
    level flag::set("enable_teleporter_" + 4);
	wait 1;
	level flag::clear("enable_teleporter_" + 1);
    level flag::clear("enable_teleporter_" + 2);
    level flag::clear("enable_teleporter_" + 3);
    level flag::clear("enable_teleporter_" + 4);
}

TriggerCrypt()
{
    door = getentArray("chamber_entrance", "targetname")[1];
    trig_position = struct::get(door.targetname + "_position", "targetname");
    level notify("open_sesame");
    trig_position.has_vinyl = 1;
    level.b_open_all_gramophone_doors = 1;
    wait 0.5;
    trig_position.trigger notify("trigger", self);
    wait 1;
    level.b_open_all_gramophone_doors = 0;
}

ActivateAllPower()
{
	thread WriteToScreen("Turning On All Powers");
    level flag::set("power_on");
    power_trigs = GetEntArray("use_elec_switch", "targetname");
    foreach (trig in power_trigs) {
        if (IsDefined(trig.script_int)) {
            level flag::set("power_on" + trig.script_int);
            level clientfield::set("zombie_power_on", trig.script_int);
			level thread zm_power::turn_power_on_and_open_doors(trig.script_int);
        }
    }
	foreach (volume in GetEntArray("power_volume", "targetname")) {
		level thread zm_power::turn_power_on_and_open_doors(volume.script_int);
	}

	if (level.mapname == "zm_island") {
		GetEnt("penstock_web_trigger", "targetname") notify("web_torn");
		level flag::set("defend_over");
		wait 0.1;
		GetEnt("use_elec_switch_deferred", "targetname") notify("trigger", self);
	}

    foreach (obj in Array("elec", "power", "master")) {
        trigs = GetEntArray("use_" + obj + "_switch", "targetname");
        if (IsDefined(trigs)) {
            foreach (trig in trigs) {
                trig notify("trigger", self);
            }
        }
    }

    if (level.mapname == "zm_tomb") {
		foreach (generator in struct::get_array("s_generator", "targetname")) {
			generator [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::set_player_controlled_area ]]();
			generator.n_current_progress = 100;
		}
	}
}

SetZBarrierState(state = 0)
{
	if (!state) {
		thread WriteToScreen("Opening All Barriers");
		zm_blockers::open_all_zbarriers();
	}
	else {
		thread WriteToScreen("Closing All Barriers");
		foreach (barrier in level.exterior_goals) {
			if (IsDefined(barrier.zbarrier)) {
				for (x = 0; x < barrier.zbarrier GetNumZBarrierPieces(); x++) {
					barrier.zbarrier SetZBarrierPieceState(x, "closing");
				}
			}
			if (IsDefined(barrier.clip)) {
				barrier.clip TriggerEnable(1);
				barrier.clip DisconnectPaths();
				continue;
			}
			zm_blockers::blocker_disconnect_paths(barrier.neg_start, barrier.neg_end);
		}
	}
}

ActivateBlocker(type, identifier)
{

	switch (type) {
		case "Doors": {
			if (identifier == "all") {
				self OpenAllDoors(false);
				return;
			}
			self OpenSpecificDoor(Int(identifier));
			break;
		}
		case "Power":
		case "Engines":
		case "Generators": {
			if (identifier == "all") {
				self ActivateAllPower();
				return;
			}
			self ActivateSpecificPower(identifier);
		}
		default: {
			self ActivateMapSpecificObject(identifier);
			break;
		}
	}
}

OpenSpecificDoor(num)
{
	SetDvar("zombie_unlock_all", 1);
	door = level.blockers[num];
	switch (door.targetname) {
		case "zombie_airlock_buy": {
			door thread [[ @zm_moon_utility<scripts\zm\zm_moon_utility.gsc>::moon_door_opened ]]();
			break;
		}
		default: {
			if (IsDefined(door) && !IsTrue(door.has_been_opened)) {
				door notify("trigger", self, 1);
			}
			break;
		}
	}
	SetDvar("zombie_unlock_all", 0);
}

ActivateSpecificPower(num)
{
	if (IsString(num) && StrIsInt(num)) {
		num = Int(num);
	}

	switch (level.mapname) {
		case "zm_zod": {
			if (level flag::get("power_on" + num)) {
				thread WriteToScreen("Power Already Activated");
				return;	
			}
			thread WriteToScreen("Activating Selected Power");
			level flag::set("power_on" + num);
			break;
		}
		case "zm_island": {
			if (IsInt(num)) {
				trigs = GetEntArray("use_elec_switch", "targetname");
				uses = self.var_6fd3d65c;
				self.var_6fd3d65c = 3;
				bucket = self clientfield::get_to_player("bucket_held");
				if (bucket) {
					self clientfield::set_to_player("bucket_held", 0);
				}
				trigs[num] notify("trigger", self);
				wait 0.05;
				self.var_6fd3d65c = uses;
				self clientfield::set_to_player("bucket_held", bucket);
				return;
			}
			else {
				self ActivateAllPower();
				if (num == "bunker") {
					level flag::set("power_on1");
					level flag::set("power_on2");
					level zm_power::turn_power_on_and_open_doors(3);
					level notify("override_bunker_door_string");
					return;
				}
			}
			break;
		}
		case "zm_genesis": {
			if (level flag::get("power_on" + num)) {
				thread WriteToScreen("Generator Already Activated");
				return;	
			}
			engine = GetEntArray("power_volume", "targetname")[num];
			level flag::set("power_on" + engine.script_int);
			level thread zm_power::turn_power_on_and_open_doors(engine.script_int);
			engine notify(engine.str_kill_notify);
			level notify("power_ritual_completed");
			break;
		}
		case "zm_tomb": {
			generator = struct::get_array("s_generator", "targetname")[num];
			if (generator flag::get("player_controlled")) {
				thread WriteToScreen("Generator Already Captured");
				return;
			}
			thread WriteToScreen("Capturing Selected Generator");
			generator [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::set_player_controlled_area ]]();
			generator.n_current_progress = 100;
			break;
		}
		default: {
			foreach (obj in Array("elec", "power", "master")) {
				trigs = GetEntArray("use_" + obj + "_switch", "targetname");
				if (IsDefined(trigs)) {
					foreach (trig in trigs) {
						trig notify("trigger", self);
					}
				}
			}
			break;
		}
	}
}

ActivateMapSpecificObject(identifier)
{
	switch (level.mapname) {
		case "zm_zod": {
			if (identifier == "all") {
				thread WriteToScreen("Breaking All Smashables");
				foreach (smashable in level.zod_smashables) {
					if (IsDefined(smashable.m_a_clip[0])) {
						smashable.m_e_trigger UseBy(self);
					}
				}
				return;
			}
			if (!IsDefined(level.zod_smashables[identifier].m_a_clip[0])) {
				thread WriteToScreen("Smashable Already Smashed");
				return;
			}
			thread WriteToScreen("Smashing Selected Smashable");
			level.zod_smashables[identifier].m_e_trigger UseBy(self);
			break;
		}
		case "zm_factory": {
			if (!level flag::get("power_on")) {
				thread WriteToScreen("Power Must Be Turned On");
				return;
			}
			if (identifier == "all") {
				thread WriteToScreen("Activating All TPs");
				foreach (index, tp in level.teleport) {
					if (tp != "waiting") {
						level.teleporter_pad_trig[index] notify("trigger");
						wait 0.1;
						GetEnt("trigger_teleport_core", "targetname") notify("trigger");
						wait 0.1;
					}
				}
				return;
			}
			if (level.teleport[Int(identifier)] != "waiting") {
				thread WriteToScreen("Teleporter Already Active");
				return;
			}
			thread WriteToScreen("Activating Selected TP");
			level.teleporter_pad_trig[Int(identifier)] notify("trigger");
			wait 0.1;
			GetEnt("trigger_teleport_core", "targetname") notify("trigger");
			break;
		}
		case "zm_castle": {
			if (identifier == "all") {
				thread WriteToScreen("Activating All Landing Pads");
				foreach (pad in struct::get_array("115_flinger_landing_pad", "targetname")) {
					if (!level flag::get(pad.script_noteworthy)) {
						foreach (stub in level._unitriggers.trigger_stubs) {
							if (IsDefined(stub.flag_name) && stub.flag_name == pad.script_noteworthy) {
								stub notify("trigger", self);
								break;
							}
						}
					}
				}
				return;
			}
			pad = struct::get_array("115_flinger_landing_pad", "targetname")[Int(identifier)];
			if (level flag::get(pad.script_noteworthy)) {
				thread WriteToScreen("Landing Pad Already Activated");
				return;
			}
			foreach (stub in level._unitriggers.trigger_stubs) {
				if (IsDefined(stub.flag_name) && stub.flag_name == pad.script_noteworthy) {
					thread WriteToScreen("Activating Selected Landing Pad");
					stub notify("trigger", self);
					break;
				}
			}
			break;
		}
		case "zm_island": {
			gate = GetEnt("pap_gate", "targetname");
			gate MoveTo(gate.s_pos.origin, 3);
			break;
		}
		case "zm_genesis": {
			self thread TrapApothicon();
			break;
		}
		case "zm_theater": {
			if (!level flag::get("power_on")) {
				thread WriteToScreen("Power Must Be On");
				return;
			}
			if (level flag::get("teleporter_linked")) {
				thread WriteToScreen("Teleporter Already Linked");
				return;
			}
			thread WriteToScreen("Linking TP");
			core = GetEnt("trigger_teleport_pad_0", "targetname");
			pad = GetEnt(core.target, "targetname");
			core notify("trigger", self);
			wait 0.1;
			pad notify("trigger", self);
			break;
		}
		case "zm_cosmodrome": {
			if (!level flag::get("power_on")) {
				thread WriteToScreen("Power Must Be On");
				return;
			}
			if (level flag::get("rocket_group")) {
				thread WriteToScreen("PAP Already Opened");
				return;
			}
			thread WriteToScreen("Opening PAP");
			[[ @zm_cosmodrome_pack_a_punch<scripts\zm\zm_cosmodrome_pack_a_punch.gsc>::pack_a_punch_open_door ]]();
			break;
		}
		case "zm_temple": {
			if (!level flag::get("power_on")) {
				thread WriteToScreen("Power Must Be On");
				return;
			}
			if (level flag::get("pap_active")) {
				thread WriteToScreen("PAP Already Activated");
				return;
			}
			thread WriteToScreen("Activating PAP");
			[[ @zm_temple_pack_a_punch<scripts\zm\zm_temple_pack_a_punch.gsc>::_pap_think ]]();
			break;
		}
		default: {
			break;
		}
	}
}