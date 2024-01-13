ZombieCounter()
{
    level endon("end_game");

    last_counted_total = -1;
    last_counted_current = -1;
    last_counted_respawn = 1;

    total_zombies = hud::createfontstring("big", 1.3);
    total_zombies hud::setpoint("TOPRIGHT", "TOPRIGHT", -25, 5);
    total_zombies.label = &"^1Zombies Remaining: ";

    respawn_zombies = hud::createfontstring("big", 1.3);
    respawn_zombies hud::setpoint("TOPRIGHT", "TOPRIGHT", -25, 20);
    respawn_zombies.label = &"^3Zombies To Respawn: ";

    current_zombies = hud::createfontstring("big", 1.3);
    current_zombies hud::setpoint("TOPRIGHT", "TOPRIGHT", -25, 35);
    current_zombies.label = &"^2Zombies Spawned: ";

    for(;;) {
        if(level.zombie_total + zombie_utility::get_current_zombie_count() != last_counted_total) {
            last_counted_total = level.zombie_total + zombie_utility::get_current_zombie_count();
            total_zombies SetValue(last_counted_total);
        }
        if (level.zombie_respawns != last_counted_respawn) {
            last_counted_respawn = level.zombie_respawns;
            respawn_zombies SetValue(last_counted_respawn);
        }
        if(zombie_utility::get_current_zombie_count() != last_counted_current) {
            last_counted_current = zombie_utility::get_current_zombie_count();
            current_zombies SetValue(last_counted_current);
        }
        wait 0.05;
    }
}