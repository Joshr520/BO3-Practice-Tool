InitCustomGums()
{
    level.custom_bgb_list = Array("zm_bgb_alchemical_antithesis", "zm_bgb_always_done_swiftly", "zm_bgb_anywhere_but_here", "zm_bgb_armamental_accomplishment", "zm_bgb_arms_grace", "zm_bgb_arsenal_accelerator", "zm_bgb_coagulant", "zm_bgb_danger_closest", "zm_bgb_eye_candy", "zm_bgb_firing_on_all_cylinders", "zm_bgb_impatient", "zm_bgb_in_plain_sight", "zm_bgb_lucky_crit", "zm_bgb_newtonian_negation", "zm_bgb_now_you_see_me", "zm_bgb_projectile_vomiting", "zm_bgb_stock_option", "zm_bgb_sword_flay", "zm_bgb_tone_death", "zm_bgb_aftertaste", "zm_bgb_board_games", "zm_bgb_board_to_death", "zm_bgb_bullet_boost", "zm_bgb_burned_out", "zm_bgb_cache_back", "zm_bgb_crate_power", "zm_bgb_crawl_space", "zm_bgb_dead_of_nuclear_winter", "zm_bgb_disorderly_combat", "zm_bgb_ephemeral_enhancement", "zm_bgb_extra_credit", "zm_bgb_fatal_contraption", "zm_bgb_fear_in_headlights", "zm_bgb_flavor_hexed", "zm_bgb_head_drama", "zm_bgb_idle_eyes", "zm_bgb_im_feelin_lucky", "zm_bgb_immolation_liquidation", "zm_bgb_kill_joy", "zm_bgb_killing_time", "zm_bgb_licensed_contractor", "zm_bgb_mind_blown", "zm_bgb_near_death_experience", "zm_bgb_on_the_house", "zm_bgb_perkaholic", "zm_bgb_phoenix_up", "zm_bgb_pop_shocks", "zm_bgb_power_vacuum", "zm_bgb_profit_sharing", "zm_bgb_reign_drops", "zm_bgb_respin_cycle", "zm_bgb_round_robbin", "zm_bgb_secret_shopper", "zm_bgb_self_medication", "zm_bgb_shopping_free", "zm_bgb_slaughter_slide", "zm_bgb_soda_fountain", "zm_bgb_temporal_gift", "zm_bgb_unbearable", "zm_bgb_undead_man_walking", "zm_bgb_unquenchable", "zm_bgb_wall_power", "zm_bgb_whos_keeping_score");
}

GivePlayerGum(bgb_in)
{
    while(!self WeaponAvailableValidation()) wait 0.5;
    if(IsString(bgb_in))
    {
        self thread bgb::give(bgb_in);
        self notify("weapon_change_complete");
        str = StrTok(bgb_in, "_");
        name = "";
        for(i = 2; i < str.size; i++)
        {
            name += ToUpper(GetSubStr(str[i], 0, 1)) + GetSubStr(str[i], 1) + " ";
        }
        thread WriteToScreen("Giving Gum " + name);
        return;
    }
    foreach(index, bgb in level.custom_bgb_list)
    {
        if(index == bgb_in)
        {
            self thread bgb::give(bgb);
            self notify("weapon_change_complete");
            str = StrTok(bgb, "_");
            name = "";
            for(i = 2; i < str.size; i++)
            {
                name += ToUpper(GetSubStr(str[i], 0, 1)) + GetSubStr(str[i], 1) + " ";
            }
            thread WriteToScreen("Giving Gum " + name);
            return;
        }
    }
}

TakePlayerGum()
{
    while(!self WeaponAvailableValidation()) wait 0.5;
    if(self.bgb == "none")
    {
        thread WriteToScreen("No Gum Held");
        return;
    }
    str = StrTok(self.bgb, "_");
    name = "";
    for(i = 2; i < str.size; i++)
    {
        name += ToUpper(GetSubStr(str[i], 0, 1)) + GetSubStr(str[i], 1) + " ";
    }
    thread WriteToScreen("Taking Gum " + name);
    self bgb::take();
}

TakePlayerGumCharge()
{
    while(!self WeaponAvailableValidation()) wait 0.5;
    if(self.bgb == "none")
    {
        thread WriteToScreen("No Gum Held");
        return;
    }
    if(level.bgb[self.bgb].limit_type != "activated")
    {
        thread WriteToScreen("Gum Isn't Player Activated");
        return;
    }
    thread WriteToScreen("Taking Gum Charge");
    self notify("bgb_activation", self.bgb);
}

UsePlayerGumCharge()
{
    while(!self WeaponAvailableValidation()) wait 0.5;
    if(self.bgb == "none")
    {
        thread WriteToScreen("No Gum Held");
        return;
    }
    if(level.bgb[self.bgb].limit_type != "activated")
    {
        thread WriteToScreen("Gum Isn't Player Activated");
        return;
    }
    str = StrTok(self.bgb, "_");
    name = "";
    for(i = 2; i < str.size; i++)
    {
        name += ToUpper(GetSubStr(str[i], 0, 1)) + GetSubStr(str[i], 1) + " ";
    }
    thread WriteToScreen("Using Gum " + name);
    self notify("bgb_activation_request");
}