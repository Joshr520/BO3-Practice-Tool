CompleteSkull(num)
{
    if(num == 5)
    {
        self thread CompleteSkullRitual();
        return;
    }
    if(level.var_a576e0b9[num] flag::get("skullquest_completed")) return;
    level.var_a576e0b9[num] flag::set("skullquest_completed");
    self [[ @zm_island_skullquest<scripts\zm\zm_island_skullweapon_quest.gsc>::function_b6c35b21 ]](num);
    level.var_452c59e0++;
    if(level.var_452c59e0 == 4)
    {
        level thread exploder::exploder("fxexp_503");
        wait(0.25);
        level thread [[ @zm_island_skullquest<scripts\zm\zm_island_skullweapon_quest.gsc>::function_fdafeea2 ]](self);
        foreach(skull in level.var_a576e0b9)
        {
            skull.mdl_skull_p clientfield::set("skullquest_finish_done_glow_fx", 1);
        }
    }
    zm_unitrigger::unregister_unitrigger(level.var_a576e0b9[num].s_utrig_pillar);
    zm_unitrigger::unregister_unitrigger(level.var_a576e0b9[num].s_utrig_skulltar);
}

CompleteSkulls()
{
    self thread CompleteSkull(1);
    self thread CompleteSkull(2);
    self thread CompleteSkull(3);
    self thread CompleteSkull(4);
    self thread CompleteSkullRitual();
}

CompleteSkullRitual()
{
    if(!level flag::exists("skullroom_defend_inprogress")) CompleteSkulls();
    while(!level flag::exists("skullroom_defend_inprogress")) wait 0.05;
    self BuildAndActivateTrigger(level.var_b2152df5);
    wait 0.5;
    level.var_9bc0cd6e = 50;
    wait 3;
    self BuildAndActivateTrigger(level.var_b2152df5);
}