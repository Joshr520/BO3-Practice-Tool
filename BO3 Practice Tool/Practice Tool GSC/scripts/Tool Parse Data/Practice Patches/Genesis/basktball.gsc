BasketballPractice()
{
    if (level.mapname != "zm_genesis") {
        DebugPrint("error", "Basketball Practice attempted to load on " + level.mapDisplayName + " but it requires Revelations");
        return;
    }
	RegisterHotkey(0x74, "Reset Ball: ^9F5", undefined, ::BallReset);
	RegisterHotKey(0x75, "Toggle Ball Camera: ^9F6\n^3Camera: ", "Active", ::ToggleProjectileCamera, ::ProjectileActiveText);
	RegisterHotKey(0x76, "View Impact Point: ^9F7", undefined, ::ViewImpactPoint, undefined, level.players[0]);
	RegisterHotkey(0x77, "Load Last Thrown Position: ^9F8", undefined, ::RestorePreviousShotLocation);
	RegisterHotkey(0x78, "Reset Timer: ^9F9", undefined, ::ResetBasketballTimer, undefined, level.players[0]);
    level.projectile_camera = true;
    level.display_messages = false;
	level flag::init("camera_viewing_projectile");
    level flag::init("camera_viewing_impact");
	level flag::init("ball_in_use");
	level flag::set("basketball_practice");

    WaitFadeIn();
    level.players[0] thread OpenAllDoors();
    level.players[0] thread ActivateAllPower();
    foreach (player in GetPlayers()) {
        player.score = 50000;
        player InfiniteAmmo(true);
        player GiveAllPerks();
		player thread WatchBallThrow();
    }
	level.players[0] thread BasketballTimer();

    level thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_11a85c29 ]](1);
    level thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_b1e065cd ]]();
    thread WatchToys();

    level flag::wait_till("initial_blackscreen_passed");
    level.display_messages = true;
    thread WriteToScreen("Basketball Practice Loaded");
    level flag::clear("spawn_zombies");
}

WatchToys()
{
    toys = [];
	for(i = 0; i < 7; i++) {
		toy = struct::get("ee_grand_tour_toy_0" + i, "targetname");
		toys[toys.size] = toy;
		model = GetEnt(toy.target, "targetname");
		model NotSolid();
	}

    ball_prev_origin = undefined;
	for(;;) {
		ball = level.ball;
        if (!IsDefined(ball)) {
            wait 0.05;
            continue;
        }
		ball_visuals = ball.visuals[0];
		ball_origin = ball_visuals.origin;
		if(IsDefined(ball.carrier) || IsTrue(ball.isresetting)) {
			util::wait_network_frame();
			continue;
		}
		else if(IsDefined(ball_prev_origin) && ball_origin != ball_prev_origin && DistanceSquared(ball_origin, ball_prev_origin) < 40000) {
			foreach(toy in toys) {
				point = PointOnSegmentNearestToPoint(ball_origin, ball_prev_origin, toy.origin);
				distance = point - toy.origin;
				n_length = Length(distance);
				if(n_length < toy.radius) {
                    IPrintLnBold("Toy Hit");
					level flag::set("ball_in_use");
					level.last_ball_carrier.projectile_impact_origin = level.ball.visuals[0].origin;
					model = util::spawn_model(toy.model, toy.origin, toy.angles);
					model SetScale(toy.script_float);
					model NotSolid();
					switch(toy.script_string) {
						case "up": {
							vector = VectorScale((0, 0, 1), 160);
							break;
						}
						case "forward": {
							vector = (VectorNormalize(ball.lastcarrier.origin - toy.origin)) * 160;
							break;
						}
						default: {
							break;
						}
					}
					model thread [[ @zm_genesis_ee_quest<scripts\zm\zm_genesis_ee_quest.gsc>::function_e2a94206 ]](vector);
					ball thread [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_b8faebaf ]](5);
					PlaySoundAtPosition("zmb_gen_ee_toy_found", toy.origin);
					wait(4.8);
					level thread [[ @zm_genesis_vo<scripts\zm\zm_genesis_vo.gsc>::function_e644549c ]](ball.lastcarrier);
					toy Notify("toy_found");
					if(toys.size) {
						if(toy.target == "ee_grand_tour_origins") {
							ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::reset_ball ]](1, ball.lastcarrier.origin, 1);
							thread WaitBallReset();
						}
						else {
							distance = Distance2D(ball.lastcarrier.origin, toy.origin);
							distance_mult = distance * 0.3;
							n_z_diff = Abs(ball.lastcarrier.origin[2] - toy.origin[2]);
							distance_mult = distance_mult + (n_z_diff * 0.5);
							distance_mult = distance_mult + 150;
							v_force = (ball.lastcarrier.origin + (0, 0, distance_mult)) - ball_origin;
							ball thread [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_fed77788 ]](ball_origin, v_force);
							level flag::clear("ball_in_use");
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

WaitBallReset()
{
	level.ball waittill("reset");
	level flag::clear("ball_in_use");
}

BallReset()
{
	if (level flag::get("ball_in_use")) {
		thread WriteToScreen("Ball currently in use");
		return;
	}
	level flag::set("ball_in_use");
	level.ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::reset_ball ]](1, self.origin, 1);
	level flag::clear("ball_in_use");
}

WatchBallThrow()
{
	for (;;) {
		self waittill("weapon_fired", weapon);
		if(weapon != GetWeapon("ball")) {
			continue;
		}
		level.last_ball_carrier = self;
		self.keywaypoint = Array(self GetOrigin(), self GetPlayerAngles());
		if(IsDefined(level.ball)) {
			visual = level.ball.visuals[0];
			if (!IsDefined(visual)) {
				IPrintLnBold("no visuals");
				return;
			}

			self.projectile_impact_origin = undefined;
			self thread WaitBallPathFinished();
			self thread UpdateBallImpactOnReset();
			if (level.projectile_camera) {
				self FollowProjectile(visual, "ball_camera_ended", "end_ball_camera", "path_ended", false);
				self notify("ball_camera_ended");
			}
		}
	}
}

WaitBallPathFinished()
{
	level endon("ball_camera_ended");
	level.ball endon("reset");
	while (!level.ball.visuals[0] clientfield::get("ball_on_ground_fx")) {
		wait 0.05;
	}
	self.projectile_impact_origin = level.ball.visuals[0].origin;
	level.ball.visuals[0] notify("path_ended");
}

UpdateBallImpactOnReset()
{
	level.ball waittill("reset");
	if (!IsDefined(self.projectile_impact_origin)) {
		self.projectile_impact_origin = level.ball.prev_valid_origin;
		level.ball.visuals[0] notify("path_ended");
	}
}

RestorePreviousShotLocation()
{
	if(IsDefined(self.keywaypoint)) {
		thread WriteToScreen("Key Throw Position Loaded");
		self SetOrigin(self.keywaypoint[0]);
		self SetPlayerAngles(self.keywaypoint[1]);
	}
	else {
		thread WriteToScreen("Key Not Thrown Yet");
	}
}

BasketballTimer()
{
	for(;;) {
		level waittill(#"hash_f81a82d1");
		while(!self.b_teleporting) {
			wait 0.05;
		}
		start_time = GetTime();
		timer = hud::createservertimer("default", 1.5);
		timer.label = &"^3Basketball: ";
		timer.x = 15;
		timer SetTenthsTimerUp(0.05);
		msg = level.var_7d7ca0ea util::waittill_any_return("trigger", "reset");
		if(msg == "trigger") {
			end_time = GetTime();
			timer SetText("^2" + CalcTime(end_time - start_time));
			level.var_7d7ca0ea waittill("reset");
			timer Destroy();
		}
		if(self flag::get("has_ball")) {
			self notify("ball_pass");
			wait 0.5;
		}
		level.ball.visuals[0] clientfield::set("ball_on_ground_fx", 0);
		level.ball [[ @ball<scripts\zm\_zm_weap_ball.gsc>::function_a41df27c ]]();
		level thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_11a85c29 ]](1);
		level thread [[ @zm_genesis_arena<scripts\zm\zm_genesis_arena.gsc>::function_b1e065cd ]]();
	}
}

ResetBasketballTimer()
{
	level.var_7d7ca0ea notify("reset");
}

detour ball<scripts\zm\_zm_weap_ball.gsc>::ball_return_home(var_fd894ecd, var_6f3d4b2e)
{
	if (!level flag::get("basketball_practice")) {
		return self [[ @ball<scripts\zm\_zm_weap_ball.gsc>::ball_return_home ]](var_fd894ecd, var_6f3d4b2e);
	}

	self.isresetting = 1;
	prev_origin = self.trigger.origin;
	self.prev_valid_origin = prev_origin;
	self notify("reset");
	self gameobjects::move_visuals_to_base();
	self.trigger.origin = self.trigger.baseorigin;
	self.curorigin = self.trigger.origin;
	if(isdefined(self.onreset)) {
		self [[self.onreset]](prev_origin, var_fd894ecd, var_6f3d4b2e);
	}
	self gameobjects::clear_carrier();
	gameobjects::update_world_icons();
	gameobjects::update_compass_icons();
	self.isresetting = 0;
}