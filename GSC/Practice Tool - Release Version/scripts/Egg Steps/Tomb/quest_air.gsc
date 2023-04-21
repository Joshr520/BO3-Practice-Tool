BuildWindStaff()
{
    self thread PickupSinglePiece(3, 0);
    self thread PickupSinglePiece(3, 1);
    self thread PickupSinglePiece(3, 2);
    self thread PickupSinglePiece(3, 3);
    wait 2;
    self thread PickupSinglePiece(2, 2);
    foreach(stub in level.a_uts_craftables)
	{
        if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
        if(stub.equipname == "elemental_staff_air")
        {
            stub.usetime = 0;
            self BuildAndActivateTrigger(stub);
            break;
        }
        
    }
    wait 3;
}

CompleteWindRings()
{
    level flag::set("air_puzzle_1_complete");
}

CompleteWindSmoke()
{
    level flag::wait_till("air_puzzle_1_complete");
    level flag::set("air_puzzle_2_complete");
}