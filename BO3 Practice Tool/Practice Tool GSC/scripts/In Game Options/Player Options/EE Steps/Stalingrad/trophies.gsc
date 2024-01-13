ActivateTrophy(trophy)
{
    if (!IsDefined(level.trophies)) {
        level.trophies = [];
    }

    switch (trophy) {
        case "gersh": {
            GetEnt("ee_keys_anomaly_damage_trig", "targetname") notify("damage", 1, level.players[0]);
            break;
        }
        case "bomb": {
            GetEnt("ee_keys_puddle_damage_trig", "targetname") notify("damage", 1, level.players[0], undefined, undefined, undefined, undefined, undefined, undefined, GetWeapon("launcher_dragon_fire"));
            wait 3;
            level scene::skip_scene("p7_fxanim_zm_stal_pickups_figure_nuke_bundle", 0, 0, 0);
            break;
        }
        case "groph": {
            if (!IsDefined(level.players[0].var_ae0fff53)) {
                level.players[0].var_ae0fff53 = level.var_ae0fff53;
                reset = true;
            }
            GetEnt("ee_keys_safe_damage_trig", "targetname") notify("damage", 1, level.players[0], undefined, undefined, "MOD_MELEE", undefined, undefined, undefined, level.players[0].var_ae0fff53);
            wait 0.05;
            if (IsTrue(reset)) {
                level.players[0].var_ae0fff53 = undefined;
            }
            break;
        }
        case "drone": {
            level notify(#"hash_a8bfa21a", GetWeapon("dragonshield"));
            break;
        }
        case "935": {
            GetEnt("ee_sewer_damage_trig", "targetname") notify("damage");
            break;
        }
        case "mangler": {
            level notify(#"hash_278aa663", "fxexp_200");
            break;
        }
        default: {
            return;
        }
    }

    level.trophies[trophy] = true;
}

PickupTrophy(trophy)
{
    if (!IsDefined(level.trophies) || !IsTrue(level.trophies[trophy])) {
        ActivateTrophy(trophy);
        wait 0.5;
    }

    switch (trophy) {
        case "gersh": {
            trophy = GetEnt("pickup_blob", "targetname");
            if (!IsDefined(trophy)) {
                return;
            }
            while (!IsDefined(trophy.s_unitrigger)) {
                wait 0.05;
            }
            trophy notify("trigger_activated", level.players[0]);
            break;
        }
        case "bomb": {
            trophy = GetEnt("pickup_nuke", "targetname");
            if (!IsDefined(trophy)) {
                IPrintLnBold("No Trophy");
                return;
            }
            while (!IsDefined(trophy.s_unitrigger)) {
                wait 0.05;
            }
            trophy notify("trigger_activated", level.players[0]);
            break;
        }
        case "groph": {
            ents = GetEntArray("script_model", "classname");
            foreach (ent in ents) {
                if (IsDefined(ent.origin) && ent.origin == (-625.845, 3467.62, -139)) {
                    ent notify("trigger_activated", level.players[0]);
                    break;
                }
            }
            break;
        }
        case "drone": {
            trophy = GetEnt("pickup_drone", "targetname");
            if (!IsDefined(trophy)) {
                return;
            }
            while (!IsDefined(trophy.s_unitrigger)) {
                wait 0.05;
            }
            trophy notify("trigger_activated", level.players[0]);
            break;
        }
        case "935": {
            ents = GetEntArray("script_model", "classname");
            foreach (ent in ents) {
                if (IsDefined(ent.origin) && ent.origin == (-2995.18, 21103.3, 15.9233)) {
                    ent notify("trigger_activated", level.players[0]);
                    break;
                }
            }
            break;
        }
        case "mangler": {
            trophy = struct::get("ee_keys_raz_struct", "targetname");
            if (!IsDefined(trophy)) {
                return;
            }
            while (!IsDefined(trophy.s_unitrigger)) {
                wait 0.05;
            }
            trophy notify("trigger_activated", level.players[0]);
            // trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(trophy.s_unitrigger, level.players[0]);
            // zm_unitrigger::assess_and_apply_visibility(trig, trig.stub, self, 0);
            // trig notify("trigger", self);
            break;
        }
        default: {
            break;
        }
    }
}

ActivateAllTrophies()
{
    trophies = Array("gersh", "bomb", "groph", "drone", "935", "mangler");
    foreach (trophy in trophies) {
        thread ActivateTrophy(trophy);
    }
}

PickupAllTrophies()
{
    trophies = Array("gersh", "bomb", "groph", "drone", "935", "mangler");
    foreach (trophy in trophies) {
        thread PickupTrophy(trophy);
    }
}