#include scripts\codescripts\struct;

#include scripts\shared\system_shared;
#include scripts\shared\_burnplayer;
#include scripts\shared\callbacks_shared;
#include scripts\shared\clientfield_shared;
#include scripts\shared\ai_shared;
#include scripts\shared\vehicle_shared;
#include scripts\shared\vehicle_ai_shared;
#include scripts\shared\array_shared;
#include scripts\shared\math_shared;
#include scripts\shared\flag_shared;
#include scripts\shared\lui_shared;
#include scripts\shared\scene_shared;
#include scripts\shared\trigger_shared;
#include scripts\shared\visionset_mgr_shared;
#include scripts\shared\spawner_shared;
#include scripts\shared\exploder_shared;
#include scripts\shared\scoreevents_shared;
#include scripts\shared\util_shared;
#include scripts\shared\hud_util_shared;
#include scripts\shared\fx_shared;

#include scripts\shared\ai\zombie_utility;
#include scripts\shared\ai\systems\blackboard;
#include scripts\shared\ai\systems\gib;

#include scripts\zm\_zm;
#include scripts\zm\_zm_audio;
#include scripts\zm\_zm_score;
#include scripts\zm\_zm_bgb;
#include scripts\zm\_zm_blockers;
#include scripts\zm\_zm_equipment;
#include scripts\zm\_zm_pack_a_punch_util;
#include scripts\zm\_zm_perks;
#include scripts\zm\_zm_powerups;
#include scripts\zm\_zm_power;
#include scripts\zm\_zm_magicbox;
#include scripts\zm\_zm_weapons;
#include scripts\zm\_zm_hero_weapon;
#include scripts\zm\_zm_placeable_mine;
#include scripts\zm\_zm_utility;
#include scripts\zm\_zm_zonemgr;
#include scripts\zm\_zm_spawner;
#include scripts\zm\_zm_unitrigger;

#include scripts\zm\craftables\_zm_craftables;

#namespace serious;

autoexec __init__system__()
{
	compiler::detour();
	system::register("serious", ::__init__, undefined, undefined);
}

__init__()
{
	callback::on_start_gametype(::init);
	callback::on_connect(::on_player_connect);
	callback::on_spawned(::on_player_spawned);
}