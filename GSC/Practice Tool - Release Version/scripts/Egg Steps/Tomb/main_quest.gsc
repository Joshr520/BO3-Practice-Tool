IceStaffUpgrade()
{
    staff = level.a_elemental_staffs["staff_water"];
    if(!self HasWeapon(staff.w_weapon))
    {
        self.get_player_weapon_limit = ::TradeWeaponLimit;
        foreach(stub in level.a_uts_craftables)
        {
            if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
            if(stub.equipname == "elemental_staff_water")
            {
                stub.usetime = 0;
                self BuildAndActivateTrigger(stub);
                wait 0.5;
                break;
            }
        }
    }
    self BuildAndActivateTrigger(staff.charge_trigger);
    staff.charger.charges_received = 20;
    self.get_player_weapon_limit = undefined;
}

FireStaffUpgrade()
{
    staff = level.a_elemental_staffs["staff_fire"];
    if(!self HasWeapon(staff.w_weapon))
    {
        self.get_player_weapon_limit = ::TradeWeaponLimit;
        foreach(stub in level.a_uts_craftables)
        {
            if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
            if(stub.equipname == "elemental_staff_fire")
            {
                stub.usetime = 0;
                self BuildAndActivateTrigger(stub);
                wait 0.5;
                break;
            }
        }
    }
    self BuildAndActivateTrigger(staff.charge_trigger);
    staff.charger.charges_received = 20;
    self.get_player_weapon_limit = undefined;
}

LightningStaffUpgrade()
{
    staff = level.a_elemental_staffs["staff_lightning"];
    if(!self HasWeapon(staff.w_weapon))
    {
        self.get_player_weapon_limit = ::TradeWeaponLimit;
        foreach(stub in level.a_uts_craftables)
        {
            if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
            if(stub.equipname == "elemental_staff_lightning")
            {
                stub.usetime = 0;
                self BuildAndActivateTrigger(stub);
                wait 0.5;
                break;
            }
        }
    }
    self BuildAndActivateTrigger(staff.charge_trigger);
    staff.charger.charges_received = 20;
    self.get_player_weapon_limit = undefined;
}

WindStaffUpgrade()
{
    staff = level.a_elemental_staffs["staff_air"];
    if(!self HasWeapon(staff.w_weapon))
    {
        self.get_player_weapon_limit = ::TradeWeaponLimit;
        foreach(stub in level.a_uts_craftables)
        {
            if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
            if(stub.equipname == "elemental_staff_air")
            {
                stub.usetime = 0;
                self BuildAndActivateTrigger(stub);
                wait 0.5;
                break;
            }
        }
    }
    self BuildAndActivateTrigger(staff.charge_trigger);
    staff.charger.charges_received = 20;
    self.get_player_weapon_limit = undefined;
}

AFD()
{
    level flag::wait_till("ee_all_staffs_upgraded");
    level.n_ee_robot_staffs_planted = 4;
    level flag::set("ee_all_staffs_placed");
}

RainFire()
{
    level flag::wait_till("ee_all_staffs_placed");
    wait 1;
    level flag::set("ee_mech_zombie_hole_opened");
}

UnleashHorde()
{
    level flag::wait_till("ee_mech_zombie_hole_opened");
    level flag::set("ee_mech_zombie_fight_completed");
    level flag::set("ee_quadrotor_disabled");
}

SkewerWingedBeast()
{
    level flag::wait_till("ee_mech_zombie_fight_completed");
    wait 1;
    level flag::set("ee_maxis_drone_retrieved");
}

WieldIronFirst()
{
    level flag::wait_till("ee_maxis_drone_retrieved");
    wait 1;
    level flag::set("ee_all_players_upgraded_punch");
}

RaiseHell()
{
    level flag::wait_till("ee_all_players_upgraded_punch");
    wait 1;
    level flag::set("ee_souls_absorbed");
}