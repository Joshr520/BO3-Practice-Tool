autoexec InitMapName()
{
    level.mapname = ToLower(GetDvarString("mapname", ""));
    switch (level.mapname) {
        case "zm_zod": {
            level.mapDisplayName = "Shadows of Evil";
        }
        case "zm_factory": {
            level.mapDisplayName = "The Giant";
        }
        case "zm_castle": {
            level.mapDisplayName = "Der Eisendrache";
        }
        case "zm_island": {
            level.mapDisplayName = "Zetsubou No Shima";
        }
        case "zm_stalingrad": {
            level.mapDisplayName = "Gorod Krovi";
        }
        case "zm_genesis": {
            level.mapDisplayName = "Revelations";
        }
        case "zm_prototype": {
            level.mapDisplayName = "Nacht Der Untoten";
        }
        case "zm_asylum": {
            level.mapDisplayName = "Verruckt";
        }
        case "zm_sumpf": {
            level.mapDisplayName = "Shi No Numa";
        }
        case "zm_theater": {
            level.mapDisplayName = "Kino Der Toten";
        }
        case "zm_cosmodrome": {
            level.mapDisplayName = "Ascension";
        }
        case "zm_temple": {
            level.mapDisplayName = "Shangri-La";
        }
        case "zm_moon": {
            level.mapDisplayName = "Moon";
        }
        case "zm_tomb": {
            level.mapDisplayName = "Origins";
        }
        default: {
            level.mapDisplayName = "Unknown";
            break;
        }
    }
}

DebugPrint(type = "debug", msg)
{
    file = "Practice Tool/debug.log";

    switch (type) {
        case "info": {
            type = "[INFO] ";
            break;
        }
        case "debug": {
            type = "[DEBUG] ";
            break;
        }
        case "error": {
            type = "[ERROR] ";
            break;
        }
        default: {
            type = "[INFO] ";
            break;
        }
    }

    time = "[TIME] | " + GetTime() + " | ";

    compiler::debugmsg(file, type + time + msg, true);
}

IsTrue(bool)
{
	if (IsDefined(bool) && bool) {
        return true;
    }
	else {
        return false;
    }
}

IsFalse(bool)
{
    if (!IsDefined(bool) || bool) {
        return false;
    }
    return true;
}

FlagDebug(flag)
{
    for (;;) {
        level flag::wait_till(flag);
        DebugPrint("Flag Set: " + flag);
        level flag::wait_till_clear(flag);
        DebugPrint("Flag Clear: " + flag);
    }
}

GiveLoadout()
{
    self GiveWeapon(level.weaponbasemelee);
    self zm_weapons::weapon_give(level.start_weapon, 0, 0, 1, 0);
    self GiveMaxAmmo(level.start_weapon);
    self zm_weapons::weapon_give(level.super_ee_weapon, 0, 0, 1, 1);
}

TrueMaxAmmo() {
    foreach (player in level.activeplayers) {
        foreach (weapon in player GetWeaponsList(1)) {
            player zm_weapons::ammo_give(weapon);
            player SetWeaponAmmoClip(weapon, weapon.clipsize);
            if (zm_utility::is_hero_weapon(weapon)) {
                player GadgetPowerSet(player GadgetGetSlot(weapon), 100);
            }
        }
    }
}

WriteToScreen(text)
{
    if (!level.display_messages) {
        return;
    }

    message = hud::createserverfontstring("big", 1.1);
    if (level.messages_prompt.size >= 6) {
        level.messages_prompt[0] Destroy();
        ArrayRemoveIndex(level.messages_prompt, 0, 0);
    }
    level.messages_prompt[level.messages_prompt.size] = message;
    index = 0;
    for (i = level.messages_prompt.size; i > 0; i--) {
        level.messages_prompt[i-1] hud::setpoint("BOTTOM LEFT", "BOTTOM LEFT", 20, (-130 - (15 * index)));
        index++;
    }
    if (IsDefined(message)) {
        message SetText(text);
        message.alpha = 1;
    }
    wait 2.5;
    if (IsDefined(message)) {
        message FadeOverTime(2);
        message.alpha = 0;
    }
    wait 2;
    if (IsDefined(message)) {
        message Destroy();
        ArrayRemoveValue(level.messages_prompt, message, 0);
    }
}

IsScenePlaying(scene)
{
    return self scene::is_playing(scene);
}

WaitFadeIn()
{
    while (!IsDefined(level.n_gameplay_start_time)) {
        wait 0.05;
    }
}

DamageEnt(damage, attacker, direction_vec, point, type, tagname, modelname, partname, weapon, inflictor)
{
    if (!IsDefined(damage)) {
        damage = 1;
    }
    if (!IsDefined(attacker)) {
        attacker = GetPlayers()[0];
    }
    if (!IsDefined(type)) {
        type = "MOD_PROJECTILE";
    }
    if (!IsDefined(weapon)) {
        weapon = GetWeapon("ar_standard");
    }

    self notify("damage", damage, attacker, direction_vec, point, type, tagname, modelname, partname, weapon, inflictor);
}

WaitEntDeath(time_delay, str_notify)
{
    self waittill("death");
    if (IsDefined(time_delay)) {
        wait time_delay;
    }
    self notify(str_notify);
}

EndOnMelee(str_endon, str_notify, ent_delete, func_on_delete = undefined)
{
    self endon(str_endon);
    while (!self MeleeButtonPressed()) {
        wait 0.05;
    }
    self notify(str_notify);
    if (IsDefined(ent_delete)) {
        ent_delete Delete();
    }
    if (IsDefined(func_on_delete)) {
        self [[ func_on_delete ]]();
    }
}

InitHud(alpha = 1)
{
    hud = NewTeamHudElem("allies");

	hud.alignX = "left";
	hud.alignY = "top";
	hud.horzAlign = "user_left";
	hud.vertAlign = "user_top";
	hud.foreground = true;
	hud.hidewheninmenu = true;
	hud.x = 0;
	hud.y = 5;
	hud.alpha = alpha;
	hud.font = "default";
	hud.fontScale = 1.5;
	hud.color = (1,1,1);
	
	return hud;
}