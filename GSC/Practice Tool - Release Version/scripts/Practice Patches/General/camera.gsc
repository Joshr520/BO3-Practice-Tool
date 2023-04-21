CameraPos(key)
{
    pos = (0, 0, 0);
    angles = (0, 0, 0);
    for(;;)
    {
        while(!compiler::waitkey(key)) wait 0.05;
        pos = self GetTagOrigin("j_head");
        angles = self GetPlayerAngles();
        self CameraSetPosition(pos, angles);
        thread WriteToScreen("Updated Camera Position");
    }
}

CameraToggle(key)
{
    camera_active = false;
    for(;;)
    {
        while(!compiler::waitkey(key)) wait 0.05;
        camera_active = !camera_active;
        self CameraActivate(camera_active);
        thread WriteToScreen("Camera Toggled");
    }
}