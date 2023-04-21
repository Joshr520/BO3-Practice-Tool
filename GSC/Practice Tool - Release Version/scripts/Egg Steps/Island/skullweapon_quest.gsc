CompleteSkulls()
{
    level.var_69fe775a = 1;
    level.var_452c59e0 = 3;
    while(level.var_452c59e0 != 4)
    {
        self BuildAndActivateTrigger(level.var_a576e0b9[1].s_utrig_pillar);
        wait 1;
        self BuildAndActivateTrigger(level.var_a576e0b9[1].s_utrig_skulltar);
        wait 4;
        self BuildAndActivateTrigger(level.var_a576e0b9[1].s_utrig_skulltar);
        wait 1;
        self BuildAndActivateTrigger(level.var_a576e0b9[1].s_utrig_pillar);
        wait 2;
    }
}

CompleteSkullRitual()
{
    while(!level flag::exists("skullroom_defend_inprogress")) wait 0.05;
    self BuildAndActivateTrigger(level.var_b2152df5);
    wait 0.5;
    level.var_9bc0cd6e = 50;
    wait 3;
    self BuildAndActivateTrigger(level.var_b2152df5);
}