autoexec MonitorServer()
{
    level endon("end_server");
    
    funcs = [];
    funcs["map"] = ::ParseMap; funcs["timescale"] = ::ParseTimescale; funcs["g_speed"] = ::ParseSeed; funcs["godmode"] = ::ParseGodmode; funcs["infinite_ammo"] = ::ParseInfiniteAmmo; funcs["set_points"] = ::ParseSetPoints;
    funcs["perks"] = ::ParsePerkMessage; funcs["weapon"] = ::ParseWeaponMessage; funcs["bgb"] = ::ParseBGBMessage; funcs["zombie_health"] = ::ParseZombieHealth; funcs["zombie_speed"] = ::ParseZombieSpeed;
    funcs["zombie_freeze"] = ::ParseZombieFreeze; funcs["zombie_spawns"] = ::ParseZombieSpawns; funcs["zombie_tp"] = ::ParseZombieTP; funcs["round"] = ::ParseRoundRequest; funcs["zombie_total"] = ::ParseZombieTotal;
    funcs["powerup"] = ::ParsePowerup; funcs["craftable"] = ::ParseCraftable; funcs["blocker"] = ::ParseBlocker;

    WaitFadein();

    for (;;) {
        msg = compiler::readserver();
        if (IsInt(msg) && !msg) {
            wait 0.05;
            continue;
        }

        tokens = StrTok(msg, "-");
        func_index = array::pop_front(tokens, 0);

        if (!IsDefined(funcs[func_index])) {
            DebugPrint("error", "Func: " + func_index + " not found");
            continue;
        }

        DebugPrint("debug", msg);

        level.players[0] thread [[ funcs[func_index] ]](tokens);
    }
}

ParseMap(tokens)
{
    Map(tokens[0]);
}

ParseTimescale(tokens)
{
    Timescale(tokens[0]);
}

ParseSeed(tokens)
{
    SetSpeed(tokens[0]);
}

ParseGodmode(tokens)
{
    Godmode(Int(tokens[0]));
}

ParseInfiniteAmmo(tokens)
{
    InfiniteAmmo(Int(tokens[0]));
}

ParseSetPoints(tokens)
{
    SetPlayerPoints(Int(tokens[0]));
}

ParsePerkMessage(tokens)
{
    switch (tokens[0]) {
        case "give": {
            if (tokens[1] == "all") {
                self GiveAllPerks();
            }
            else {
                self GivePerk(tokens[1]);
            }
            break;
        }
        case "take": {
            if (tokens[1] == "all") {
                self TakeAllPerks();
            }
            else {
                self TakePerk(tokens[1]);
            }
            break;
        }
        default: {
            break;
        }
    }
}

ParseWeaponMessage(tokens)
{
    switch (tokens[0]) {
        case "give": {
            self GivePlayerWeapon(tokens[1], Int(tokens[2]));
            break;
        }
        case "take": {
            self TakePlayerWeapon(tokens[1]);
            break;
        }
        default: {
            break;
        }
    }
}

ParseBGBMessage(tokens)
{
    switch (tokens[0]) {
        case "give": {
            thread WriteToScreen("Giving Gum " + GetBGBName(tokens[1]));
            self bgb::give(tokens[1]);
            break;
        }
        case "take": {
            if (self.bgb == "none") {
                thread WriteToScreen("No Gum To Take");
                return;
            }
            thread WriteToScreen("Taking Gum " + GetBGBName(self.bgb));
            self bgb::take();
            break;
        }
        case "use": {
            if (level.bgb[self.bgb].limit_type != "activated") {
                thread WriteToScreen("Gum Isn't Player Activated");
                return;
            }
            thread WriteToScreen("Using Gum Charge");
            self notify("bgb_activation", self.bgb);
            break;
        }
        case "activate": {
            self WaitForWeaponAvailable("bgb");
            thread WriteToScreen("Activating Gum " + GetBGBName(self.bgb));
            self notify("bgb_activation_request");
            break;
        }
        default: {
            break;
        }
    }
}

ParseZombieHealth(tokens)
{
    switch (Int(tokens[0])) {
        case 0: {
            RestoreZombieHealthOverride();
            break;
        }
        case 1: {
            SetZombieHealthOverride(Int(tokens[1]));
            break;
        }
        default: {
            break;
        }
    }
}

ParseZombieSpeed(tokens)
{
    switch (Int(tokens[0])) {
        case 0: {
            ResetZombieSpeedOverride();
            break;
        }
        case 1: {
            SetZombieSpeedOverride(ToLower(tokens[1]));
            break;
        }
        default: {
            break;
        }
    }
}

ParseZombieFreeze(tokens)
{
    switch (Int(tokens[0])) {
        case 0: {
            UnfreezeZombies();
            break;
        }
        case 1: {
            FreezeZombies();
            break;
        }
        default: {
            break;
        }
    }
}

ParseZombieSpawns(tokens)
{
    if (!Int(tokens[0])) {
        level endon("zombie_spawns_request");
        for (;;) {
            level flag::clear("spawn_zombies");
            level flag::wait_till("spawn_zombies");
        }
    }
    level notify("zombie_spawns_request");
    level flag::set("spawn_zombies");
}

ParseZombieTP(tokens)
{
    switch (tokens[0]) {
        case "freeze": {
            if (Int(tokens[1])) {
                SetTPZombiesOverride();
            }
            else {
                ResetTPZombiesOverride();
            }
            break;
        }
        default: {
            self TPZombiesToPlayer();
            break;
        }
    }
}

ParseRoundRequest(tokens)
{
    switch (tokens[0]) {
        case "goto": {
            GotoRound(Int(tokens[1]));
            break;
        }
        case "end": {
            GotoRound(level.round_number + 1);
            break;
        }
        case "restart": {
            GotoRound(level.round_number);
            break;
        }
        case "infinite": {
            InfiniteRound(Int(tokens[1]));
            break;
        }
        case "special": {
            StartSpecialRound();
            break;
        }
        default: {
            break;
        }
    }
}

ParseZombieTotal(tokens)
{
    SetZombieCount(Int(tokens[0]));
}

ParsePowerup(tokens)
{
    self GivePowerup(tokens[0], Int(tokens[1]), tokens[2]);
}

ParseCraftable(tokens)
{
    CollectCraftible(tokens[0], tokens[1]);
}

ParseBlocker(tokens)
{
    
}