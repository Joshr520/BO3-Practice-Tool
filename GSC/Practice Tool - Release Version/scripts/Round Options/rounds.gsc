EndRound(text = 1)
{
    if(level flag::get("round_skip_request"))
    {
        thread WriteToScreen("Round Request Already Queued");
        return;
    }
    level flag::set("round_skip_request");
    while(!level.zombie_total && !zombie_utility::get_round_enemy_Array().size) wait 0.05;
    level.zombie_total = 0;
	zombie_utility::ai_calculate_health(level.round_number + 1);
	level notify("kill_round");
	PlaySoundAtPosition("zmb_bgb_round_robbin", (0, 0, 0));
    if(text) thread WriteToScreen("Ending Current Round");
    level endon("end_of_round");
    thread WaitRound();
    for(;;)
    {
        foreach(zombie in GetAITeamArray(level.zombie_team))
        {
            zombie Kill();
        }
        wait 1;
    }
}

WaitRound()
{
    level waittill("end_of_round");
    level flag::clear("round_skip_request");
}

SkipToRound(round = 0, zombies_left = -1, delay = 0)
{
    if(level flag::get("round_skip_request"))
    {
        thread WriteToScreen("Round Request Already Queued");
        return;
    }
    if(!round) round = level.round_number;
    if(delay) wait delay;
    thread WriteToScreen("Changing To Round " + round);
	zm::set_round_number(round - 1);
    EndRound(0);
    level.zombie_move_speed = round * level.zombie_vars["zombie_move_speed_multiplier"];
    if(zombies_left >= 0)
    {
        level waittill("zombie_total_set");
        level.zombie_total = zombies_left;
    }
}

SetZombieCount(zombies_left)
{
    while(!level.zombie_total && !zombie_utility::get_round_enemy_Array().size) wait 0.05;
    thread WriteToScreen("Setting Zombie Count to: " + zombies_left);
    level.zombie_total = zombies_left;
}