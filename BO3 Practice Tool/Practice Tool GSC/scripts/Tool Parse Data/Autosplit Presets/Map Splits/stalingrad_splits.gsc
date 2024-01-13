SplitPAPGroph(split_name)
{
    switch (split_name) {
        case "Groph 1": {
            piecename = "part_transmitter";
            break;
        }
        case "Groph 2": {
            piecename = "part_codes";
            break;
        }
        case "Groph 3": {
            piecename = "part_map";
            break;
        }
        default: {
            return;
        }
    }

    level flag::wait_till("dragonride_" + piecename + "_found");
}

// tank = 1, supply = 2, dc = 3
SplitFly(split_name)
{
    switch (split_name) {
        case "Fly Tank": {
            num = 1;
            break;
        }
        case "Fly Supply": {
            num = 2;
            break;
        }
        case "Fly DC": {
            num = 3;
            break;
        }
        default: {
            return;
        }
    }
    while (!level IsScenePlaying("cin_t7_ai_zm_dlc3_dragon_transport_roost" + num + "_idle_2_pavlovs")) {
        wait 0.05;
    }
}

SplitPickupEgg(split_name)
{
    level flag::wait_till("dragon_egg_acquired");
}

SplitEggBathed(split_name)
{
    level flag::wait_till("egg_bathed_in_flame");
}

SplitIncubationStart(split_name)
{
    level flag::wait_till("egg_placed_incubator");
}

SplitGauntletPickup(split_name)
{
    level flag::wait_till("dragon_gauntlet_acquired");
}

SplitPAPLockdown(split_name)
{
    level flag::wait_till("dragon_strike_unlocked");
    level flag::wait_till("lockdown_active");
    if (split_name == "Lockdown Start") {
        return;
    }
	level flag::wait_till("lockdown_complete");
}

SplitChallenges(split_name)
{
    level flag::wait_till("scenario_active");
}

SplitDownload(split_name)
{
    level waittill(#"hash_8cc49f44");
}

SplitStalingradBoss(split_name)
{
    GetEnt("ee_sewer_to_arena_trig", "targetname") waittill("trigger", player);
}

SplitStalingradEnd()
{
    level waittill(#"hash_9b1cee4c");
}