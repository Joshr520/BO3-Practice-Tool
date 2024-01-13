FollowProjectile(projectile, str_endon, str_notify, projectile_waittill, link_To_ent = true, func_on_delete = ::FreePlayerCamera)
{
    if (level flag::get("camera_viewing_impact") || level flag::get("camera_viewing_projectile")) {
        thread WriteToScreen("Camera in use");
        return;
    }

    self endon(str_notify);
    level flag::set("camera_viewing_projectile");
    camera = Spawn("script_model", projectile.origin);
    camera.angles = projectile.angles;
    if (link_To_ent) {
        camera LinkTo(projectile, "", (-100, 0, 10));
    }
    else {
        camera thread MoveToProjectile(projectile, str_notify);
    }
    self thread EndOnMelee(str_endon, str_notify, camera, func_on_delete);
    self FreezeControls(1);
    self CameraSetPosition(camera);
    self CameraSetLookAt(projectile);
    self CameraActivate(1);
    self PlayerCamLinkTo(camera, "tag_origin");
    projectile waittill(projectile_waittill);
    wait 0.1;
    self FreePlayerCamera();
    camera Delete();
}

FreePlayerCamera()
{
    self PlayerCamUnlink();
    self FreezeControls(0);
    self CameraActivate(0);
    level flag::clear("camera_viewing_projectile");
}

MoveToProjectile(projectile, str_endon)
{
    self endon(str_endon);
    self endon("death");
    wait 0.1;
    while (IsDefined(projectile)) {
        if (Abs(Distance(projectile.origin, self.origin)) >= 100) {
            self MoveTo((projectile.origin + self.origin) / 2, 0.25);
        }
        wait 0.05;
    }
}

ViewImpactPoint()
{
    if (level flag::get("camera_viewing_impact") || level flag::get("camera_viewing_projectile")) {
        thread WriteToScreen("Camera in use");
        return;
    }
    if (!IsDefined(self.projectile_impact_origin)) {
        thread WriteToScreen("No impact point to view");
        return;
    }

    level flag::set("camera_viewing_impact");
    obj = objpoints::create("camera_obj", self.projectile_impact_origin, "all", undefined);
    point = util::spawn_model("tag_origin", (obj.x, obj.y, obj.z), (0, 0, 0));
    camera = util::spawn_model("tag_origin", point.origin, point.angles);
    camera LinkTo(point, "tag_origin", (200, 0, 200));
    point thread RevolveCamera();
    self FreezeControls(1);
    self CameraSetPosition(camera);
    self CameraSetLookAt(point);
    self CameraActivate(1);
    while (!self MeleeButtonPressed()) {
        wait 0.05;
    }
    point notify("revolve_end");
    self FreePlayerCamera();
    objpoints::delete(obj);
    point Delete();
    camera Delete();
    level flag::clear("camera_viewing_impact");
}

RevolveCamera()
{
    self endon("revolve_end");
    for (;;) {
        self RotateYaw(360, 5);
        wait 5;
    }
}

ToggleProjectileCamera()
{
    level.projectile_camera = !level.projectile_camera;
}

ProjectileActiveText(hud_elem)
{
    hud_elem SetText(level.projectile_camera ? "Active" : "Inactive");
}