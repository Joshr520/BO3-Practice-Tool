FireBowShotsPractice()
{
    WaitFadeIn();
    if(level.script != "zm_castle")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: DE");
        return;
    }
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Fire Bow Shots Practice Starting");
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    level flag::clear("spawn_zombies");
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("elemental_bow"));
    self InfiniteAmmo(1);
    self thread WatchFireBowShots();
    self thread WaypointSave(0xDB);
    self thread WaypointLoad(0xDD);
}

WatchFireBowShots()
{
    obelisk = GetEnt("aq_rp_obelisk_magma_trig", "targetname");
    circles = GetEntArray("aq_rp_runic_circle_volume", "script_noteworthy");
    array::thread_all(circles, ::WaitCircleHit);
    for(;;)
    {
        obelisk waittill("damage", amount, attacker, direction, point, mod, tagname, modelname, partname, weapon);
        if(CheckBowHit(point, obelisk)) IPrintLnBold("Obelisk Hit");
    }
}

WaitCircleHit()
{
    damage = GetEnt(self.target + "_trig", "targetname");
    for(;;)
    {
        damage waittill("damage", amount, attacker, direction, point, mod, tagname, modelname, partname, weapon);
        if(CheckBowHit(point, damage)) IPrintLnBold("Circle Hit");
    }
}

ShowFireBowControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 20, 125);
    controls SetText("Controls List\nHide Controls: F3\nToggle Beacon FX: F5\nToggle Fog: F6\nSave Waypoint: [\nLoad Waypoint: ]");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}