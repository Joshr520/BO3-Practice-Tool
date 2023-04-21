InitMapList()
{
    level.load_map_list = Array("zm_zod", "zm_factory", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis", "zm_prototype", "zm_asylum", "zm_sumpf", "zm_theater", "zm_cosmodrome", "zm_temple", "zm_moon", "zm_tomb");
}

LoadMap(num_map)
{
    Map(level.load_map_list[num_map]);
}