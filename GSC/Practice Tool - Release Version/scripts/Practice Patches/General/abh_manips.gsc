#include scripts\zm\bgbs\_zm_bgb_anywhere_but_here;

// SOE
/*
    0 - Waterfront Junction
    1 - Footlight Junction
    2 - Waterfront Perk
    3 - Deep Waterfront
    4 - Footlight Perk
    5 - Deep Footlight
    6 - Canals Junction
    7 - Deep Canals
    8 - Spawn
    9 - Rift
    10 - Canals Perk
*/

ZodABHManip()
{
    if(!IsDefined(level.num_abh)) level.num_abh = 0;
    respawn_points = struct::get_array("player_respawn_point", "targetname");
    fallback_point = zm_bgb_anywhere_but_here::function_728dfe3();
    if(!IsDefined(level.abh_array) || level.num_abh >= level.abh_array.size)
    {
        if(IsDefined(level.old_abh_func)) level.var_2c12d9a6 = level.old_abh_func;
        else level.var_2c12d9a6 = undefined;
        return fallback_point;
    }
    respawn_point = respawn_points[level.abh_array[level.num_abh]];
    if(!zm_utility::is_point_inside_enabled_zone(respawn_point.origin))
    {
        level.var_2c12d9a6 = undefined;
        return fallback_point;
    }
    spots = struct::get_array(respawn_point.target, "targetname");
    foreach(spot in spots)
    {
        n_script_int = self GetEntityNumber() + 1;
        if(spot.script_int === n_script_int)
        {
            player_respawn_point = spot;
            break;
        }
    }
    level.num_abh++;
    if(level.num_abh >= level.abh_array.size)
    {
        if(IsDefined(level.old_abh_func)) level.var_2c12d9a6 = level.old_abh_func;
        else level.var_2c12d9a6 = undefined;
    }
    return player_respawn_point;
}