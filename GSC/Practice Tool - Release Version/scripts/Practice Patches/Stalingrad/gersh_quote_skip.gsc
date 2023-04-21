GershQuoteSkipPractice()
{
	level flag::wait_till("initial_blackscreen_passed");
	if(level.script != "zm_stalingrad")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Stalingrad");
        return;
    }
	thread WriteToScreen("Gersh Quote Skip Practice Starting");
	self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(level.w_raygun_mark3_upgraded, 1);
    self zm_weapons::weapon_give(GetWeapon("lmg_cqb"));
    self zm_weapons::weapon_give(GetWeapon("sniper_fastsemi"));
    self thread GiveGauntlet();
    self thread GiveDragonStrike();
    self GiveRiotshield(0);
    self SetOrigin((-275.725, 4768.26, 144.125));
    self SetPlayerAngles((5.32288, -179.615, 0));
	level flag::set("story_playing");
    IPrintLnBold("Wait...");
	wait 10;
    level flag::clear("story_playing");
	level notify("ee_cylinder_acquired");
    level.var_a090a655 HidePart("button_green_jnt");
	level.var_a090a655 ShowPart("button_red_jnt");
	IPrintLnBold("Melee To Start Gersh");
	while(!self MeleeButtonPressed()) wait 0.05;
	[[ @zm_stalingrad_ee_main<scripts\zm\zm_stalingrad_ee_main.gsc>::function_21284834 ]]();
}