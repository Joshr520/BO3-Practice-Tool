AFDGstrikePractice()
{
    WaitFadeIn();
    if(level.script != "zm_tomb")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Origins");
        return;
    }

    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Rain Fire Panzers Practice Loaded");

    level flag::clear("spawn_zombies");
    self thread DebugTesting();
    self thread WaypointSetup(1);
    self thread zm_weapons::weapon_give(level.w_beacon);
    quadrotor = GetWeapon("equip_dieseldrone");
    self zm_weapons::weapon_give(quadrotor);
	self SetWeaponAmmoClip(quadrotor, 1);
    self SetACtionSlot(4, "weapon", quadrotor);
    self clientfield::set_player_uimodel("hudItems.showDpadRight_Drone", 1);
    self notify("equip_dieseldrone_zm_given");
    level thread [[ @zm_tomb_craftables<scripts\zm\zm_tomb_craftables.gsc>::quadrotor_watcher ]](self);

    level flag::set("ee_all_staffs_crafted");
	level flag::set("ee_all_staffs_upgraded");
    level flag::set("ee_all_staffs_placed");
    level flag::set("fire_link_enabled");
}