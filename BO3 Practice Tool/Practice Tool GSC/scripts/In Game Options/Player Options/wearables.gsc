GiveZodMargwaHead()
{
    if (IsTrue(self.var_d89174ae)) {
        thread WriteToScreen("Already Have Margwa Head");
        return;
    }
    thread WriteToScreen("Giving Margwa Head");
    self.var_d89174ae = 1;
	self thread HideInBeast();
}

RemoveZodMargwaHead()
{
    thread WriteToScreen("Removing Margwa Head");
    self.var_d89174ae = undefined;
    self Detach("p7_zm_zod_margwa_head", "j_head");
	self notify("margwa_head_stop");
}

HideInBeast()
{
	self endon("death");
    self endon("margwa_head_stop");
	for (;;) {
		self Attach("p7_zm_zod_margwa_head", "j_head");
		self flag::wait_till("in_beastmode");
		self Detach("p7_zm_zod_margwa_head", "j_head");
		self flag::wait_till_clear("in_beastmode");
	}
}

detour zm_zod_ee_side<scripts\zm\zm_zod_ee_side.gsc>::function_68f6dbc2()
{
    self thread [[ @zm_zod_ee_side<scripts\zm\zm_zod_ee_side.gsc>::function_68f6dbc2 ]]();
    if (IsTrue(level.num_splits)) {
        return;
    }
    self waittill("trigger_activated", player);
    self notify("margwa_head_stop");
}

GiveClawHelmet()
{
    if (level flag::get("ee_claw_hat")) {
        thread WriteToScreen("Already Have Claw Helmet");
    }
    thread WriteToScreen("Giving Claw Helmet");
    [[ @zm_castle_ee_side<scripts\zm\zm_castle_ee_side.gsc>::function_90b13c3d ]]();
}

RemoveClawHelmet()
{
    thread WriteToScreen("Removing Claw Helmet");
    level flag::clear("ee_claw_hat");
    foreach (player in level.activeplayers) {
        self Detach("c_t6_zom_mech_claw_hat", "j_head");
    }
    callback::remove_on_spawned(@zm_castle_ee_side<scripts\zm\zm_castle_ee_side.gsc>::function_d249c76c);
}

GiveStalingradWearable(name)
{
    if (!IsDefined(level.var_37e960a1)) {
        [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_484ecd5 ]]();
    }

    if (IsDefined(self.var_bc5f242a)) {
        if (IsSubStr(self.var_bc5f242a, "wings") && IsSubStr(name, "wings") || IsSubStr(self.var_bc5f242a, "raz") && IsSubStr(name, "raz") || IsSubStr(self.var_bc5f242a, "sentinel") && IsSubStr(name, "sentinel")) {
            thread WriteToScreen("Already Have " + name);
            return;
        }
        self [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_caffcf07 ]]();
        if (name != "dragon_wings") {
            self [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_588ad36a ]]();
        }
    }
    else {
        self.var_bc5f242a = SpawnStruct();
    }

    switch (name) {
        case "dragon_wings": {
            self.var_bc5f242a.model = "c_zom_dlc3_player_wings";
            self.var_bc5f242a.tag = "j_spine4";
            self PlaySound("zmb_wearable_wing_wear");
            num = 3;
            break;
        }
        case "raz_hat": {
            self.var_bc5f242a.model = "c_zom_dlc3_player_raz_facemask";
            self.var_bc5f242a.
            self PlaySound("zmb_wearable_raz_wear");
            num = 1;
            break;
        }
        case "sentinel_hat": {
            self.var_bc5f242a.model = "c_zom_dlc3_player_sentinel_drone_hat";
            self.var_bc5f242a.
            self PlaySound("zmb_wearable_sent_wear");
            num = 2;
            break;
        }
        default: {
            return;
        }
    }
    thread WriteToScreen("Giving " + name);

    self.var_e7d196cc = name;
    self [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_20f2df00 ]]();
    zm::register_player_damage_callback(@zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_9c197fbf);
    self [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_793f10ed ]](num);
    self thread [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_2436f867 ]]();
    if (num == 3) {
        array::run_all(level.var_37e960a1, ::ShowToPlayer, self);
    }
}

RemoveStalingradWearable()
{
    thread WriteToScreen("Removing Wearable");
    if (self.var_bc5f242a.var_e7d196cc == "dragon_wings") {
        self [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_588ad36a ]]();
    }
    self [[ @zm_stalingrad_wearables<scripts\zm\zm_stalingrad_wearables.gsc>::function_caffcf07 ]]();
    self.var_bc5f242a = undefined;
    self.var_e7d196cc = undefined;
    level clientfield::set(("player" + self.entity_num) + "wearableItem", 0);
}

GiveGenesisWearable(name)
{
    switch (name) {
        case "s_weasels_hat": {
            model = "c_zom_dlc4_player_arlington_helmet";
            break;
        }
        case "s_helm_of_siegfried": {
            model = "c_zom_dlc4_player_siegfried_helmet";
            break;
        }
        case "s_helm_of_the_king": {
            model = "c_zom_dlc4_player_king_helmet";
            break;
        }
        case "s_dire_wolf_head": {
            model = "c_zom_dlc4_player_direwolf_helmet";
            break;
        }
        case "s_keeper_skull_head": {
            model = "c_zom_dlc4_player_keeper_helmet";
            break;
        }
        case "s_margwa_head": {
            model = "c_zom_dlc4_player_margwa_helmet";
            break;
        }
        case "s_apothicon_mask": {
            model = "c_zom_dlc4_player_apothican_helmet";
            break;
        }
        case "s_fury_head": {
            model = "c_zom_dlc4_player_fury_helmet";
            break;
        }
        default: {
            return;
        }
    }

    if (!IsDefined(self.var_bc5f242a)) {
        self.var_bc5f242a = SpawnStruct();
    }
    else if (self.var_bc5f242a.str_model == model) {
        thread WriteToScreen("Already Have " + name);
        return;
    }
    thread WriteToScreen("Giving " + name);
    self [[ @namespace_6b38abe3<scripts\zm\zm_genesis_wearables.gsc>::function_e5974b49 ]]();
    self [[ @namespace_6b38abe3<scripts\zm\zm_genesis_wearables.gsc>::function_a16ce474 ]](model, name, "j_head");
}

RemoveGenesisWearable()
{
    thread WriteToScreen("Removing Wearable");
    self.var_bc5f242a = undefined;
    self [[ @namespace_6b38abe3<scripts\zm\zm_genesis_wearables.gsc>::function_e5974b49 ]]();
	self [[ @namespace_6b38abe3<scripts\zm\zm_genesis_wearables.gsc>::function_b712ee6f ]](0);
	self [[ @namespace_6b38abe3<scripts\zm\zm_genesis_wearables.gsc>::function_30fb8e63 ]](0);
    if (self.characterindex != 2) {
		self SetCharacterBodyStyle(1);
	}
}

GiveTombWearable(name)
{
    switch (name) {
        case "golden_shovel": {
            if (IsTrue(self.dig_vars["has_upgraded_shovel"])) {
                thread WriteToScreen("Already Have Golden Shovel");
                return;
            }
            thread WriteToScreen("Giving Golden Shovel");
            self.dig_vars["has_upgraded_shovel"] = 1;
            if (!IsDefined(self.t_zombie_blood_dig) && self.player_perk_purchase_limit == level.perk_purchase_limit) {
                self thread [[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::ee_zombie_blood_dig ]]();
            }
            level clientfield::set([[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::function_f4768ce9 ]](self GetEntityNumber()), 2);
            self PlaySoundToPlayer("zmb_squest_golden_anything", self);
            break;
        }
        case "golden_helmet": {
            if (IsTrue(self.dig_vars["has_helmet"])) {
                thread WriteToScreen("Already Have Golden Helmet");
                return;
            }
            thread WriteToScreen("Giving Golden Helmet");
            self.dig_vars["has_helmet"] = 1;
            level clientfield::set([[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::function_6e5f017f ]](self GetEntityNumber()), 1);
            self PlaySoundToPlayer("zmb_squest_golden_anything", self);
            self thread [[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::function_85eef9f2 ]]();
            break;
        }
        default: {
            return;
        }
    }
}

RemoveTombWearable()
{
    thread WriteToScreen("Removing Wearables");
    self.dig_vars["has_upgraded_shovel"] = 0;
    self.dig_vars["has_helmet"] = 0;
    level clientfield::set([[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::function_f4768ce9 ]](self GetEntityNumber()), 0);
    level clientfield::set([[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::function_6e5f017f ]](self GetEntityNumber()), 0);
    if (IsDefined(self.var_8e065802)) {
        self Detach(self.var_8e065802.model, self.var_8e065802.tag);
        self.var_8e065802 = undefined;
    }
    self SetCharacterBodyStyle(0);
}

detour zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::ee_zombie_blood_dig()
{
	if (IsDefined(self.t_zombie_blood_dig) || self.player_perk_purchase_limit != level.perk_purchase_limit) {
        return;
    }

    self [[ @zm_tomb_dig<scripts\zm\zm_tomb_dig.gsc>::ee_zombie_blood_dig ]]();
}