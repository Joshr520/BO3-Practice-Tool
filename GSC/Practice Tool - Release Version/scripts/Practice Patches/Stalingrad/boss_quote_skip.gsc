BossQuoteSkipPractice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_stalingrad")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Stalingrad");
        return;
    }
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    self SetOrigin((-275.725, 4768.26, 144.125));
    self SetPlayerAngles((5.32288, 179.615, 0));
    level flag::clear("spawn_zombies");
    level flag::set("story_playing");
    IPrintLnBold("Wait...");
    wait 10;
    level flag::clear("story_playing");
    level notify("ee_cylinder_acquired");
    level.var_a090a655 HidePart("button_green_jnt");
	level.var_a090a655 ShowPart("button_red_jnt");
    IPrintLnBold("Melee To Start Countdown");
    while(!self MeleeButtonPressed()) wait 0.05;
    timer = hud::createfontstring("big", 2);
    timer hud::setpoint("TOPLEFT", "TOPLEFT", 5, 200);
    timer SetTenthsTimer(5);
    wait 5;
    timer Destroy();
    level flag::set("weapon_cores_delivered");
}