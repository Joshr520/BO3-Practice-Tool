FlytrapPractice()
{
    self thread OpenAllDoors();
    self.score = 50000;
    self thread InfiniteAmmo(1);
    level flag::clear("spawn_zombies");
    thread HideAndSeek("ee_exp_monkey");
    thread HideAndSeek("ee_bowie_bear");
    thread HideAndSeek("ee_perk_bear");

    self thread ShowFlytrapControls();
    self thread WaypointSave(0xDB);
    self thread WaypointLoad(0xDD);
    self thread CameraPos(0x75);
    self thread CameraToggle(0x76);

    for(;;)
    {
        WaitF5();
        self SetPlayerAngles(VectortoAngles(GetEnt("trig_ee_perk_bear", "targetname").origin - self GetTagOrigin("j_head")));
    }
}

HideAndSeek(targetname)
{
    obj_array = GetEntArray(targetname, "targetname");
    trig = GetEnt("trig_" + targetname, "targetname");
    foreach(obj in obj_array)
	{
		obj Show();
	}
    trig TriggerEnable(1);
    for(;;)
    {
        trig waittill("trigger");
        thread WriteToScreen("Triggered: " + targetname);
    }
}

ShowFlytrapControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 20, 125);
    controls SetText("Controls List\nHide Controls: F3\nTarget Bear: F5\nSet Camera Pos: F6\nToggle Camera: F7\nSave Waypoint: [\nLoad Waypoint: ]");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}