TriggerBook()
{
    if(!level flag::exists("trigger_book")) level flag::init("trigger_book");
    if(level flag::get("trigger_book")) return;
    level flag::set("trigger_book");
    self thread PickupUpgradedSword();
    level flag::wait_till("ee_begin");
    wait 4;
    book = GetEnt("ee_book", "targetname");
    self BuildAndActivateTrigger(book.unitrigger_stub);
}

FinishFlag(num)
{
    if(!level flag::get("ee_book")) self thread TriggerBook();
    level flag::wait_till("ee_book");
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
        default:
            thread WriteToScreen("Incorrect Flag Input - Must Be 0-3");
            return;
    }
    
    level notify("ee_keeper_resurrected");
    level clientfield::set("ee_keeper_" + name + "_state", 3);
	level flag::set("ee_keeper_" + name + "_resurrected");
    level.var_f47099f2 = 0;
    level.var_e59fb2 = 2;
}

FinishAllFlags()
{
    self thread FinishFlag(0);
    self thread FinishFlag(1);
    self thread FinishFlag(2);
    self thread FinishFlag(3);
}