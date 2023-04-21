ShootCharacterStones()
{
    level.var_753f10ae[1] notify("trigger");
    wait 0.15;
    level.var_753f10ae[2] notify("trigger");
    wait 0.15;
    level.var_753f10ae[0] notify("trigger");
    wait 0.15;
    level.var_753f10ae[3] notify("trigger");
}

FinishKeeper()
{
    self PickupKeeperParts();
    wait 1;
    callbox = struct::get("companion_callbox_" + level.var_1e5eff79[0], "script_noteworthy");
    found_stub = undefined;
    foreach(stub in level._unitriggers.trigger_stubs)
	{
        if(callbox.unitrigger_stub.origin == stub.origin)
        {
            self BuildAndActivateTrigger(stub, 1);
            break;
        }
    }
    while(IsTrue(self.is_flung) || IsTrue(self.b_teleporting)) wait 0.05;
    org = self.origin;
    self SetOrigin(callbox.unitrigger_stub.origin);
    self FreezeControls(1);
    wait 3;
    ents = GetEntArray();
    wait_var = 0;
    bgb = self.bgb;
	self.bgb = "zm_bgb_shopping_free";
    foreach(ent in ents)
	{
        if(callbox.unitrigger_stub.origin == ent.origin)
        {
            ent notify("trigger", self);
            break;
        }
        wait_var++;
        if(wait_var >= 100)
        {
            wait 0.05;
            wait_var = 0;
        }
    }
    self SetOrigin(org);
    self FreezeControls(0);
    wait 0.1;
	self.bgb = bgb;
    wait 10;
    ents = GetEntArray();
    wait_var = 0;
    orgs = Array((2308.89, 3774.03, 1222), (5886.91, -3551.38, 302), (-2228.08, -3341.28, -1633));
    skip = 0;
    rock = undefined;
    foreach(ent in ents)
	{
        foreach(org in orgs)
        {
            if(org == ent.origin)
            {
                rock = org;
                skip = 1;
            }
        }
        if(skip) break;
        wait_var++;
        if(wait_var >= 100)
        {
            wait 0.05;
            wait_var = 0;
        }
    }
    while(IsTrue(self.is_flung) || IsTrue(self.b_teleporting)) wait 0.05;
    org = self.origin;
    self SetOrigin(rock + VectorScale((0, 0, 1), 32));
    self FreezeControls(1);
    self.is_flung = 1;
    while(!IsTrue(level.ai_companion.var_57376ff1))
    {
        level.ai_companion ForceTeleport(self.origin);
        wait 0.25;
    }
    wait 0.5;
    self SetOrigin(org);
    self FreezeControls(0);
    self.is_flung = 0;
    level.ai_companion notify("death");
    wait 2;
    orgs = Array((2100, 3848.25, 1227.75), (-2251.11, -3539.81, -1630.75), (5994.25, -3410.98, 308.61));
    while(!level flag::get("got_audio1"))
    {
        FindAndactivateEnt(orgs);
        wait 1;
    }
    wait 1;
    place = struct::get_array("audio_reel_place", "targetname")[1];
    place notify("trigger_activated", self);
}

FinishArnies()
{
    level flag::wait_till("placed_audio1");
    wait 1;
    level flag::set("acm_done");
    level.check_b_valid_poi = undefined;
    wait 1.5;
    reel = struct::get("acm_reel", "targetname");
    org = struct::get(reel.target, "targetname").origin;
    while(!level flag::get("got_audio2"))
    {
        FindAndactivateEnt(org);
        wait 1;
    }
    wait 1;
    place = struct::get_array("audio_reel_place", "targetname")[1];
    place notify("trigger_activated", self);
}

FinishBones()
{
    level flag::wait_till("placed_audio2");
    wait 1;
    bones = GetEntArray("b_target", "targetname");
    foreach(bone in bones)
    {
        bone notify("damage", 99999, self, (0, 0, 0), (0, 0, 0), "MOD_PROJECTILE", "", "", "", GetWeapon("ar_standard_upgraded"));
        wait 0.1;
        level notify(#"hash_2751215d", bone.origin, level.var_ed2646a1, self);
        wait 0.1;
    }
    level flag::wait_till("b_targets_collected");
    wait 1;
    reel = struct::get("b_target_reel", "targetname");
    level notify(#"hash_2751215d", reel.origin, level.var_ed2646a1, self);
    wait 7;
    level notify(#"hash_2751215d", reel.origin + VectorScale((0, 0, 1), 32), level.var_ed2646a1, self);
    wait 1;
    org = (-6.86, 410.19, -236.25);
    FindAndactivateEnt(org);
    wait 1;
    place = struct::get_array("audio_reel_place", "targetname")[0];
    place notify("trigger_activated", self);
}

ActivateAndGuideSophia()
{
    level flag::wait_till("phased_sophia_start");
    wait 1;
    level flag::set("sophia_beam_locked");
    wait 8;
    sophia = struct::get("ee_sophia_activate", "targetname");
    sophia notify("trigger_activated", self);
    wait 0.5;
    while(IsTrue(self.is_flung) || IsTrue(self.b_teleporting)) wait 0.05;
    org = self.origin;
    self LinkTo(level.var_2309b03e);
    level.var_2309b03e thread Speedup();
    while(Distance2DSquared((-25, -840, -380), self.origin) > 800) wait 0.05;
    self Unlink();
    wait 0.5;
    self SetOrigin((-25, -920, -380));
    wait 0.25;
    while(IsTrue(self.b_teleporting)) wait 0.05;
    wait 0.25;
    self LinkTo(level.var_2309b03e);
    level.var_2309b03e thread Speedup();
    wait 0.5;
    while(!level flag::get("sophia_at_teleporter")) wait 0.05;
    self Unlink();
    self SetOrigin(org);
}

Speedup()
{
    self endon("reached_end_node");
    for(;;)
    {
        self SetSpeedImmediate(100);
        wait 0.05;
    }
}

PickupAndPlaceBook()
{
    level flag::wait_till("sophia_at_teleporter");
    wait 1;
    book = struct::get("ee_book_sams_room", "targetname");
    book notify("trigger_activated", self);
    wait 1;
    place = struct::get("ee_book_theater", "targetname");
    place notify("trigger_activated", self);
}

PickupAndFillEggs()
{
    level flag::wait_till("book_placed");
    wait 5;
    orgs = Array((-180, 587.5, -3387.75), (-203.75, 412.5, -3379.25), (280.75, 133.25, -3390.5), (372.5, 559.25, -3393));
    foreach(index, egg in level.var_393eea44)
    {
        egg notify("trigger_activated", self);
        wait 0.1;
        level.var_2a7689da[index - 1] notify("trigger_activated", self);
        wait 0.1;
    }
    while(!IsDefined(level.var_2a7689da[3].var_165d49f6.var_b99b1b98)) wait 0.05;
    wait 0.5;
    foreach(pod in level.var_2a7689da)
    {
        pod.var_165d49f6.var_b99b1b98 = 0;
        pod.var_165d49f6 notify(#"hash_71f0e810");
        wait 0.1;
    }
    wait 8;
    elec = struct::get_array("electricity_rune_quest_start", "targetname");
    fire = struct::get_array("fire_rune_quest_start", "targetname");
    light = struct::get_array("light_rune_quest_start", "targetname");
    shad = struct::get_array("shadow_rune_quest_start", "targetname");
    elec_orgs = [];
    fire_orgs = [];
    light_orgs = [];
    shad_orgs = [];
    elec_symbol_orgs = Array((5028, -1279, 381.725), (6133.75, -570, 381.725), (6131, -2894, 219.725));
    fire_symbol_orgs = Array((-4937, -26, -416.207), (-3979, 672, -456), (-4219, -562, -444.846));
    light_symbol_orgs = Array((644, 4476, 1258.13), (-268, 4290, 1367.75), (1630, 3866, 1517.31));
    shad_symbol_orgs = Array((-2013, -4171, -1667.54), (151, -6855, -1278.17), (-1999, -6757, -1635.3));
    rune_orgs = Array(elec_orgs, fire_orgs, light_orgs, shad_orgs);
    symbol_orgs = Array(elec_symbol_orgs, fire_symbol_orgs, light_symbol_orgs, shad_symbol_orgs);
    foreach(rune in elec)
    {
        elec_orgs[elec_orgs.size] = rune.origin;
    }
    foreach(rune in fire)
    {
        fire_orgs[fire_orgs.size] = rune.origin;
    }
    foreach(rune in light)
    {
        light_orgs[light_orgs.size] = rune.origin;
    }
    foreach(rune in shad)
    {
        shad_orgs[shad_orgs.size] = rune.origin;
    }
    foreach(index, org in orgs)
    {
        found = 0;
        while(!found)
        {
            foreach(stub in level._unitriggers.trigger_stubs)
            {
                if(DistanceSquared(org, stub.origin) <= 5)
                {
                    self BuildAndActivateTrigger(stub);
                    found = 1;
                    break;
                }
            }
            wait 2;
        }
        wait 0.1;
        found = 0;
        skip = 0;
        while(!found)
        {
            foreach(stub in level._unitriggers.trigger_stubs)
            {
                foreach(org in rune_orgs[index])
                {
                    if(DistanceSquared(org, stub.origin) <= 5)
                    {
                        self BuildAndActivateTrigger(stub);
                        skip = 1;
                        found = 1;
                        break;
                    }
                }
                if(skip) break;
            }
            wait 2;
        }
        found = 0;
        skip = 0;
        while(!found)
        {
            foreach(stub in level._unitriggers.trigger_stubs)
            {
                foreach(org in symbol_orgs[index])
                {
                    if(DistanceSquared(org, stub.origin) <= 5)
                    {
                        self BuildAndActivateTrigger(stub);
                        skip = 1;
                        found = 1;
                        break;
                    }
                }
                if(skip) break;
            }
            wait 2;
        }
    }
}

FinishFirstBoss()
{
    self SetOrigin((-364.035, -8082.19, -1327.55));
    self SetPlayerAngles((2.47742, -132.885, 0));
    level flag::wait_till("rune_circle_on");
    wait 4;
    level flag::set("boss_rush");
    level flag::set("grand_tour");
}

CollectToys()
{
    level flag::wait_till("grand_tour");
    wait 1;
    level flag::set("toys_collected");
}