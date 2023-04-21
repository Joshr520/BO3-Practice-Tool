TimeTravel(num)
{
    upgrades = Array("demon_gate_upgraded", "elemental_storm_upgraded", "rune_prison_upgraded", "wolf_howl_upgraded");
	level flag::wait_till_any(upgrades);
    level flag::set("ee_start_done");
    if(num >= 1)
    {
        level flag::set("time_travel_teleporter_ready");
        wait 0.25;
        fuse = struct::get("ee_lab_fuse");
        canister = struct::get("mpd_canister");
        fuse_found = 0;
        canister_found = 0;
        for(;;)
        {
            foreach(stub in level._unitriggers.trigger_stubs)
            {
                if(!fuse_found && fuse.origin == stub.origin)
                {
                    self BuildAndActivateTrigger(stub);
                    fuse_found = 1;
                }
                if(!canister_found && canister.origin == stub.origin)
                {
                    self BuildAndActivateTrigger(stub);
                    canister_found = 1;
                }
            }
            if(fuse_found && canister_found) break;
            wait 0.05;
        }
        wait 0.5;
        level flag::clear("time_travel_teleporter_ready");
    }
    if(num >= 2)
    {
        level flag::set("death_ray_trap_used");
        fuse_box = GetEnt("fuse_box", "targetname");
        fuse_box notify("trigger_activated");
        wait 0.5;
        deathray = GetEnt("ee_death_ray_switch", "targetname");
        deathray notify("trigger_activated");
        wait 0.5;
        level.var_ab58bca7 = [0, 0, 0];
        monitor = struct::get("monitor_launch_platform_1");
        monitor.var_d82c7c68 = 1;
        for(i = 0; i < 3; i++)
        {
            monitor notify("trigger_activated");
            wait 1.5;
        }
        key = GetEnt("golden_key", "targetname");
        key notify("trigger_activated");
        wait 0.5;
    }
    if(num == 3)
    {
        level flag::set("time_travel_teleporter_ready");
        key_slot = struct::get("golden_key_slot_past");
        key_slot notify("trigger_activated");
        wait 0.5;
        tablet = GetEnt("stone_past", "targetname");
        tablet notify("trigger_activated");
        wait 0.5;
        level flag::clear("time_travel_teleporter_ready");
    }
}