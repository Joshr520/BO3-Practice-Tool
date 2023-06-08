init()
{
    SetDvar("sv_cheats", 1);
    SetDvar("scr_firstGumFree", 1);
	SetDvar("zm_private_rankedmatch", 1);
    level.player_out_of_playable_area_monitor = 0; //ADD THIS AS AN OPTION
    level.givecustomloadout = ::GiveLoadout;
    level.onlinegame = true;
    level.zm_disable_recording_stats = true;
    level.rankedmatch = 1;
    level.var_dfc343e9 = 0;

    level.round_sr = false;
    level.pap_sr = false;
    level.basic_timer = true;
    level.map_specific_timer = false;
}

on_player_connect()
{
    self InitVars();
    level flag::wait_till("start_zombie_round_logic");
    if(!self IsHost()) return;

    WaitFadeIn();

    message = hud::createfontstring("big", 1.1);
    hud::setpoint("TOP", "MIDDLE", 20, 0);
    message SetText("BO3 Practice Tool Loaded: ^6github.com/Joshr520/BO3-Practice-Tool");
    wait 3;
    message FadeOverTime(5);
    message.alpha = 0;
    wait 5;
    message Destroy();
}

on_player_spawned()
{
    if(self IsTestClient()) return;

    level flag::wait_till("initial_blackscreen_passed");
}

DebugTesting()
{
    self thread GodMode(1);
    self thread InfiniteAmmo(1);
    self thread GiveAllPerks();
    self thread OpenAllDoors();
    self thread ActivateAllPower();
    self.score = 50000;
}

InitVars()
{
    self InitWeapons();
    self InitPowerups();
    self InitCraftNames();
    self InitPerksList();
    self InitValveSolutions();
    self InitCustomGums();
    self InitCustomFlags();
    self InitBlockerLists();
    self InitMapList();
    self LoadSettings();
    level.craftables_picked_up = 0;
    level.MAX_MESSAGES = 6;
    level.messages_prompt = [];
    
    switch(level.script)
    {
        case "zm_zod":
        {
            level.smashables_open = 0;
            break;
        }
        case "zm_genesis":
        {
            level.bones_origin = [];
            level.bones_model = [];
            foreach(index, bone in GetEntArray("b_target", "targetname"))
            {
                level.bones_origin[index] = bone.origin;
                level.bones_model[index] = bone.model;
            }
            break;
        }
    }
    
}

InitCustomFlags()
{
    level flag::init("round_skip_request");
    level flag::init("puzzle_practice");
    level flag::init("tomb_soft_patch");
}

IsTrue(bool)
{
	if(IsDefined(bool) && bool) return true;
	else return false;
}

GiveLoadout()
{
    self GiveWeapon(level.weaponbasemelee);
    self zm_weapons::weapon_give(level.start_weapon, 0, 0, 1, 0);
    self GiveMaxAmmo(level.start_weapon);
    self zm_weapons::weapon_give(level.super_ee_weapon, 0, 0, 1, 1);
}

WriteToScreen(text)
{
    if(!IsDefined(level.messages_prompt)) level.messages_prompt = [];
    message = hud::createfontstring("big", 1.1);
    if(level.messages_prompt.size >= level.MAX_MESSAGES)
    {
        level.messages_prompt[0] Destroy();
        ArrayRemoveIndex(level.messages_prompt, 0, 0);
    }
    level.messages_prompt[level.messages_prompt.size] = message;
    index = 0;
    for(i = level.messages_prompt.size; i > 0; i--)
    {
        level.messages_prompt[i-1] hud::setpoint("BOTTOM LEFT", "BOTTOM LEFT", 20, (-130 - (15 * index)));
        index++;
    }
    if(IsDefined(message))
    {
        message SetText(text);
        message.alpha = 1;
    }
    wait 2.5;
    if(IsDefined(message))
    {
        message FadeOverTime(2);
        message.alpha = 0;
    }
    wait 2;
    if(IsDefined(message))
    {
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
    while(!IsDefined(level.n_gameplay_start_time)) wait 0.05;
}

TradeWeaponLimit(player)
{
    return player GetWeaponsListPrimaries().size + 10;
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