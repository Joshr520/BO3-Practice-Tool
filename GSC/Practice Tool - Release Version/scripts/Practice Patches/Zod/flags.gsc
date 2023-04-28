FlagsPractice()
{
    level flag::wait_till("start_zombie_round_logic");
    if(level.script != "zm_zod")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: SOE");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_wall_power");
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("Starting Flags Practice");
    wait 3;
    Timescale(10);
    self thread OpenBeastDoors();
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
    self thread Godmode(1);

    self thread FinishRitual(4);

    self thread FillAllEggs();

    self thread PickupOvum();
    self.sword_quest_2.var_db999762[1] = 1;
    self.sword_quest_2.var_db999762[2] = 1;
    self.sword_quest_2.var_db999762[3] = 1;

    self thread FixSwordPickups();
    self thread ZombieCounter();

    self GiveZodCharacter();
    level.givecustomcharacters = ::GiveZodCharacter;
    self GiveAllPerks();
    self.score = 6000;
    foreach(weapon in self GetWeaponsListPrimaries())
    {
        self TakeWeapon(weapon);
        wait 0.1;
    }
    self zm_weapons::weapon_give(GetWeapon("pistol_revolver38"));
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self zm_weapons::weapon_give(GetWeapon("shotgun_pump_upgraded"), 1);
    self thread GivePlayerGum("zm_bgb_anywhere_but_here");
    if(IsDefined(level.var_2c12d9a6)) level.old_abh_func = level.var_2c12d9a6;
    level.var_2c12d9a6 = ::ZodABHManip;
    level.abh_array = Array(3, 8);
    self.bgb_pack_randomized = Array("zm_bgb_perkaholic", "zm_bgb_wall_power", "zm_bgb_dead_of_nuclear_winter");
    self SetOrigin((5770.28, -3095.37, 512.125));
    self SetPlayerAngles((5.28442, -45.3186, 0));
    GetEnt("keeper_subway_welcome", "targetname") notify("trigger", self);
    level.var_bf361dc0 = RandomIntRange(10, 12);
    while(level clientfield::get("keeper_quest_state_" + self.characterindex) < 2) wait 0.05;
    level.next_wasp_round = 5;
    self thread SkipToRound(5);
    level waittill("start_of_round");
    wait 15;
    self thread SkipToRound(8);
    Timescale(1);
    self notify("bgb_activation_request");
    self thread Godmode(0);

    level thread zm_blockers::open_all_zbarriers();
}