FinishRitual(num)
{
    if(!level flag::exists("ritual_" + num + "_force_started"))
    {
        level flag::init("ritual_" + num + "_force_started");
        level.force_rituals = 0;
    }
    if(level flag::get("ritual_" + num + "_force_started")) return;
    level flag::set("ritual_" + num + "_force_started");
    switch(num)
    {
        case 0:
            name = "magician";
            break;
        case 1:
            name = "femme";
            break;
        case 2:
            name = "detective";
            break;
        case 3:
            name = "boxer";
            break;
        case 4:
            name = "pap";
            break;
        default:
            thread WriteToScreen("Incorrect Ritual Input - Must Be 0-4");
            return;
    }
    if(name == "pap")
    {
        level flag::set("pap_door_open");
        self thread FinishRitual(0);
        self thread FinishRitual(1);
        self thread FinishRitual(2);
        self thread FinishRitual(3);
        while(level.force_rituals < 4) wait 0.05;
        for(i = 1; i < 5; i++)
        {
            self BuildAndActivateTrigger(level.var_f86952c7["pap_basin_" + i]);
        }
        wait 5;
    }

    if(name != "pap") ActivateMemento(level.zombie_include_craftables["ritual_" + name].a_piecestubs[0], "memento_" + name);

    thread DisableMargwa();
    
    level.var_9bc9c61f = name;
    level.a_o_defend_areas[name] [[ @zm_zod_quest<scripts\zm\zm_zod_quest.gsc>::ritual_succeed ]](name);
    if(name != "pap")
    {
        self thread zm_craftables::player_get_craftable_piece("ritual_pap", "relic_" + name);
        level clientfield::set("quest_state_" + name, 4);
    }

    if(level.n_zod_rituals_completed >= 4) level flag::set("pap_door_open");

    foreach(trig in level.a_uts_craftables)
    {
        if(trig.targetname == "quest_ritual_usetrigger_" + name)
        {
            trig zm_craftables::craftablestub_remove();
            zm_unitrigger::unregister_unitrigger(trig);
            break;
        }
    }

    level.force_rituals++;
}

DisableMargwa()
{
    level.var_6e63e659++;
    wait 80;
    level.var_6e63e659--;
}