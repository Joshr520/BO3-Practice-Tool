FlytrapPractice()
{
    if (level.mapname != "zm_factory") {
        DebugPrint("error", "Flytrap Practice Test attempted to load on " + level.mapDisplayName + " but it requires The Giant");
        return;
    }
    RegisterHotkey(0x74, "Target Bear: ^9F5", undefined, ::TargetBear, undefined, level.players[0]);
    RegisterHotKey(0x75, "Toggle Grenade Camera: ^9F6\n^3Camera: ", "Active", ::ToggleProjectileCamera, ::ProjectileActiveText);
    RegisterHotKey(0x76, "View Impact Point: ^9F7", undefined, ::ViewImpactPoint, undefined, level.players[0]);
    level.projectile_camera = true;
    level.display_messages = false;
    level flag::init("camera_viewing_projectile");
    level flag::init("camera_viewing_impact");

    WaitFadeIn();
    level.players[0] thread OpenAllDoors();
    level.players[0] thread ActivateAllPower();
    foreach (player in GetPlayers()) {
        player.score = 50000;
        player InfiniteAmmo(true);
        player GiveAllPerks();
        player thread WatchGrenades();
    }
    TrueMaxAmmo(); 

    level flag::wait_till("initial_blackscreen_passed");
    level.display_messages = true;
    thread WriteToScreen("Flytrap Practice Loaded");
    level flag::clear("spawn_zombies");

    targets = Array("ee_exp_monkey", "ee_bowie_bear", "ee_perk_bear");
    foreach (target in targets) {
        foreach(obj in GetEntArray(target, "targetname")) {
            obj Show();
        }
        trig = GetEnt("trig_" + target, "targetname");
        trig TriggerEnable(1);
        trig thread WaitTargetTriggered(target);
    }
}

TargetBear()
{
    self SetPlayerAngles(VectortoAngles(GetEnt("trig_ee_perk_bear", "targetname").origin - self GetTagOrigin("j_head")));
}

WatchGrenades()
{
    for (;;) {
        self waittill("grenade_fire", grenade, weapon);
        self thread WaitGrenadeExplode(grenade);
        self.projectile_impact_origin = undefined;
        if (level.projectile_camera) {
            self FollowProjectile(grenade, "grenade_camera_ended", "end_grenade_camera", "death", false);
            self notify("grenade_camera_ended");
        }
    }
}

WaitGrenadeExplode(grenade)
{
    grenade endon("explode");
    self thread UpdateImpactOnExplode(grenade);
    self.projectile_bounce_origin = undefined;
    for (;;) {
        grenade waittill("grenade_bounce", pos);
        self.projectile_bounce_origin = pos;
    }
}

UpdateImpactOnExplode(grenade)
{
    grenade waittill("explode");
    self.projectile_impact_origin = self.projectile_bounce_origin;
}

WaitTargetTriggered(target)
{
    for (;;) {
        self waittill("trigger");
        thread WriteToScreen("Triggered: " + target);
    }
}