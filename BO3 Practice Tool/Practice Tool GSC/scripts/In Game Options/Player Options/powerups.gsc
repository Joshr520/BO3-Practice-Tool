GivePowerup(powerup, insta_grab = 1, displayName = undefined)
{
    if (IsSubStr(powerup, "code_cylinder") && !level flag::get("power_on")) {
        thread WriteToScreen("Power Required For Code Cylinders");
        return;
    }
    origin = insta_grab ? self.origin : self.origin + VectorScale(AnglesToForward((0, self GetPlayerAngles()[1], 0)), 60) + VectorScale((0, 0, 1), 5);
    zm_powerups::specific_powerup_drop(powerup, origin);
    if (IsDefined(displayName)) {
        thread WriteToScreen("Spawning Powerup " + displayName);
    }
    else {
        thread WriteToScreen("Spawning Powerup " + powerup);
    }
}