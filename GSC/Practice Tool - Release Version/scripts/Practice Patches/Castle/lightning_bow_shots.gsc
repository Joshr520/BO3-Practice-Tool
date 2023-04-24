LightningBowShotsPractice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_castle")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: DE");
        return;
    }
    thread WriteToScreen("Lightning Bow Shots Practice Starting");
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    level flag::clear("spawn_zombies");
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("elemental_bow"));
    self SetOrigin((-631.237, 1906.1, 608.125));
    self SetPlayerAngles((-8.62427, -64.0613, 0));
    self InfiniteAmmo(1);
    self thread ShowLightningBowControls();
    self thread WatchLightningBowShots();
    self thread ToggleBeaconFX();
    self thread ToggleDogFog();
    self thread CameraPos(0x76);
    self thread CameraToggle(0x77);
    self thread WaypointSave(0xDB);
    self thread WaypointLoad(0xDD);
}

WatchLightningBowShots()
{
    beacons = GetEntArray("aq_es_beacon_trig", "script_noteworthy");
    for(;;)
    {
        hit = 0;
        self waittill("projectile_impact", weapon, point, radius, attacker, normal);
        foreach(beacon in beacons)
        {
            if(CheckBowHit(point, beacon))
            {
                hit = 1;
                break;
            }
        }
        if(hit) IPrintLnBold("Beacon Hit");
    }
}

CheckBowHit(impact, damage)
{
	touching = Spawn("script_origin", impact);
    if(!touching IsTouching(damage))
    {
        touching Delete();
        return false;
    }
    touching Delete();
	return true;
}

ToggleBeaconFX()
{
    beacons = GetEntArray("aq_es_beacon_trig", "script_noteworthy");
    foreach(beacon in beacons)
    {
        beacon_fx = struct::get(beacon.target);
        beacon_fx.fx = util::spawn_model("tag_origin", beacon_fx.origin);
    }
    for(;;)
    {
        WaitF5();
        foreach(beacon in beacons)
        {
            beacon_fx = struct::get(beacon.target);
            beacon_fx.fx clientfield::set("beacon_fx", 1);
        }
        IPrintLnBold("Beacons Activated");
        wait 1;
        WaitF5();
        foreach(beacon in beacons)
        {
            beacon_fx = struct::get(beacon.target);
            beacon_fx.fx clientfield::set("beacon_fx", 0);
        }
        IPrintLnBold("Beacons Deactivated");
        wait 1;
    }
}

ToggleDogFog()
{
    for(;;)
    {
        WaitF6();
        level clientfield::set("castle_fog_bank_switch", 1);
        IPrintLnBold("Fog Start");
        wait 1;
        WaitF6();
        level clientfield::set("castle_fog_bank_switch", 0);
        IPrintLnBold("Fog Stop");
        wait 1;
    }
}

ShowLightningBowControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 20, 125);
    controls SetText("Controls List\nHide Controls: F3\nToggle Beacon FX: F5\nToggle Fog: F6\nSet Camera Pos: F7\nToggle Camera: F8\nSave Waypoint: [\nLoad Waypoint: ]");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}