SimonSays()
{
    level flag::wait_till("ee_golden_key");
    wait 0.5;
    tc = struct::get("tc_launch_platform");
    tc notify("trigger_activated");
    tc = struct::get("tc_lower_tower");
    tc notify("trigger_activated");
    deathray = GetEnt("ee_death_ray_switch", "targetname");
    deathray notify("trigger_activated");
    wait 0.5;
    simon_1 = struct::get_array("golden_key_slot")[0];
    simon_2 = struct::get_array("golden_key_slot")[1];
    simon_1 notify("trigger_activated");
    level waittill(#"hash_706f7f9a");
    wait 1;
    level.var_521b0bd1 = 8;
    monitor = struct::get("monitor_lower_tower_1");
    monitor.var_73527aa3 = 1;
    monitor notify("trigger_activated");
    wait 5;
    simon_2 notify("trigger_activated");
    level waittill(#"hash_706f7f9a");
    wait 1;
    level.var_521b0bd1 = 8;
    monitor = struct::get("monitor_launch_platform_1");
    monitor.var_73527aa3 = 1;
    monitor notify("trigger_activated");
    wait 5;
    level.simon_complete = 1;
}