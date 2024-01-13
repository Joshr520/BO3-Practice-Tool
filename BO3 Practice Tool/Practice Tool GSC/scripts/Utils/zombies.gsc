KillAllZombies()
{
    foreach (zombie in GetAITeamArray(level.zombie_team)) {
        zombie DoDamage(zombie.health + 666, zombie.origin);
    }
}