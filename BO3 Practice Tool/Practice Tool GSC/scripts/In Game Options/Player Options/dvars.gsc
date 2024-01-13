Timescale(value)
{
    thread WriteToScreen("Changing Timescale To " + value);
    SetDvar("timescale", value);
}

SetSpeed(value)
{
    thread WriteToScreen("Changing Speed To " + value);
    SetDvar("g_speed", value);
}

ThirdPerson(num)
{
    text = "POV First Person";
    if (num) {
        text = "POV Third Person";
    }
    thread WriteToScreen(text);
    SetDvar("cg_thirdperson", num);
}

Godmode(enabled)
{
    level notify("godmode_request");
    level endon("godmode_request");

    if (enabled) {
        thread WriteToScreen("Enabling Godmode");
        for (;;) {
            self EnableInvulnerability();
            wait 0.25;
        }
    }
    else {
        thread WriteToScreen("Disabling Godmode");
        level notify("godmode_end");
        wait 0.5;
        self DisableInvulnerability();
    }
}

InfiniteAmmo(enabled)
{
    self notify("infinite_ammo_change");
    if (enabled) {
        thread WriteToScreen("Infinite Ammo Enabled");
    }
    else {
        thread WriteToScreen("Infinite Ammo Disabled");
    }
    SetDvar("player_sustainAmmo", enabled);
    self thread GiveAmmoOnShoot();
}

GiveAmmoOnShoot()
{
    self endon("infinite_ammo_change");
    for (;;) {
        self waittill("weapon_fired");
        TrueMaxAmmo();
    }
}