GotoRound(round)
{
    if (level flag::get("round_skip_request")) {
        thread WriteToScreen("Round Request Already Queued");
        return;
    }
    level flag::set("round_skip_request");
    thread WriteToScreen("Changing To Round " + round);
    while (!level.zombie_total && !zombie_utility::get_round_enemy_Array().size) {
        wait 0.05;
    }
    level.zombie_total = 0;
	zombie_utility::ai_calculate_health(round);
	zm::set_round_number(round - 1);
    level notify("kill_round");
    while (level.zombie_total || zombie_utility::get_round_enemy_Array().size) {
        foreach (zombie in GetAITeamArray(level.zombie_team)) {
            if (!IsTrue(zombie.ignore_round_robbin_death)) {
                zombie DoDamage(zombie.health + 666, zombie.origin);
            }
        }
        wait 1;
    }
    level waittill("between_round_over");
    level flag::clear("round_skip_request");
}

InfiniteRound(enabled)
{
    if (enabled) {
        level.zombie_total_restore = level.zombie_total;
        thread KeepRoundInfinite();
        return;
    }
    level.zombie_total = level.zombie_total_restore;
    level.zombie_total_restore = undefined;
    level notify("stop_infinite_round");
}

KeepRoundInfinite()
{
    level endon("stop_infinite_round");
    for (;;) {
        level.zombie_total = 9999;
        wait(level.zombie_vars["zombie_spawn_delay"]);
    }
}

StartSpecialRound()
{

}

SetZombieCount(zombie_count)
{
    while (!level.zombie_total && !zombie_utility::get_round_enemy_Array().size) {
        wait 0.05;
    }
    thread WriteToScreen("Setting Zombie Count to: " + zombie_count);
    level.zombie_total = zombie_count;
}