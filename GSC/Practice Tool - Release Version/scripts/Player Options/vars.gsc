Timescale(value)
{
    thread WriteToScreen("Changing Timescale To " + value);
    SetDvar("timescale", value);
}

ThirdPerson(num)
{
    text = "POV First Person";
    if(num) text = "POV Third Person";
    thread WriteToScreen(text);
    SetDvar("cg_thirdperson", num);
}

Godmode(num)
{
    if(num)
    {
        thread WriteToScreen("Enabling Godmode");
        level endon("godmode_end");
        for(;;)
        {
            self EnableInvulnerability();
            wait 0.25;
        }
    }
    else
    {
        thread WriteToScreen("Disabling Godmode");
        level notify("godmode_end");
        wait 0.5;
        self DisableInvulnerability();
    }
}

InfiniteAmmo(value)
{
    text = "Infinite Ammo Disabled";
    if(value) text = "Infinite Ammo Enabled";
    thread WriteToScreen(text);
    SetDvar("player_sustainAmmo", value);
}