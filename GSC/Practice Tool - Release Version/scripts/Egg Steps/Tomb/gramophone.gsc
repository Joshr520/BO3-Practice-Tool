TriggerCrypt()
{
    door = getentArray("chamber_entrance", "targetname")[1];
    trig_position = struct::get(door.targetname + "_position", "targetname");
    level notify("open_sesame");
    trig_position.has_vinyl = 1;
    level.b_open_all_gramophone_doors = 1;
    wait 0.5;
    trig_position.trigger notify("trigger", self);
    wait 1;
    level.b_open_all_gramophone_doors = 0;
}