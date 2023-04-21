BuildFireStaff()
{  
    self thread PickupSinglePiece(4, 0);
    self thread PickupSinglePiece(4, 1);
    self thread PickupSinglePiece(4, 2);
    self thread PickupSinglePiece(4, 3);
    wait 2;
    self PickupSinglePiece(2, 4);
    foreach(stub in level.a_uts_craftables)
	{
        if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
        if(stub.equipname == "elemental_staff_fire")
        {
            stub.usetime = 0;
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
    wait 3;
}

CompleteFireSouls()
{
    foreach(volume in level.sacrifice_volumes)
    {
        volume.b_gods_pleased = 1;
    }
    level notify("fire_sacrifice_completed");
}

CompleteFireTorches()
{
    level flag::wait_till("fire_puzzle_1_complete");
    wait 1;
    torches = struct::get_array("church_torch_target", "script_noteworthy");
    foreach(torch in torches)
    {
        if(IsTrue(torch.b_correct_torch))
        {
            level notify("fire_staff_explosion", torch.origin);
            wait 0.25;
        }
    }
}