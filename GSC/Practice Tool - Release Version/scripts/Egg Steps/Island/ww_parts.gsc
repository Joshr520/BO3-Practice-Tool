PickupWWParts()
{
    if(!level.craftables_picked_up) thread WriteToScreen("Picking Up WW Parts");
    parts = Array("ww1_found", "ww2_found", "ww3_found", "wwup1_found", "wwup2_found", "wwup3_found");
    foreach(part in parts)
    {
        self notify("player_got_ww_part");
        level flag::set(part);
        if(part == "ww3_found")
        {
            venom = GetEnt("venom_extractor", "targetname");
            wait(0.5);
            venom thread FinalizeWW();
        }
    }
    foreach(player in GetPlayers())
	{
        player clientfield::set_to_player("wonderweapon_part_wwi", 1);
		player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.wonderweapon_part_wwi", "zmInventory.widget_wonderweapon_parts", 0);
        player clientfield::set_to_player("wonderweapon_part_wwii", 1);
		player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.wonderweapon_part_wwii", "zmInventory.widget_wonderweapon_parts", 0);
        player clientfield::set_to_player("wonderweapon_part_wwiii", 1);
		player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.wonderweapon_part_wwiii", "zmInventory.widget_wonderweapon_parts", 0);
	}
}

PickupWWPart(found)
{
    parts = Array("ww1_found", "ww2_found", "ww3_found", "wwup1_found", "wwup2_found", "wwup3_found");
    if(IsInArray(parts, found))
    {
        self notify("player_got_ww_part");
        level flag::set(found);
        if(found == "ww3_found")
        {
            venom = GetEnt("venom_extractor", "targetname");
            wait(0.5);
            venom thread FinalizeWW();
        }
    }
    else
    {
        thread WriteToScreen("Incorrect WW Part Given");
        return;
    }
    if(IsSubStr(found, "up")) thread WriteToScreen("Picking Up Masamune Part " + GetSubStr(found, 4, 5));
    else thread WriteToScreen("Picking Up KT-4 Part " + GetSubStr(found, 2, 3));
    switch(found)
    {
        case "ww1_found":
            foreach(player in GetPlayers())
            {
                player clientfield::set_to_player("wonderweapon_part_wwi", 1);
                player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.wonderweapon_part_wwi", "zmInventory.widget_wonderweapon_parts", 0);
            }
            break;
        case "ww2_found":
            foreach(player in GetPlayers())
            {
                player clientfield::set_to_player("wonderweapon_part_wwii", 1);
		        player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.wonderweapon_part_wwii", "zmInventory.widget_wonderweapon_parts", 0);
            }
            break;
        case "ww3_found":
            foreach(player in GetPlayers())
            {
                player clientfield::set_to_player("wonderweapon_part_wwiii", 1);
		        player thread zm_craftables::player_show_craftable_parts_ui("zmInventory.wonderweapon_part_wwiii", "zmInventory.widget_wonderweapon_parts", 0);
            }
            break;
    }
}

FinalizeWW()
{
	self scene::play("p7_fxanim_zm_island_venom_extractor_end_bundle", self);
	self SetModel("p7_fxanim_zm_island_venom_extractor_red_mod");
	self scene::init("p7_fxanim_zm_island_venom_extractor_red_bundle", self);
}