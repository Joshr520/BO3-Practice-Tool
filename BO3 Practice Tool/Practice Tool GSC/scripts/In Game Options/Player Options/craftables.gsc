CollectCraftible(craft_name, piece_name)
{
    craft_stub = level.zombie_include_craftables[craft_name];
    if (!IsDefined(craft_stub)) {
        if (piece_name == "all") {
            PickupKeeperPart("totem");
            PickupKeeperPart("head");
            PickupKeeperPart("gem");
            return;
        }
        else if (piece_name == "totem" || piece_name == "head" || piece_name == "gem") {
            PickupKeeperPart(piece_name);
            return;
        }
        thread WriteToScreen("Craftable: " + craft_name + " Not Found");
    }
    foreach (piece_stub in craft_stub.a_piecestubs) {
        if (piece_name == "all" || piece_stub.pieceName == piece_name) {
            if (IsDefined(piece_stub.pieceSpawn)) {
                self thread zm_craftables::player_take_piece(piece_stub.pieceSpawn);
            }
        }
    }
}