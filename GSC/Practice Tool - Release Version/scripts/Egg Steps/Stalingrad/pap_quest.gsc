FinishPAPGrophs()
{
    self thread PickupSinglePiece(1, 1);
    self thread PickupSinglePiece(1, 0);
    self thread PickupSinglePiece(1, 2);

    while(level.var_583e4a97.var_365bcb3c < 3) wait 0.05;

    foreach(stub in level.a_uts_craftables)
    {
        if(stub.targetname == "dragonride_zm_craftable_trigger")
        {
            self thread BuildAndActivateTrigger(stub);
            break;
        }
    }
}