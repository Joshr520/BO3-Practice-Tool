InfiniteBeastStart(hud = 1)
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_zod")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: SOE");
        return;
    }
    level endon("infinite_beast_end");
    thread WriteToScreen("Starting Infinite Beast");
    self.score = 50000;
    level clientfield::set("bm_superbeast", 1);
    level flag::clear("spawn_zombies");
    thread InfiniteKiosks();
    self thread InfiniteLives();
    if(hud)
    {
        self thread ShowInfiniteBeastControls();
        self thread WaypointSave(0xDB);
        self thread WaypointLoad(0xDD);
    }
}

InfiniteKiosks()
{
    level endon("infinite_beast_end");
    for(;;)
    {
        level waittill("kiosk_used", kiosk);
        wait 0.25;
        kiosk.is_cooling_down = 0;
    }
}

InfiniteLives()
{
    level endon("infinite_beast_end");
    for(;;)
    {
        self.og_beastlives = self.beastlives;
        level waittill("kiosk_used", kiosk);
        self.beastlives = 99;
    }
}

InfiniteBeastEnd()
{
    level notify("infinite_beast_end");
    self.beastlives = self.og_beastlives;
}

ShowInfiniteBeastControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 20, 125);
    controls SetText("Controls List\nHide Controls: F3\nSave Waypoint: [\nLoad Waypoint: ]");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}