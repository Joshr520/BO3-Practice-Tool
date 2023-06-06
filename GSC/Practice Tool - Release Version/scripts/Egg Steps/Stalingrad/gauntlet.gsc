AcquireEgg()
{
    egg = GetEnt("egg_drop_damage", "targetname");
    if(!IsDefined(egg)) return;
    level flag::set("dragon_pavlov_first_time");
    wait 0.1;
    egg notify("damage");
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_stal_pavlov_boards_bundle", 0, 0, 0);
    wait 0.5;
    pickup = struct::get("dragon_egg_pickup", "targetname").s_unitrigger;
    self BuildAndActivateTrigger(pickup);
}

AwakenEgg()
{
    if(level flag::get("egg_awakened")) return;
    if(!level flag::get("dragon_egg_acquired")) self AcquireEgg();
    wait 1;
    self BuildAndActivateTrigger(level.var_de98e3ce.var_9cd2418f[0].s_unitrigger);
    wait 0.5;
    level flag::set("egg_bathed_in_flame");
    wait 0.5;
    level notify("start_of_round");
    wait 0.5;
	level notify("end_of_round");
    wait 1;
    self BuildAndActivateTrigger(level.var_de98e3ce.var_9cd2418f[0].s_unitrigger);
}

FinishNapalm()
{
    if(level flag::get("gauntlet_step_2_complete")) return;
    if(!level flag::get("egg_cooled_hazard")) self AwakenEgg();
    level flag::wait_till("egg_awakened");
    wait 1;
    level notify(#"hash_68bf9f79");
	level flag::set("gauntlet_step_2_complete");
}

FinishMultiKills()
{
    if(!level flag::get("gauntlet_step_2_complete")) self FinishNapalm();
    if(level flag::get("gauntlet_step_3_complete")) return;
    level flag::wait_till("gauntlet_step_2_complete");
    wait 1;
    level notify(#"hash_b227a45b");
	level flag::set("gauntlet_step_3_complete");
}

FinishMeleeKills()
{
    if(!level flag::get("gauntlet_step_3_complete")) self FinishMultiKills();
    if(level flag::get("gauntlet_step_4_complete")) return;
    level flag::wait_till("gauntlet_step_3_complete");
    wait 1;
    level notify(#"hash_9b46a273");
    level flag::set("gauntlet_step_4_complete");
}

IncubateEgg()
{
    if(!level flag::get("gauntlet_step_4_complete")) self FinishMeleeKills();
    level flag::wait_till("gauntlet_step_4_complete");
    wait 1;
    incubate = struct::get("gauntlet_incubation_start", "script_noteworthy").s_unitrigger;
    self BuildAndActivateTrigger(incubate);
    wait 0.5;
    level.var_de98e3ce.var_a6563820 = 1;
    basement = GetEnt("basement_lockdown_score", "targetname");
    while(!level.var_de98e3ce.var_179b5b71)
    {
        zombies = GetAITeamArray(level.zombie_team);
        foreach(zombie in zombies)
        {
            zombie ForceTeleport(basement.origin);
            zombie DoDamage(zombie.health + 666, self.origin, self);
        }
        wait 0.05;
    }
}

FinishPostIncubate()
{
    if(level flag::get("egg_cooled_incubator")) return;
    while(!IsDefined(level.var_de98e3ce.var_987fcd7a)) wait 0.05;
    wait 1;
    level notify("start_of_round");
    wait 0.5;
	level notify("end_of_round");
    wait 0.5;
    level.var_de98e3ce.var_d54b9ade notify("trigger_activated");
}

PickupGauntlet()
{
    if(!level flag::get("egg_cooled_incubator")) self FinishPostIncubate();
    if(level flag::get("dragon_gauntlet_acquired")) return;
    level flag::wait_till("gauntlet_quest_complete");
    wait 1;
    org = self GetOrigin();
    ang = self GetPlayerAngles();
    challenges = struct::get_array("s_challenge_trigger");
    foreach(challenge in challenges)
    {
        if(challenge.script_int == self GetEntityNumber())
        {
            reward = challenge;
        }
    }
    self SetOrigin((389.356, 373.366, 22.0565));
    self SetPlayerAngles((30.2509, -132.61, 0));
    self FreezeControls(1);
    wait 0.25;
    reward notify("trigger_activated", self);
    wait 2;
    reward notify("trigger_activated", self);
    wait 0.25;
    self SetOrigin(org);
    self SetPlayerAngles(ang);
    self FreezeControls(0);
}