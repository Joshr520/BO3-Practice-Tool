#include scripts\codescripts\struct;
#include scripts\shared\util_shared;

#include scripts\shared\system_shared;
#include scripts\shared\callbacks_shared;
#include scripts\shared\clientfield_shared;
#include scripts\shared\postfx_shared;

#namespace serious;

autoexec __init__system__()
{
	compiler::detour();
	system::register("serious", ::__init__, undefined, undefined);
}

__init__()
{
	callback::on_start_gametype(::init);
	callback::on_localplayer_spawned(::on_localplayer_spawned);
}