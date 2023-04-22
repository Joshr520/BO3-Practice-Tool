InitBlockerLists()
{
	level.blockers_doors_list = [];
	level.non_unique_blockers_list = [];
	level.blockers_power_list = [];
	level.blockers_special_list = [];
	switch(level.script)
	{
		case "zm_zod":
			level.blockers_doors_list = Array("connect_start_to_junction", "connect_canal_to_junction", "connect_canal_to_train", "connect_canal_high_to_train", "connect_theater_to_junction", "connect_theater_alley_to_square");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_theater_high_to_square", "connect_slums_to_junction", "connect_slums_waterfront_to_alley", "connect_slums_high_to_train", "activate_canal"), 1, 0);
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("activate_theater_square", "activate_slums_junction_alley", "activate_slums_waterfront"), 1, 0);
			level.blockers_power_list = Array(1, 4, 5, 6, 2, 3, 7, 14, 12, 16, 13, 21, 11, 15, 20, 23);
			level.blockers_special_list = Array("pf29459_auto3", "pf29461_auto3", "pf29468_auto3", "pf29470_auto3", "unlock_quest_key", "memento_detective_drop", "memento_femme_drop", "memento_boxer_drop", "smash_trigger_open_slums");
			level.blockers_special_list = ArrayCombine(level.blockers_special_list, Array("smash_unnamed_0", "canal_portal", "theater_portal", "slums_portal"), 1, 0);
			break;
		case "zm_factory":
			level.blockers_doors_list = Array("enter_outside_west", "enter_warehouse_building", "enter_warehouse_second_floor", "enter_tp_west", "enter_outside_east", "enter_wnuen_building", "enter_wnuen_loading_dock", "enter_tp_east", "enter_tp_south");
			break;
		case "zm_castle":
			level.blockers_doors_list = Array("connect_start_to_subclocktower", "connect_subclocktower_to_courtyard", "connect_courtyard_to_greathall_upper", "connect_greathall_to_armory", "connect_start_to_tram2gatehouse");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_gatehouse_to_lowercourtyard", "connect_lowercourtyard_to_livingquarters", "connect_armory_to_livingquarters", "connect_undercroft_to_undercroft_lab"), 1, 0);
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("buyable_dropdown_gatehouse_to_lowercourtyard"), 1, 0);
			level.non_unique_blockers_list = Array((-581, 3336, 876), (-1125, 3876, 876), (1232, 2687, 628), (1676, 2307, 580));
			break;
		case "zm_island":
			level.blockers_doors_list = Array("connect_start_2_to_jungle", "connect_jungle_to_crash_site", "connect_jungle_to_jungle_lab", "connect_jungle_lab_to_jungle_lab_upper", "connect_jungle_lab_to_bunker_exterior", "connect_start_2_to_swamp");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_swamp_to_ruins", "connect_swamp_to_swamp_lab", "connect_swamp_lab_to_swamp_lab_inside", "connect_swamp_lab_to_meteor_site", "connect_swamp_lab_to_bunker_exterior"), 1, 0);
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_bunker_exterior_to_bunker_interior", "connect_bunker_interior_to_bunker_upper", "connect_bunker_interior_to_bunker_left", "connect_cliffside_to_bunker_left"), 1, 0);
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_bunker_interior_to_bunker_right", "connect_bunker_interior_to_operating_rooms", "connect_operating_rooms_to_flooded_bunker_right"), 1, 0);
			level.non_unique_blockers_list = Array((1329, 2040, -143));
			break;
		case "zm_stalingrad":
			level.blockers_doors_list = Array("department_store_open", "alley_to_department_store_open", "department_store_2f_to_3f", "yellow_to_bunker_open", "red_brick_to_bunker_open", "dept_to_yellow", "department_floor3_to_red_brick_open");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("yellow_to_judicial_street_open", "library_open", "factory_open", "library_street_dropdown_open", "factory_bridge_dropdown_open"), 1, 0);
			break;
		case "zm_genesis":
			level.blockers_doors_list = Array("connect_start_to_left", "connect_start_to_right", "connect_temple_to_temple_stairs", "connect_undercroft_to_temple", "connect_undercroft_to_foyer", "connect_undercroft_to_theater");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_asylum_downstairs_to_upstairs", "connect_asylum_kitchen_to_upstairs", "connect_cellblock_to_messhall", "connect_ruins_to_inner_ruins"), 1, 0);
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("connect_generator_to_trenches", "connect_prototype_start_to_upstairs", "prison_drop_down_power_opened", "prison_drop_down_opened"), 1, 0);
			break;
		case "zm_prototype":
			level.blockers_doors_list = Array("start_2_box", "start_2_upstairs", "box_2_upstairs");
			break;
		case "zm_asylum":
			level.blockers_doors_list = Array("north_door1", "south_upstairs_blocker", "north_upstairs_blocker", "upstairs_north_door1", "upstairs_north_door2", "magic_box_north", "south_access_1", "magic_box_south");
			break;
		case "zm_sumpf":
			level.blockers_doors_list = Array("unlock_hospital_downstairs", "unlock_hospital_upstairs", "sw_magic_box", "southwest_building_unlocked", "se_magic_box", "southeast_building_unlocked", "ne_magic_box", "northeast_building_unlocked");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("nw_magic_box", "northwest_building_unlocked"), 1, 0);
			break;
		case "zm_theater":
			level.blockers_doors_list = Array("magic_box_crematorium1", "magic_box_alleyway1", "magic_box_west_balcony1", "magic_box_west_balcony2", "magic_box_foyer1", "vip_to_dining", "dining_to_dressing", "magic_box_dressing1");
			break;
		case "zm_cosmodrome":
			level.blockers_doors_list = Array("centrifuge2power", "power2centrifuge", "tunnel_centrifuge_entry", "base_entry_2_power", "power_interior_2_roof", "power_catwalk_access", "base_entry_2_north_path", "catwalks_2_shed");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("base_entry_2_storage", "storage_lander_area"), 1, 0);
			break;
		case "zm_temple":
			level.blockers_doors_list = Array("start_to_waterfall_upper", "waterfall_to_tunnel", "cave_water_to_waterfall", "cave_water_to_power", "start_to_pressure", "pressure_to_cave01", "cave01_to_cave02", "cave03_to_power");
			break;
		case "zm_moon":
			level.blockers_doors_list = Array("receiving_exit", "catacombs_west", "tunnel_6_door1", "catacombs_west4", "catacombs_east",  "tunnel_11_door1", "tunnel_11_door2", "catacombs_east4", "generator_exit_east", "dig_enter_east");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("enter_forest_east", "exit_dig_east", "forest_enter_digsite"), 1, 0);
			break;
		case "zm_tomb":
			level.blockers_doors_list = Array("activate_zone_bunker_1", "activate_zone_bunker_3a", "activate_zone_bunker_3b", "activate_zone_bunker_2", "activate_zone_bunker_4a", "activate_zone_bunker_4b");
			level.blockers_doors_list = ArrayCombine(level.blockers_doors_list, Array("activate_zone_nml", "activate_zone_ruins", "activate_zone_farm", "activate_zone_village_0", "activate_zone_village_1"), 1, 0);
			// 3, 5, 4, 1, 2, 6
			level.blockers_power_list = Array(3, 4, 0, 2, 1, 5);
			break;
	}
}

OpenAllDoors()
{
	if(IsTrue(level.blockers_open))
	{
		thread WriteToScreen("Doors Already Opened");
		return;
	}
	else
	{
		level.blockers_open = 1;
		thread WriteToScreen("Opening All Doors");
	}
	SetDvar("zombie_unlock_all", 1);
    types = Array("zombie_door", "zombie_airlock_buy", "zombie_debris");
    foreach(type in types)
    {
        doors = GetEntArray(type, "targetname");
        foreach(door in doors)
        {
			switch(type)
			{
				case "zombie_airlock_buy":
					door thread [[ @zm_moon_utility<scripts\zm\zm_moon_utility.gsc>::moon_door_opened ]]();
					break;
				default:
					if(IsDefined(door) && !IsTrue(door.has_been_opened)) door notify("trigger", self, 1);
					wait 0.1;
				break;
			}
        }
    }
	SetDvar("zombie_unlock_all", 0);

	foreach(zone in GetArrayKeys(level.zones)) zm_zonemgr::enable_zone(zone);

	switch(level.script)
	{
		case "zm_zod":
			self thread OpenBeastDoors();
			break;
		case "zm_castle":
			self thread EnablePads();
			break;
		case "zm_island":
			gate = GetEnt("pap_gate", "targetname");
			gate MoveTo(gate.s_pos.origin, 3);
			level flag::set("power_on1");
			level flag::set("power_on2");
			level zm_power::turn_power_on_and_open_doors(3);
			level notify("override_bunker_door_string");
			wait 0.1;
			OpenDoorByFlagOrOrigin("connect_bunker_exterior_to_bunker_interior");
			break;
		case "zm_genesis":
			self thread TrapApothicon();
		case "zm_tomb":
			self thread EnableTeleporters();
			self thread TriggerCrypt();
			break;
	}
}

OpenBeastDoors()
{
	if(level.script != "zm_zod") return;
	if(IsTrue(level.smashables_open))
	{
		thread WriteToScreen("Beast Breakables Already Broken");
		return;
	}
	else
	{
		level.smashables_open = 1;
		thread WriteToScreen("Opening All Beast Breakables");
	}
	foreach(smashable in level.zod_smashables)
	{
		smashable.m_e_trigger UseBy(level.players[0]);
	}

	wait 3;

	self thread OpenRifts();
}

OpenRifts()
{
	wait 3;
	foreach(index, stub in level._unitriggers.trigger_stubs)
	{
		if(stub.origin == (2064.2, -7058.48, 48) || stub.origin == (4142.14, -3399.36, 176) || stub.origin == (1559.13, -3089.37, 160))
		{
			self BuildAndActivateTrigger(stub);
		}
	}
}

EnablePads()
{
	wait 3;
	pads = struct::get_array("115_flinger_landing_pad", "targetname");
	foreach(pad in pads)
	{
		foreach(stub in level._unitriggers.trigger_stubs)
		{
			if(IsDefined(stub.flag_name) && stub.flag_name == pad.script_noteworthy)
			{
				self BuildAndActivateTrigger(stub);
				break;
			}
		}
	}

	bgb = self.bgb;
	self.bgb = "zm_bgb_shopping_free";
	level.n_teleport_cooldown = 1;
	teles = GetEntArray("trigger_teleport_pad", "targetname");
	foreach(tele in teles)
	{
		tele notify("trigger", self);
	}
	wait 0.5;
	self.bgb = bgb;
	level.is_cooldown = 0;
	wait 5;
	level.n_teleport_cooldown = 45;
}

TrapApothicon()
{
	trig = struct::get("apothicon_trap_trig", "targetname");
	if(!level flag::get("all_power_on")) self ActivateAllPower();
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

ActivateAllPower(excludes = Array())
{
	if(IsTrue(level.power_on))
	{
		thread WriteToScreen("Power Already On");
		return;
	}
	else
	{
		level.power_on = 1;
		thread WriteToScreen("Turning On All Powers");
	}
    level flag::set("power_on");
    power_trigs = GetEntArray("use_elec_switch", "targetname");
    foreach(trig in power_trigs)
    {
		skip = false;
		foreach(exclude in excludes)
		{
			if(trig.origin == exclude) skip = true;
		}
		if(skip) continue;
        if(IsDefined(trig.script_int))
        {
            level flag::set("power_on" + trig.script_int);
            level clientfield::set("zombie_power_on", trig.script_int);
			level thread zm_power::turn_power_on_and_open_doors(trig.script_int);
        }
    }
	foreach(volume in GetEntArray("power_volume", "targetname"))
	{
		level thread zm_power::turn_power_on_and_open_doors(volume.script_int);
	}
    foreach(obj in Array("elec", "power", "master"))
    {
        trigs = GetEntArray("use_" + obj + "_switch", "targetname");
        if(IsDefined(trigs))
        {
            foreach(trig in trigs)
            {
				skip = false;
				foreach(exclude in excludes)
				{
					if(trig.origin == exclude) skip = true;
				}
				if(skip) continue;
                trig notify("trigger", level.players[0]);
            }
        }
    }

    foreach(generator in struct::get_array("s_generator", "targetname"))
	{
		generator [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::set_player_controlled_area ]]();
		generator.n_current_progress = 100;
	}
}

SetZBarrierState(state = 0)
{
	if(!state)
	{
		thread WriteToScreen("Opening All Barriers");
		zm_blockers::open_all_zbarriers();
	}
	else
	{
		thread WriteToScreen("Closing All Barriers");
		foreach(barrier in level.exterior_goals)
		{
			if(IsDefined(barrier.zbarrier))
			{
				for(x = 0; x < barrier.zbarrier GetNumZBarrierPieces(); x++)
				{
					barrier.zbarrier SetZBarrierPieceState(x, "closing");
				}
			}
			if(IsDefined(barrier.clip))
			{
				barrier.clip TriggerEnable(1);
				barrier.clip DisconnectPaths();
				continue;
			}
			zm_blockers::blocker_disconnect_paths(barrier.neg_start, barrier.neg_end);
		}
	}
}

OpenSpecificDoor(num)
{
	if(num < level.blockers_doors_list.size) OpenDoorByFlagOrOrigin(level.blockers_doors_list[num]);
	else OpenDoorByFlagOrOrigin(level.non_unique_blockers_list[num - level.blockers_doors_list.size]);
}

ActivateSpecificPower(num)
{
	switch(level.script)
	{
		case "zm_zod":
		{
			power_num = level.blockers_power_list[num];
			if(level flag::get("power_on" + power_num))
			{
				thread WriteToScreen("Power Already Activated");
				return;	
			}
			thread WriteToScreen("Activating Selected Power");
			level flag::set("power_on" + power_num);
			break;
		}
		case "zm_genesis":
		{
			if(level flag::get("power_on" + Int(num + 1)))
			{
				thread WriteToScreen("Generator Already Activated");
				return;
			}
			thread WriteToScreen("Activating Selected Generator");
			level flag::set("power_on" + Int(num + 1));
			level thread zm_power::turn_power_on_and_open_doors(num + 1);
			break;
		}
		case "zm_tomb":
		{
			generator = struct::get_array("s_generator", "targetname")[level.blockers_power_list[num]];
			if(generator flag::get("player_controlled"))
			{
				thread WriteToScreen("Generator Already Captured");
				return;
			}
			thread WriteToScreen("Capturing Selected Generator");
			generator [[ @zm_tomb_capture_zones<scripts\zm\zm_tomb_capture_zones.gsc>::set_player_controlled_area ]]();
			generator.n_current_progress = 100;
			break;
		}
		default:
			break;
	}
}

ActivateMapSpecificObject(num)
{
	switch(level.script)
	{
		case "zm_zod":
		{
			if(!IsDefined(level.zod_smashables[level.blockers_special_list[num]].m_a_clip[0]))
			{
				thread WriteToScreen("Smashable Already Smashed");
				return;
			}
			thread WriteToScreen("Smashing Selected Smashable");
			level.zod_smashables[level.blockers_special_list[num]].m_e_trigger UseBy(level.players[0]);
			break;
		}
		case "zm_factory":
		{
			if(!level flag::get("power_on"))
			{
				thread WriteToScreen("Power Must Be Turned On");
				return;
			}
			if(level.teleport[num] != "waiting")
			{
				thread WriteToScreen("Teleporter Already Active");
				return;
			}
			thread WriteToScreen("Activating Selected TP");
			level.teleporter_pad_trig[num] notify("trigger");
			wait 0.1;
			GetEnt("trigger_teleport_core", "targetname") notify("trigger");
			break;
		}
		case "zm_castle":
		{
			pad = struct::get_array("115_flinger_landing_pad", "targetname")[num];
			if(level flag::get(pad.script_noteworthy))
			{
				thread WriteToScreen("Landing Pad Already Activated");
				return;
			}
			foreach(stub in level._unitriggers.trigger_stubs)
			{
				if(IsDefined(stub.flag_name) && stub.flag_name == pad.script_noteworthy)
				{
					thread WriteToScreen("Activating Selected Landing Pad");
					self BuildAndActivateTrigger(stub);
					break;
				}
			}
			break;
		}
		case "zm_genesis":
		{
			if(level flag::get("apothicon_trapped"))
			{
				thread WriteToScreen("Apothicon Allreadt Trapped");
				return;	
			}
			thread WriteToScreen("Trapping Apothicon");
			self thread TrapApothicon();
			break;
		}
		case "zm_theater":
		{
			if(!level flag::get("power_on"))
			{
				thread WriteToScreen("Power Must Be On");
				return;
			}
			if(level flag::get("teleporter_linked"))
			{
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
		case "zm_cosmodrome":
		{
			if(!level flag::get("power_on"))
			{
				thread WriteToScreen("Power Must Be On");
				return;
			}
			if(level flag::get("launch_complete"))
			{
				thread WriteToScreen("PAP Already Opened");
				return;
			}
			thread WriteToScreen("Opening PAP");
			level flag::set("lander_a_used");
			level flag::set("lander_b_used");
			level flag::set("lander_c_used");
			level flag::set("launch_activated");
			wait 0.1;
			GetEnt("trig_launch_rocket", "targetname") notify("trigger", self);
			break;
		}
		case "zm_temple":
		{
			if(!level flag::get("power_on"))
			{
				thread WriteToScreen("Power Must Be On");
				return;
			}
			if(level flag::get("pap_active"))
			{
				thread WriteToScreen("PAP Already Activated");
				return;
			}
			thread WriteToScreen("Activating PAP");
			[[ @zm_temple_pack_a_punch<scripts\zm\zm_temple_pack_a_punch.gsc>::_pap_think ]]();
			break;
		}
		case "zm_moon":
		{
			if(level flag::get("enter_nml"))
			{
				if(level.teleporter_exit_nml_gate_open)
				{
					thread WriteToScreen("Gate Already Opened");
					return;
				}
				thread WriteToScreen("Opening TP Gate");
				[[ @zm_moon_teleporter<scripts\zm\zm_moon_teleporter.gsc>::teleporter_exit_nml_gate_move ]](1);
				return;
			}
			if(level.teleporter_to_nml_gate_open)
			{
				thread WriteToScreen("Gate Already Opened");
				return;
			}
			thread WriteToScreen("Opening TP Gate");
			[[ @zm_moon_teleporter<scripts\zm\zm_moon_teleporter.gsc>::teleporter_to_nml_gate_move ]](1);
			break;
		}
		default:
			break;
	}
}

OpenDoorByFlagOrOrigin(flag_org)
{
	if(IsString(flag_org))
	{
		types = Array("zombie_door", "zombie_airlock_buy", "zombie_debris");
		foreach(type in types)
		{
			doors = GetEntArray(type, "targetname");
			foreach(door in doors)
			{
				if(!IsDefined(door) || !IsDefined(door.script_flag) || door.script_flag != flag_org) continue;
				if(IsTrue(door.has_been_opened))
				{
					thread WriteToScreen("Door Already Opened");
					return;
				}
				SetDvar("zombie_unlock_all", 1);
				thread WriteToScreen("Opening Selected Door");
				if(door.script_flag == "connect_bunker_exterior_to_bunker_interior")
				{
					level zm_power::turn_power_on_and_open_doors(3);
					level notify("override_bunker_door_string");
					wait 0.1;
				}
				switch(type)
				{
					case "zombie_airlock_buy":
						door thread [[ @zm_moon_utility<scripts\zm\zm_moon_utility.gsc>::moon_door_opened ]]();
						break;
					default:
						door notify("trigger", self, 1);
						break;
				}
				wait 0.1;
				SetDvar("zombie_unlock_all", 0);
				return;
			}
		}
		thread WriteToScreen("Door Already Opened");
		return;
	}
	types = Array("zombie_door", "zombie_airlock_buy", "zombie_debris");
	foreach(type in types)
	{
		doors = GetEntArray(type, "targetname");
		foreach(door in doors)
		{
			if(!IsDefined(door) || !IsDefined(door.origin) || door.origin != flag_org) continue;
			if(IsTrue(door.has_been_opened))
			{
				thread WriteToScreen("Door Already Opened");
				return;
			}
			SetDvar("zombie_unlock_all", 1);
			thread WriteToScreen("Opening Selected Door");
			switch(type)
			{
				case "zombie_airlock_buy":
					door thread [[ @zm_moon_utility<scripts\zm\zm_moon_utility.gsc>::moon_door_opened ]]();
					break;
				default:
					door notify("trigger", self, 1);
					break;
			}
			wait 0.1;
			SetDvar("zombie_unlock_all", 0);
			return;
		}
	}
	thread WriteToScreen("Door Not Found");
}