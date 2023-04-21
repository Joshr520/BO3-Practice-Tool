PickupMachineTools()
{
    if(!level.craftables_picked_up) thread WriteToScreen("Picking Up PAP Parts");
    parts = Array("valve1_found", "valve2_found", "valve3_found");
    foreach(part in parts)
    {
        self notify("player_got_valve_part");
        level flag::set(part);
    }
    foreach(player in GetPlayers())
	{
        player clientfield::set_to_player("valveone_part_lever", 1);
		player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.valvetwo_part_lever", "zmInventory.widget_machinetools_parts", 0);
        player clientfield::set_to_player("valvetwo_part_lever", 1);
		player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.valvethree_part_lever", "zmInventory.widget_machinetools_parts", 0);
        player clientfield::set_to_player("valvethree_part_lever", 1);
        player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.valveone_part_lever", "zmInventory.widget_machinetools_parts", 0);
	}
}

PickupMachinePart(found)
{
    parts = Array("valve1_found", "valve2_found", "valve3_found");
    if(IsInArray(parts, found))
    {
        self notify("player_got_valve_part");
        level flag::set(found);
    }
    else
    {
        thread WriteToScreen("Incorrect PAP Part Given");
        return;
    }
    thread WriteToScreen("Picking Up PAP Part " + GetSubStr(found, 5, 6));
    switch(found)
    {
        case "valve1_found":
            foreach(player in GetPlayers())
            {
                player clientfield::set_to_player("valveone_part_lever", 1);
                player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.valvetwo_part_lever", "zmInventory.widget_machinetools_parts", 0);
            }
            break;
        case "valve2_found":
            foreach(player in GetPlayers())
            {
                player clientfield::set_to_player("valvetwo_part_lever", 1);
		        player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.valvethree_part_lever", "zmInventory.widget_machinetools_parts", 0);
            }
            break;
        case "valve3_found":
            foreach(player in GetPlayers())
            {
                player clientfield::set_to_player("valvethree_part_lever", 1);
                player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.valveone_part_lever", "zmInventory.widget_machinetools_parts", 0);
            }
            break;
    }
}