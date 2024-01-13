GiveAllPerks()
{
    if (IsDefined(self.perks_active) && IsDefined(level._custom_perks) && self.perks_active.size == level._custom_perks.size) {
        thread WriteToScreen("Already Have All Perks");
        return;
    }
    thread WriteToScreen("Giving All Perks");
	self zm_utility::give_player_all_perks();
}

TakeAllPerks()
{
    if (!IsDefined(self.perks_active) || !self.perks_active.size) {
        thread WriteToScreen("No Perks To Take");
        return;
    }
    thread WriteToScreen("Taking All Perks");
    foreach (perk in self.perks_active) {
        self notify(perk + "_stop");
    }
}

GivePerksAndRevive()
{
    self endon("disconnect");
	while(IsDefined(self)) {
        self waittill("player_downed");
        self zm_laststand::auto_revive(self, 0);
		self zm_utility::give_player_all_perks();
	}
}

GivePerk(perk)
{
    if (self HasPerk(perk)) {
        thread WriteToScreen("Already Have Perk " + perk);
        return;
    }
    thread WriteToScreen("Giving Perk " + GetPerkDisplayName(perk));
    self zm_perks::give_perk(perk, 0);
}

TakePerk(perk)
{
    if (!IsDefined(self.perks_active) || !self HasPerk(perk)) {
        thread WriteToScreen("Don't Have Perk " + GetPerkDisplayName(perk));
        return;
    }
    thread WriteToScreen("Taking Perk " + GetPerkDisplayName(perk));
    self notify(perk + "_stop");
}

GetPerkDisplayName(perk)
{
    switch (perk) {
        case "specialty_armorvest": {
            return "Juggernog";
        }
        case "specialty_fastreload": {
            return "Speed Cola";
        }
        case "specialty_doubletap2": {
            return "Double Tap";
        }
        case "specialty_quickrevive": {
            return "Quick Revive";
        }
        case "specialty_staminup": {
            return "Stamin-Up";
        }
        case "specialty_additionalprimaryweapon": {
            return "Mule Kick";
        }
        case "specialty_widowswine": {
            return "Widow's Wine";
        }
        case "specialty_deadshot": {
            return "Deadshot Daiquiri";
        }
        case "specialty_electriccherry": {
            return "Electric Cherry";
        }
        default: {
            return "None";
        }
    }
}