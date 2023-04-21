PickupSprayer()
{
	if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Fumigator");
    sprayers = struct::get_array("pod_sprayer_location", "targetname");
    foreach(sprayer in sprayers)
	{
		if(!IsDefined(sprayer.trigger) || sprayer.script_int != 1) continue;
		sprayer.trigger notify("trigger", self);
        break;
	}
}