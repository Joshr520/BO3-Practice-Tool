ZombieCounter()
{
    level endon("end_game");

    last_counted_total = -1;
    last_counted_current = -1;
    total_zombies = hud::createfontstring("big", 1.3);
    current_zombies = hud::createfontstring("big", 1.3);
    total_zombies hud::setpoint("TOPRIGHT", "TOPRIGHT", -5, 20);
    current_zombies hud::setpoint("TOPRIGHT", "TOPRIGHT", -9, 35);
    total_zombies.color = (1, 0, 0);
    current_zombies.color = (0, 1, 0);
    thread MonitorAlpha(total_zombies, current_zombies);
    for(;;)
    {
        if(IsTrue(level.custom_spawn_monitor))
        {
            total_zombies Destroy();
            current_zombies Destroy();
            return;
        }
        if(level.zombie_total + zombie_utility::get_current_zombie_count() != last_counted_total)
        {
            if(IsTrue(level.zombie_counter_diagnostic)) compiler::nprintln("Zombies Remaining Changed At " + Int(GetTime() - level.n_gameplay_start_time) + " - " + last_counted_total + " -> " + Int(level.zombie_total + zombie_utility::get_current_zombie_count()));
            last_counted_total = level.zombie_total + zombie_utility::get_current_zombie_count();
            total_zombies SetText("Zombies Remaining: " + last_counted_total);
        }
        if(zombie_utility::get_current_zombie_count() != last_counted_current)
        {
            if(IsTrue(level.zombie_counter_diagnostic)) compiler::nprintln("Zombies Spawned Changed At " + Int(GetTime() - level.n_gameplay_start_time) + " - " + last_counted_current + "/" + last_counted_total + " -> " + zombie_utility::get_current_zombie_count() + "/" + last_counted_total);
            last_counted_current = zombie_utility::get_current_zombie_count();
            current_zombies SetText("Zombies Spawned: " + last_counted_current);
        }
        wait 0.05;
    }
}

MonitorAlpha(hud1, hud2)
{
    level endon("end_game");

    for(;;)
    {
        hud1.alpha = 1;
        hud2.alpha = 1;
        while(!compiler::waitkey(0x72)) wait 0.05;
        hud1.alpha = 0;
        hud2.alpha = 0;
        while(!compiler::waitkey(0x72)) wait 0.05;
    }
}