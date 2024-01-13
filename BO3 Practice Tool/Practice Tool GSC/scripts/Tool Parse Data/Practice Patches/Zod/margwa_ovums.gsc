MargwaOvumsPractice()
{
    if (level.mapname != "zm_zod") {
        DebugPrint("error", "Margwa Practice attempted to load on " + level.mapDisplayName + " but it requires Shadows of Evil");
        return;
    }
    level flag::set("margwa_ovums_practice");

    RegisterHotkey(0x74, "Reset Margwas: ^9F5", undefined, ::ResetMargwas);
    RegisterHotkey(0x75, "Change District: ^9F6\n^3District: ", "Junction", ::ChangeMargwaDistrict, ::MargwaDistrictText);
    RegisterHotkey(0x76, "Num Of Margwas: ^9F7\n^3Margwas: ", 1, ::ChangeMargwaAmount, ::MargwaAmountValue);

    level.margwa_district = "Junction";
    level.active_district = "Junction";
    level.margwa_charges = 1;

    foreach (player in GetPlayers()) {
        self flagsys::wait_till("loadout_given");

        foreach (weapon in player GetWeaponsListPrimaries()) {
            player TakeWeapon(weapon);
        }

        player zm_weapons::weapon_give(GetWeapon("ar_cqb"));
        player zm_weapons::weapon_give(GetWeapon("shotgun_pump_upgraded"));
        player waittill("weapon_change_complete");
        player GivePlayerWeapon("keeper_sword", false);
        player GiveAllPerks();
        player thread GivePerksAndRevive();
        player thread KeepQuestState();
    }

    WaitFadeIn();
    level flag::init("magic_circle_in_progress");
    level flag::clear("spawn_zombies");
    level.players[0] thread OpenAllDoors();
    spawner::add_global_spawn_function("axis", ::DestroyHeadCB);

    foreach (circle in struct::get_array("sword_quest_magic_circle_place", "targetname")) {
        [[ @zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::create_magic_circle_unitrigger ]](circle, circle.script_int);
    }

    for (;;) {
        level util::waittill_any("magic_circle_failed", "magic_circle_0_off", "magic_circle_1_off", "magic_circle_2_off", "magic_circle_3_off");

        level flag::clear("magic_circle_in_progress");
        circle = GetCircleByDistrict(level.active_district);
        trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(circle.unitrigger_stub, level.players[0]);
        trig.stub.activated = 0;
        foreach (player in GetPlayers()) {
            player flag::clear("magic_circle_wait_for_round_completed");
            player.sword_quest_2.kills[trig.stub.n_char_index] = 0;
            player.sword_quest_2.var_db999762 = [];
        }
        trig Delete();
        TrueMaxAmmo();
    }
}

// boxer = 0, detective = 1, femme = 2, magician = 3
ResetMargwas()
{
    circle = GetCircleByDistrict(level.active_district);
    trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(circle.unitrigger_stub, level.players[0]);
    trig.stub.player notify("entering_last_stand");
    trig Delete();
    circle = GetCircleByDistrict(level.margwa_district);

    level thread lui::screen_fade_out(1, "black");
    wait 1;

    foreach (player in GetPlayers()) {
        switch (player.characterindex) {
            case 0: {
                player SetOrigin(circle.origin + VectorScale((1, 0, 0), 64));
                break;
            }
            case 1: {
                player SetOrigin(circle.origin + VectorScale((0, 1, 0), 64));
                break;
            }
            case 2: {
                player SetOrigin(circle.origin + VectorScale((-1, 0, 0), 64));
                break;
            }
            case 3: {
                player SetOrigin(circle.origin + VectorScale((0, -1, 0), 64));
                break;
            }
            default: {
                break;
            }
        }
    }
    player SetPlayerAngles(VectortoAngles(circle.origin - player.origin));

    level thread lui::screen_fade_in(2, "black");
}

ChangeMargwaDistrict()
{
    switch (level.margwa_district) {
        case "Junction": {
            level.margwa_district = "Canals";
            break;
        }
        case "Canals": {
            level.margwa_district = "Footlight";
            break;
        }
        case "Footlight": {
            level.margwa_district = "Waterfront";
            break;
        }
        case "Waterfront": {
            level.margwa_district = "Junction";
            break;
        }
        default: {
            level.margwa_district = "Junction";
            break;
        }
    }
}

MargwaDistrictText(hud_elem)
{
    hud_elem SetText(level.margwa_district);
}

ChangeMargwaAmount()
{
    if (level flag::get("magic_circle_in_progress")) {
        return;
    }
    level.margwa_charges = level.margwa_charges == 1 ? 2 : 1;
}

MargwaAmountValue(hud_elem)
{
    hud_elem SetValue(level.margwa_charges);
}

KeepQuestState()
{
    for (;;) {
        level clientfield::set("keeper_quest_state_" + self.characterindex, 2);
        wait 1;
    }
}

DestroyHeadCB()
{
    if (self.archetype != "margwa") {
        return;
    }

    while (!IsDefined(self.destroyheadcb)) {
        wait 0.05;
    }

    self.destroyheadcb = undefined;
}

GetCircleByDistrict(district)
{
    switch (district) {
        case "Junction": {
            num = 3;
            break;
        }
        case "Footlight": {
            num = 2;
            break;
        }
        case "Waterfront": {
            num = 0;
            break;
        }
        case "Canals": {
            num = 1;
            break;
        }
        default: {
            num = 3;
            break;
        }
    }

    foreach (circle in struct::get_array("sword_quest_magic_circle_place", "targetname")) {
        if (circle.script_int == num) {
            return circle;
        }
    }
}

detour zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::function_b7af29e0()
{
	if (IsDefined(level.margwa_charges)) {
        return level.margwa_charges - 1;
    }
	return [[ @zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::function_b7af29e0 ]]();
}

detour zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::magic_circle_trigger_activate(trig_stub, player)
{
    if (level flag::get("margwa_ovums_practice")) {
        TrueMaxAmmo();
        switch (trig_stub.n_char_index) {
            case 0: {
                level.active_district = "Waterfront";
                break;
            }
            case 1: {
                level.active_district = "Canals";
                break;
            }
            case 2: {
                level.active_district = "Footlight";
                break;
            }
            case 3: {
                level.active_district = "Junction";
                break;
            }
            default: {
                level.active_district = "Junction";
                break;
            }
        }
    }
    level notify("magic_circle_" + trig_stub.n_char_index); // for splits
    [[ @zm_zod_sword<scripts\zm\zm_zod_sword_quest.gsc>::magic_circle_trigger_activate ]](trig_stub, player);
}