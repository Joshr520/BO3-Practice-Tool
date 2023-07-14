IslandSoftPatch()
{
    level flag::set("island_soft_patch");
    WaitFadeIn();
    if(level.script != "zm_island")
    {
        level flag::wait_till("initial_blackscreen_passed");
        thread WriteToScreen("Wrong Map For Loaded Patch - Correct Map: ZNS");
        return;
    }
    self.bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
 	level.players[0].bgb_pack = Array("zm_bgb_perkaholic", "zm_bgb_reign_drops", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_anywhere_but_here", "zm_bgb_extra_credit");
    self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    while((self.bgb_pack_randomized[4] != "zm_bgb_anywhere_but_here" && self.bgb_pack_randomized[5] != "zm_bgb_anywhere_but_here") || (self.bgb_pack_randomized[0] != "zm_bgb_reign_drops" && self.bgb_pack_randomized[0] != "zm_bgb_extra_credit")) self.bgb_pack_randomized = array::randomize(self.bgb_pack);
    level flag::wait_till("initial_blackscreen_passed");
    thread WriteToScreen("ZNS Soft Patch Loaded");
    thread GuaranteeBullet();
    if (!GetDvarInt("zns_soft_patch_spider_round", 0))
    {
        SetDvar("zns_soft_patch_spider_round", 5);
    }
    level.n_next_spider_round = GetDvarInt("zns_soft_patch_spider_round", 5);
    message = hud::createfontstring("big", 1.25);
    message hud::setpoint("TOPLEFT", "TOPLEFT", 20, 15);
    message SetText("Spider Round: " + level.n_next_spider_round + "\nPress + To Change");
    thread ChangeSpiderRound(message);
    wait 10;
    message Destroy();
}

GuaranteeBullet()
{
    level.var_df105f37 = 100;
}

ChangeSpiderRound(message)
{
    start_time = GetTime();
    for(;;)
    {
        WaitPlus();
        if(GetTime() - start_time >= 10000)
        {
            return;
        }
        level.n_next_spider_round++;
        if(level.n_next_spider_round > 7)
        {
            level.n_next_spider_round = 5;
        }
        SetDvar("zns_soft_patch_spider_round", level.n_next_spider_round);
        message SetText("Spider Round: " + level.n_next_spider_round + "\nPress + To Change");
    }
}

detour zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::main()
{
	level flag::init("flag_init_challenge_pillars");
	level thread [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenge_pillars ]]();
	if(getdvarint("splitscreen_playerCount") > 2)
	{
		array::run_all(getentarray("t_lookat_challenge_1", "targetname"), ::delete);
		array::run_all(getentarray("t_lookat_challenge_2", "targetname"), ::delete);
		array::run_all(getentarray("t_lookat_challenge_3", "targetname"), ::delete);
		array::thread_all(struct::get_array("s_challenge_trigger"), struct::delete);
		struct::get("s_challenge_altar") struct::delete();
	}
	else
	{
		level._challenges = spawnstruct();
		level._challenges.challenge_1 = [];
		level._challenges.challenge_2 = [];
		level._challenges.challenge_3 = [];
		array::add(level._challenges.challenge_1, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](1, &"ZM_ISLAND_CHALLENGE_1_1", 1, "update_challenge_1_1", undefined));
		array::add(level._challenges.challenge_1, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](1, &"ZM_ISLAND_CHALLENGE_1_2", 1, "update_challenge_1_2", undefined));
		array::add(level._challenges.challenge_1, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](1, &"ZM_ISLAND_CHALLENGE_1_3", 5, "update_challenge_1_3", undefined));
		array::add(level._challenges.challenge_1, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](1, &"ZM_ISLAND_CHALLENGE_1_4", 5, "update_challenge_1_4", undefined));
		array::add(level._challenges.challenge_1, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](1, &"ZM_ISLAND_CHALLENGE_1_5", 5, "update_challenge_1_5", @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_2dbc7cd3));
		array::add(level._challenges.challenge_2, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](2, &"ZM_ISLAND_CHALLENGE_2_1", 1, "update_challenge_2_1", undefined));
		array::add(level._challenges.challenge_2, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](2, &"ZM_ISLAND_CHALLENGE_2_4", 10, "update_challenge_2_4", undefined));
		array::add(level._challenges.challenge_2, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](2, &"ZM_ISLAND_CHALLENGE_2_5", 20, "update_challenge_2_5", undefined));
        array::add(level._challenges.challenge_3, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](3, &"ZM_ISLAND_CHALLENGE_3_1", 8, "update_challenge_3_1", undefined));
		array::add(level._challenges.challenge_3, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](3, &"ZM_ISLAND_CHALLENGE_3_2", 3, "update_challenge_3_2", undefined));
		array::add(level._challenges.challenge_3, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](3, &"ZM_ISLAND_CHALLENGE_3_4", 30, "update_challenge_3_4", undefined));
		array::add(level._challenges.challenge_3, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](3, &"ZM_ISLAND_CHALLENGE_3_5", 5, "update_challenge_3_5", @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_26c58398));
		if(!level flag::get("island_soft_patch"))
        {
            array::add(level._challenges.challenge_2, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](2, &"ZM_ISLAND_CHALLENGE_2_2", 1, "update_challenge_2_2", @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_25c1bab7));
            array::add(level._challenges.challenge_2, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](2, &"ZM_ISLAND_CHALLENGE_2_3", 15, "update_challenge_2_3", undefined));
            array::add(level._challenges.challenge_2, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](2, &"ZM_ISLAND_CHALLENGE_2_6", 20, "update_challenge_2_6", undefined));
            array::add(level._challenges.challenge_3, [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::init_challenges ]](3, &"ZM_ISLAND_CHALLENGE_3_3", 1, "update_challenge_3_3", @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_5a96677a));
        }
        zm_spawner::register_zombie_death_event_callback(@zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_905d9544);
		zm_spawner::register_zombie_death_event_callback(@zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_682e6fc4);
		zm_spawner::register_zombie_death_event_callback(@zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_5a2a9ef9);
		zm_spawner::register_zombie_death_event_callback(@zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::function_fe94c179);
		level thread [[ @zm_island_challenges<scripts\zm\zm_island_challenges.gsc>::all_challenges_completed ]]();
		level flag::set("flag_init_player_challenges");
	}
}

// blue = 1, green = 2, purple = 3, rainbow = 4
detour zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_26651461(watered_with, plant_ww_hits, force_plant, golden_bucket)
{
	plant_odds = [];
	plant_odds[0] = 0;
	plant_odds[1] = 0;
	plant_odds[2] = 0;
	plant_odds[3] = 0;
	plant_odds[4] = 0;
	plant_odds[5] = 0;
	plant_odds[6] = 0;
	plant_odds[7] = 0;
	plant_odds[8] = 0;
	plant_odds[9] = 0;
	plant_odds[10] = 0;
	watered_with_1 = 0;
	watered_with_2 = 0;
	watered_with_3 = 0;
	watered_with_4 = 0;
	if(self.script_noteworthy === "ee_planting_spot")
	{
		b_ee_planting_spot = 1;
	}
	else
	{
		b_ee_planting_spot = 0;
	}
	for(i = 0; i < watered_with.size; i++)
	{
		if(watered_with[i] == 4)
		{
			if(b_ee_planting_spot && !level flag::get("ww_upgrade_spawned_from_plant"))
			{
				continue;
			}
			watered_with[i] = randomintrange(1, 4);
		}
	}
	foreach(plant_num in watered_with)
	{
		if(plant_num == 1)
		{
			watered_with_1++;
			continue;
		}
		if(plant_num == 2)
		{
			watered_with_2++;
			continue;
		}
		if(plant_num == 3)
		{
			watered_with_3++;
			continue;
		}
		if(plant_num == 4)
		{
			watered_with_4++;
		}
	}
	if(self.script_noteworthy === "ee_planting_spot")
	{
		self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_5726c670 ]](watered_with_4);
		return;
	}
	n_total = 0;
	if(level.var_50d5cc84 < 4)
	{
		if(plant_ww_hits == 3)
		{
			plant_odds[10] = 20;
		}
		else
		{
			plant_odds[10] = 5 * plant_ww_hits;
		}
	}
	ww_odds_mult = 100 - plant_odds[10];
	n_total = n_total + plant_odds[10];
	if(watered_with_1 == 1 && watered_with_2 == 1 && watered_with_3 == 1)
	{
		plant_odds[8] = ww_odds_mult / 2;
		plant_odds[2] = (ww_odds_mult / 2) / 3;
		plant_odds[4] = (ww_odds_mult / 2) / 3;
		plant_odds[6] = (ww_odds_mult / 2) / 3;
		plant_odds[0] = 0;
		plant_odds[1] = 0;
	}
	else
	{
		plant_odds[2] = ww_odds_mult * (watered_with_1 / 3);
		plant_odds[4] = ww_odds_mult * (watered_with_3 / 3);
		plant_odds[6] = ww_odds_mult * (watered_with_2 / 3);
		n_total = n_total + ((plant_odds[2] + plant_odds[4]) + plant_odds[6]);
		if(watered_with.size === 0 && plant_ww_hits === 0)
		{
			plant_odds[0] = 100 - n_total;
			plant_odds[1] = 0;
		}
		else
		{
			plant_odds[0] = 0;
			plant_odds[1] = 100 - n_total;
		}
	}
	plant_odds[3] = plant_odds[2] * (plant_ww_hits / 3);
	plant_odds[2] = abs(plant_odds[2] - plant_odds[3]);
	plant_odds[5] = plant_odds[4] * (plant_ww_hits / 3);
	plant_odds[4] = abs(plant_odds[4] - plant_odds[5]);
	plant_odds[7] = plant_odds[6] * (plant_ww_hits / 3);
	plant_odds[6] = abs(plant_odds[6] - plant_odds[7]);
	plant_odds[9] = plant_odds[8] * (plant_ww_hits / 3);
	plant_odds[8] = abs(plant_odds[8] - plant_odds[9]);

	if(IsTrue(golden_bucket))
	{
		plant_odds = [];
		if(plant_ww_hits > 0)
		{
			plant_odds[5] = 100;
		}
		else
		{
			plant_odds[4] = 100;
		}
	}
	plant_scores = [];
	accum_score = 0;
	foreach(plant, n_score in plant_odds)
	{
		if(n_score > 0)
		{
			plant_scores[plant] = accum_score + n_score;
			accum_score = accum_score + n_score;
		}
	}
	n_random = randomfloatrange(0, 100);
	foreach(plant, n_score in plant_scores)
	{
		if(n_random <= n_score)
		{
			break;
		}
	}
	if(IsDefined(force_plant))
	{
		plant = force_plant;
	}
	if(level flag::get("island_soft_patch") && watered_with_1 > 0)
	{
		plant = 3;
	}
	switch(plant)
	{
		case 0:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_41663231 ]]();
			break;
		}
		case 1:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_41663231 ]](1);
			break;
		}
		case 2:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_a6084535 ]](0, watered_with_4);
			break;
		}
		case 3:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_a6084535 ]](1, watered_with_4);
			break;
		}
		case 4:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_5d62716 ]](0, golden_bucket);
			break;
		}
		case 5:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_5d62716 ]](1, golden_bucket);
			break;
		}
		case 6:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_12c8548e ]]();
			break;
		}
		case 7:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_12c8548e ]](1);
			break;
		}
		case 8:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_fd098f17 ]]();
			break;
		}
		case 9:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_fd098f17 ]](1);
			break;
		}
		case 10:
		{
			self [[ @zm_island_planting<scripts\zm\zm_island_planting.gsc>::function_3e429652 ]]();
			break;
		}
		default:
		{
			break;
		}
	}
}