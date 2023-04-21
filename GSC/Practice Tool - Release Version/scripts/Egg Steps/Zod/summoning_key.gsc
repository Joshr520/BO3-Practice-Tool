PickupKey()
{
    if(!level.craftables_picked_up) thread WriteToScreen("Activating Summoning Key");
    level.zod_smashables["unlock_quest_key"].m_e_trigger UseBy(self);
    wait 1;
    if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Summoning Key");
    key = GetEnt("quest_key_pickup", "targetname").unitrigger_stub;
    key thread [[key.trigger_func]]();
    key notify("trigger", self);
}