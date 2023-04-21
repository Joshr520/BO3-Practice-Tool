PickupBucket()
{
	if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Bucket");
    buckets = struct::get_array("water_bucket_location", "targetname");
	foreach(bucket in buckets)
	{
		if(!IsDefined(bucket.trigger) || bucket.script_int != 1) continue;
		bucket.trigger notify("trigger", self);
		break;
	}
}