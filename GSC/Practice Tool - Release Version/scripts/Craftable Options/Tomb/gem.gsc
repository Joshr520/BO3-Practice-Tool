PickupStaffGem(piece)
{
    num = 0;
    switch(piece.craftableName)
    {
        case "elemental_staff_fire":
            num = 1;
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Fire Gem");
            break;
        case "elemental_staff_air":
            num = 2;
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Wind Gem");
            break;
        case "elemental_staff_lightning":
            num = 3;
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Lightning Gem");
            break;
        case "elemental_staff_water":
            num = 4;
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Ice Gem");
            break;
    }
    if(!num) return;
    level notify("player_teleported", self, num);
    while(!IsDefined(piece.piecespawn))
    {
        wait 0.05;
    }
    wait 1;
    self thread zm_craftables::player_get_craftable_piece(piece.craftablename, piece.pieceName);
}