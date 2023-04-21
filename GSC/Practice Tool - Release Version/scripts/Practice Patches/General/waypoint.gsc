WaypointSetup(controls = 0)
{
    self thread WaypointSave(0xDB);
    self thread WaypointLoad(0xDD);
    if(controls) self thread ShowWaypointControls();
}

WaypointSave(key)
{
    for(;;)
    {
        while(!compiler::WaitKey(key)) wait 0.05;
        thread WriteToScreen("Waypoint Saved");
        self.waypoint = Array(self GetOrigin(), self GetPlayerAngles());
    }
}

WaypointLoad(key)
{
    for(;;)
    {
        while(!compiler::WaitKey(key)) wait 0.05;
        if(IsDefined(self.waypoint))
        {
            thread WriteToScreen("Waypoint Loaded");
            self SetOrigin(self.waypoint[0]);
            self SetPlayerAngles(self.waypoint[1]);
        }
        else thread WriteToScreen("No Waypoint Set");
    }
}

ShowWaypointControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 20, 125);
    controls SetText("Controls List\nHide Controls: F3\nSave Waypoint: [\nLoad Waypoint: ]");
    for(;;)
    {
        controls.alpha = 1;
        while(!compiler::waitkey(0x72)) wait 0.05;
        controls.alpha = 0;
        while(!compiler::waitkey(0x72)) wait 0.05;
    }
}