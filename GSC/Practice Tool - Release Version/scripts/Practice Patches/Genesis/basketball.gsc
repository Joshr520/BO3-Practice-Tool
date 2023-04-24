BasketballPractice()
{
    level flag::wait_till("initial_blackscreen_passed");
    if(level.script != "zm_genesis")
    {
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: Rev");
        return;
    }
    thread WriteToScreen("Basketball Practice Starting");
    self.score = 50000;
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    level flag::set("character_stones_done");
    level flag::set("phased_sophia_start");
    level flag::set("boss_rush");
    level flag::clear("spawn_zombies");
    self thread GiveAllPerks();
    self zm_weapons::weapon_give(GetWeapon("ar_cqb"));
    self thread Godmode(1);

	if(!self IsHost()) return;

    self thread ShowBasketballControls();
    self thread WaypointSave(0xDB);
    self thread WaypointLoad(0xDD);
    self thread CameraPos(0x74);
    self thread CameraToggle(0x75);
    self thread WatchBallReset(0x76);
	self thread BasketballTimer();
	self thread WatchTimerReset(0x77);
	self thread SetPreviousShotLocation(0x78);

    level waittill(#"hash_f81a82d1");
    wait 5;
    level notify("toys_collected");
    self thread WatchToys();
}

BasketballTimer()
{
	for(;;)
	{
		level waittill(#"hash_f81a82d1");
		while(!self.b_teleporting) wait 0.05;
		start_time = GetTime();
		timer = self InitHud();
		timer.label = &"^3Basketball: ";
		timer.x = 15;
		timer SetTimerUp(0);
		msg = level.var_7d7ca0ea util::waittill_any_return("trigger", "reset");
		timer Destroy();
		if(msg == "trigger")
		{
			end_time = GetTime();
			time_hud = self InitHud();
			time_Hud SetText("^3Basketball: ^2" + CalcTime(end_time - start_time));
			level.var_7d7ca0ea waittill("reset");
			time_Hud Destroy();
		}
		if(self flag::get("has_ball"))
		{
			self notify("ball_pass");
			wait 0.5;
		}
		level.ball.visuals[0] clientfield::set("ball_on_ground_fx", 0);
		level.ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_a41df27c ]]();
		level thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_11a85c29 ]](1);
		level thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_b1e065cd ]]();
	}
}

WatchTimerReset(key)
{
	for(;;)
	{
		while(!compiler::waitkey(key)) wait 0.05;
		level.var_7d7ca0ea notify("reset");
	}
}

SetPreviousShotLocation(key)
{
	self thread WatchForKeyThrow();
	for(;;)
	{
		while(!compiler::waitkey(key)) wait 0.05;
		if(IsDefined(self.keywaypoint))
		{
			thread WriteToScreen("Key Throw Position Loaded");
            self SetOrigin(self.keywaypoint[0]);
            self SetPlayerAngles(self.keywaypoint[1]);
		}
		else thread WriteToScreen("Key Not Thrown Yet");
	}
}

WatchForKeyThrow()
{
	for(;;)
	{
		self waittill("weapon_fired", weapon);
		if(IsTrue(weapon == level.ballweapon)) self.keywaypoint = Array(self GetOrigin(), self GetPlayerAngles());
	}
}

WatchBallReset(key)
{
    for(;;)
    {
        while(!compiler::waitkey(key)) wait 0.05;
        level.ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::reset_ball ]](1, self.origin, 1);
    }
}

WatchToys()
{
    ball_prev_origin = undefined;
	for(;;)
	{
		ball = level.ball;
		ball_visuals = ball.visuals[0];
		ball_origin = ball_visuals.origin;
		if(IsDefined(ball.carrier) || IsTrue(ball.isresetting) && ball.isresetting)
		{
			util::wait_network_frame();
			continue;
		}
		else if(IsDefined(ball_prev_origin) && ball_origin != ball_prev_origin && DistanceSquared(ball_origin, ball_prev_origin) < 40000)
		{
			foreach(toy in level.var_6e907685)
			{
				point = PointOnSegmentNearestToPoint(ball_origin, ball_prev_origin, toy.origin);
				distance = point - toy.origin;
				n_length = Length(distance);
				if(n_length < toy.radius)
				{
                    IPrintLnBold("Toy Hit");
					model = util::spawn_model(toy.model, toy.origin, toy.angles);
					model SetScale(toy.script_float);
					model NotSolid();
					switch(toy.script_string)
					{
						case "up":
						{
							vector = VectorScale((0, 0, 1), 160);
							break;
						}
						case "forward":
						{
							vector = (VectorNormalize(ball.lastcarrier.origin - toy.origin)) * 160;
							break;
						}
						default:
						{
							break;
						}
					}
					model thread [[ @zm_genesis_ee_quest<scripts\zm\zm_genesis_ee_quest.gsc>::function_e2a94206 ]](vector);
					ball thread [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_b8faebaf ]](5);
					PlaySoundAtPosition("zmb_gen_ee_toy_found", toy.origin);
					wait(4.8);
					level thread [[ @zm_genesis_vo<scripts\zm\zm_genesis_vo.gsc>::function_e644549c ]](ball.lastcarrier);
					toy Notify("toy_found");
					if(level.var_6e907685.size)
					{
						if(toy.target == "ee_grand_tour_origins")
						{
							ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::reset_ball ]](1, ball.lastcarrier.origin, 1);
						}
						else
						{
							distance = Distance2D(ball.lastcarrier.origin, toy.origin);
							distance_mult = distance * 0.3;
							n_z_diff = Abs(ball.lastcarrier.origin[2] - toy.origin[2]);
							distance_mult = distance_mult + (n_z_diff * 0.5);
							distance_mult = distance_mult + 150;
							v_force = (ball.lastcarrier.origin + (0, 0, distance_mult)) - ball_origin;
							ball thread [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_fed77788 ]](ball_origin, v_force);
						}
                        wait 1;
						continue;
					}
					ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_a41df27c ]]();
				}
			}
		}
		ball_prev_origin = ball_origin;
		util::wait_network_frame();
	}
}

ShowBasketballControls()
{
    controls = hud::createfontstring("big", 1.25);
    controls hud::setpoint("TOPLEFT", "TOPLEFT", 10, 90);
    controls SetText("Controls List\nHide Controls: F3\nSet Camera Pos: F5\nToggle Camera: F6\nReset Ball: F7\nReset Timer: F8\nLast Key Throw Position: F9\nSave Waypoint: [\nLoad Waypoint: ]");
    for(;;)
    {
        controls.alpha = 1;
        WaitF3();
        controls.alpha = 0;
        WaitF3();
    }
}