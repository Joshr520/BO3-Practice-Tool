InitPerksList()
{
    level.custom_perks_list = Array("specialty_armorvest", "specialty_fastreload", "specialty_doubletap2", "specialty_quickrevive", "specialty_staminup", "specialty_additionalprimaryweapon", "specialty_deadshot", "specialty_widowswine", "specialty_electriccherry");
    level.custom_perks_names = [];
    level.custom_perks_names["specialty_armorvest"] = "Juggernog";
    level.custom_perks_names["specialty_fastreload"] = "Speed Cola";
    level.custom_perks_names["specialty_doubletap2"] = "Doubletap";
    level.custom_perks_names["specialty_quickrevive"] = "Quick Revive";
    level.custom_perks_names["specialty_staminup"] = "Stamin-Up";
    level.custom_perks_names["specialty_additionalprimaryweapon"] = "Mule Kick";
    level.custom_perks_names["specialty_deadshot"] = "Deadshot Daiquiri";
    level.custom_perks_names["specialty_widowswine"] = "Widow's Wine";
    level.custom_perks_names["specialty_electriccherry"] = "Electric Cherry";
    remove = [];
    foreach(perk in level.custom_perks_list)
    {
        if(!IsDefined(level._custom_perks[perk])) remove[remove.size] = perk;
    }
    foreach(perk in remove)
    {
        ArrayRemoveValue(level.custom_perks_list, perk, 0);
    }
}

GiveAllPerks()
{
    if(!IsDefined(self.perks_active)) self.perks_active = [];
    if(self.perks_active.size == level._custom_perks.size)
    {
        thread WriteToScreen("Already Have All Perks");
        return;
    }
    thread WriteToScreen("Giving All Perks");
	perks = GetArrayKeys(level._custom_perks);
	foreach(perk in perks)
    {
        if(!self hasPerk(perk)) self zm_perks::give_perk(perk, 0);
    }
}

TakeAllPerks()
{
    if(!IsDefined(self.perks_active)) self.perks_active = [];
    if(!self.perks_active.size)
    {
        thread WriteToScreen("No Perks To Take");
        return;
    }
    thread WriteToScreen("Taking All Perks");
    foreach(perk in self.perks_active)
    {
        self notify(perk + "_stop");
    }
}

GivePerk(num)
{
    if(!IsDefined(self.perks_active)) self.perks_active = [];
    perk = level.custom_perks_list[num];
    if(self HasPerk(perk))
    {
        thread WriteToScreen("Already Have Perk " + level.custom_perks_names[perk]);
        return;
    }
    thread WriteToScreen("Giving Perk " + level.custom_perks_names[perk]);
    self zm_perks::give_perk(perk, 0);
}

TakePerk(num)
{
    if(!IsDefined(self.perks_active)) self.perks_active = [];
    perk = level.custom_perks_list[num];
    if(!self HasPerk(perk))
    {
        thread WriteToScreen("Don't Have Perk " + level.custom_perks_names[perk]);
        return;
    }
    thread WriteToScreen("Taking Perk " + level.custom_perks_names[perk]);
    self notify(perk + "_stop");
}