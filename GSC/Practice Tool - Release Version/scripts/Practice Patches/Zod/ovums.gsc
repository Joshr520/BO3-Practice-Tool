OvumsPractice()
{
    WaitFadeIn();
    if(level.script != "zm_zod")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: SOE");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Starting Ovums Practice");
    wait 3;
    Timescale(10);
    self thread Godmode(1);

    self thread FinishRitual(0);
    self thread FinishRitual(1);
    self thread FinishRitual(2);
    self thread FinishRitual(3);

    self thread FillAllEggs();

    self thread FixSwordPickups();
    self thread ZombieCounter();

    doors = GetEntArray("zombie_door", "targetname");
    order = Array(1, 4, 5, 6, 7, 8, 9, 10);
    foreach(item in order)
    {
        doors[item] thread zm_blockers::door_opened(doors[item].zombie_cost, 0);
    }
    doors = GetEntArray("zombie_debris", "targetname");
    order = Array(4);
    bgb = self.bgb;
	self.bgb = "zm_bgb_shopping_free";
    foreach(item in order)
    {
        doors[item] notify("trigger", self, 1);
    }
    wait 0.1;
	self.bgb = bgb;
    self thread ActivateAllPower(Array((2471, -3811, 306), (4567, -4666, 306), (1922, -4784, -332), (857, -3728, 501), (1191, -5509, 178), (3026, -4458, 180), (2637, -5649, -102), (4712, -4224, 452), (2861, -6897, 306)));
    self thread OpenBeastDoors();

    self GiveZodCharacter();
    level.givecustomcharacters = ::GiveZodCharacter;
    self GiveAllPerks();
    self.score = 1500;
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self zm_weapons::weapon_give(GetWeapon("pistol_revolver38"));
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self zm_weapons::weapon_give(GetWeapon("shotgun_pump_upgraded"), 1);
    level.next_wasp_round = 6;
    self SetOrigin((2620.69, -3000.64, -391.875));
    self SetPlayerAngles((4.13086, -89.7308, 0));
    GetEnt("keeper_subway_welcome", "targetname") notify("trigger", self);
    level.var_bf361dc0 = RandomIntRange(10, 12);

    self thread SkipToRound(5, 0);

    level waittill("start_of_round");
    wait 2;

    margwa = thread [[ @zm_zod_margwa<scripts\zm\zm_zod_margwa.gsc>::function_8bcb72e9 ]](1);
    wait 3;
    headfunc = margwa.destroyheadcb;
    margwa.destroyheadcb = undefined;
    partName = GetPartName(margwa.model, 135);
    modelHit = margwa MargwaHeadHit(margwa, partName);
    margwa [[ @margwaserverutils<scripts\shared\ai\margwa.gsc>::margwakillhead ]](modelHit, self);
    partName = GetPartName(margwa.model, 174);
    modelHit = margwa MargwaHeadHit(margwa, partName);
    margwa [[ @margwaserverutils<scripts\shared\ai\margwa.gsc>::margwakillhead ]](modelHit, self);
    wait 2;
    for(i = 1; i < 5; i++)
    {
        self BuildAndActivateTrigger(level.var_f86952c7["pap_basin_" + i]);
    }
    IPrintLnBold("Start PAP");
    Timescale(1);
    self thread Godmode(0);
    wait 3;
    margwa.destroyheadcb = headfunc;

    level thread zm_blockers::open_all_zbarriers();
    self.bgb_pack_randomized = Array("zm_bgb_perkaholic", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here");
}

MargwaHeadHit(entity, partName)
{
	switch(partName)
	{
        case "j_chunk_head_bone_le":
        case "j_jaw_lower_1_le":
            return self.head_left_model;
        case "j_chunk_head_bone":
        case "j_jaw_lower_1":
            return self.head_mid_model;
        case "j_chunk_head_bone_ri":
        case "j_jaw_lower_1_ri":
            return self.head_right_model;
	}
	return undefined;
}