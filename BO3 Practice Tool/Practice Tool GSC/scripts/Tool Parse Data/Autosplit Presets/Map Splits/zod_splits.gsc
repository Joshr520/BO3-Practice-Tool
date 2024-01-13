SplitRitual(split_name)
{
    name = ToLower(StrTok(split_name, " ")[0]);
    level flag::wait_till("ritual_" + name + "_complete");
}

SplitRiftTP(split_name)
{
    pos = IsSubStr(split_name, "Rift -") ? "bottom" : "top";
    tokens = StrTok(split_name, "-");
    area = (pos == "bottom") ? GetSubStr(tokens[1], 1, tokens[1].size) : GetSubStr(tokens[0], 0, tokens[0].size - 1);
    switch (area) {
        case "Footlight": {
            area = "theater";
            break;
        }
        case "Waterfront": {
            area = "slums";
            break;
        }
        case "Canals": {
            area = "canal";
            break;
        }
        default: {
            return;
        }
    }

    portal = GetEntArray(area + "_portal_" + pos, "script_noteworthy");
    portal_trig = GetEntArrayFromArray(portal, "teleport_trigger", "targetname")[0];
    for (;;)
    {
        portal_trig waittill("trigger", portee);
        if (!level clientfield::get("portal_state_" + area) || !IsPlayer(portee)) {
            continue;
        }
        return;
    }
}

SplitZodEgg(split_name)
{
    switch (StrTok(split_name, " ")[0]) {
        case "Canals": {
            num = 0;
            break;
        }
        case "Footlight": {
            num = 1;
            break;
        }
        case "Waterfront": {
            num = 3;
            break;
        }
        case "Rift": {
            num = 4;
            break;
        }
        default: {
            return;
        }
    }

    while (!IsDefined(level.sword_quest.statues[num].trigger)) {
        wait 0.05;
    }
    for (;;) {
        level.sword_quest.statues[num].trigger waittill("trigger", player);
        if (IsDefined(player.sword_quest.kills[num]) && player.sword_quest.kills[num] >= 12) {
            return;
        }
    }
}

SplitSwordPickup(split_name)
{
    if (split_name == "Pickup Sword") {
        level waittill("sword_pickup_1");
        return;
    }
    level waittill("sword_pickup_2");
}

detour zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::give_sword(n_sword_level, var_74719138 = 0)
{
	level notify("sword_pickup_" + n_sword_level);
    return self [[ @zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::give_sword ]](n_sword_level, var_74719138);
}

SplitOvumStart(split_name)
{
    switch (StrTok(split_name, " ")[0]) {
        case "Junction": {
            level waittill("magic_circle_3");
            break;
        }
        case "Canals": {
            level waittill("magic_circle_1");
            break;
        }
        case "Footlight": {
            level waittill("magic_circle_2");
            break;
        }
        case "Waterfront": {
            level waittill("magic_circle_0");
            break;
        }
        default: {
            return;
        }
    }
}

SplitBook(split_name)
{
    level flag::wait_till("ee_book");
}

SplitFlag(split_name)
{
    num = Int(StrTok(split_name, " ")[1]);
    for (;;) {
        level waittill("flag_picked_up");
        if (level [[ @zm_zod_ee<scripts\zm\zm_zod_ee.gsc>::function_832f1b2a ]]() >= num - 1) {
            return;
        }
    }
}

detour zm_zod_ee<scripts\zm\zm_zod_ee.gsc>::function_f3d23e2c()
{
	for(;;) {
		self waittill("trigger", player);
		if (!player BasicTrigPlayerCheck()) {
            continue;
        }
        level notify("flag_picked_up");
		level [[ @zm_zod_ee<scripts\zm\zm_zod_ee.gsc>::function_e525a12 ]](0);
		level [[ @zm_zod_ee<scripts\zm\zm_zod_ee.gsc>::function_ce4db8c8 ]](player);
		self.stub zm_unitrigger::run_visibility_function_for_all_triggers();
		break;
	}
}

SplitZodEnd()
{
    if (GetPlayers().size < 4) {
        level flag::wait_till("ee_boss_defeated");
        return;
    }
    while (!level IsScenePlaying("cin_zod_vign_summoning_key")) {
        wait 0.05;
    }
}