state("blackops3")
{
	byte round_counter : 0xA55BDEC;
    int level_time : 0xA6424FC;
    string13 map_name : 0x179DF840;
    int split : 0x17B3C3C8; // 0x17B3C3A8
}

startup
{
    vars.time_offset = 0;
    timer.CurrentTimingMethod = TimingMethod.GameTime;
}

start
{
	if(current.split > 0 && current.split > old.split)
    {
        vars.time_offset = current.level_time;
        return true;
    }
}

split
{
    return current.split > 1 && current.split > old.split;
}

gameTime
{
    return new TimeSpan(0, 0, 0, 0, current.level_time - vars.time_offset);
}

reset
{
    return (current.round_counter == 0 && old.round_counter != 0 || current.split == 0 && old.split != 0 || current.map_name.Equals("core_frontend"));
}

isLoading
{
    return true;
}