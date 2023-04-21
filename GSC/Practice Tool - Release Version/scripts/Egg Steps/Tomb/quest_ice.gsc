BuildIceStaff()
{
    self thread PickupSinglePiece(6, 0);
    self thread PickupSinglePiece(6, 1);
    self thread PickupSinglePiece(6, 2);
    self thread PickupSinglePiece(6, 3);
    wait 2;
    self PickupSinglePiece(2, 3);
    foreach(stub in level.a_uts_craftables)
	{
        if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
        if(stub.equipname == "elemental_staff_water")
        {
            stub.usetime = 0;
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
    wait 3;
}

CompleteIcePuzzle()
{
    tiles = GetEntArray("ice_ceiling_tile", "script_noteworthy");
    ice_gem = GetEnt("ice_chamber_gem", "targetname");
    for(i = 0; i < 6; i++)
    {
        foreach(tile in tiles)
        {
            if(tile.value == ice_gem.value)
            {
                tile notify("damage", 1, self, (0, 0, 0), tile.origin, "", "", "", "", GetWeapon("staff_water"));
                wait 1;
                ArrayRemoveValue(tiles, tile, 0);
                break;
            }
        }
    }
}

CompleteIceTombstones()
{
    level flag::wait_till("ice_puzzle_1_complete");
    wait 1;
    stones = struct::get_array("puzzle_stone_water", "targetname");
    foreach(stone in stones)
    {
        stone.e_model notify("damage", 1, self, (0, 0, 0), stone.origin, "", "", "", "", GetWeapon("staff_water"));
        wait 0.5;
        stone.e_model notify("damage", 1, self, (0, 0, 0), stone.origin, "MOD_BULLET", "", "", "", GetWeapon("ar_standard"));
    }
}