#include scripts\shared\system_shared;
#include scripts\shared\callbacks_shared;
#include scripts\shared\flag_shared;

#include scripts\zm\_zm;
#include scripts\zm\_zm_stats;

#namespace serious;

autoexec __init__sytem__()
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