GetDragonEgg()
{
    level flag::set("dragon_pavlov_first_time");
    wait 0.1;
    GetEnt("egg_drop_damage", "targetname") notify("damage");
    wait 0.1;
    level scene::skip_scene("p7_fxanim_zm_stal_pavlov_boards_bundle", 0, 0, 0);
    wait 0.5;
    trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(struct::get("dragon_egg_pickup", "targetname").s_unitrigger, self);
    zm_unitrigger::assess_and_apply_visibility(trig, trig.stub, self, 0);
    trig notify("trigger", self);
    wait 0.1;
    trig Delete();
}

// 0 = spawn, 1 = dc, 2 = inf, 3 = arm
ActivateEggNest(location)
{
    trig = zm_unitrigger::check_and_build_trigger_from_unitrigger_stub(struct::get_array("egg_cook_loc")[location].s_unitrigger, self);
    zm_unitrigger::assess_and_apply_visibility(trig, trig.stub, self, 0);
    trig notify("trigger", self);
}