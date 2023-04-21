InitCraftNames()
{
    level.tree_names = [];
    level.piece_names = [];
    level.alternate_names = [];
    switch(level.script)
    {
        case "zm_zod":
            level.tree_names["ritual_boxer"] = "memento_boxer"; level.tree_names["ritual_detective"] = "memento_detective"; level.tree_names["ritual_femme"] = "memento_femme"; level.tree_names["ritual_magician"] = "memento_magician";
            level.piece_names["memento_boxer"] = "Championship Belt"; level.piece_names["memento_detective"] = "Badge"; level.piece_names["memento_femme"] = "Hairpiece"; level.piece_names["memento_magician"] = "Golden Fountain Pen";
            level.tree_names["ritual_pap"] = Array("relic_boxer", "relic_detective", "relic_femme", "relic_magician");
            level.piece_names["relic_boxer"] = "Boxer Gateworm"; level.piece_names["relic_detective"] = "Detective Gateworm"; level.piece_names["relic_femme"] = "Femme Gateworm"; level.piece_names["relic_magician"] = "Magician Gateworm";
            level.tree_names["idgun"] = Array("part_heart", "part_skeleton", "part_xenomatter");
            level.piece_names["part_heart"] = "Margwa Heart"; level.piece_names["part_skeleton"] = "Margwa Tentacle"; level.piece_names["part_xenomatter"] = "Xenomatter";
            level.tree_names["police_box"] = Array("fuse_01", "fuse_02", "fuse_03");
            level.piece_names["fuse_01"] = "Fuse 1"; level.piece_names["fuse_02"] = "Fuse 2"; level.piece_names["fuse_03"] = "Fuse 3";
            level.tree_names["craft_shield_zm"] = Array("dolly", "door", "clamp");
            level.piece_names["dolly"] = "Shield Dolly"; level.piece_names["door"] = "Shield Door"; level.piece_names["clamp"] = "Shield Clamp";
            level.alternate_names["key"] = ::PickupKey; level.alternate_names["sprayer"] = ::PickupSprayer;
            break;
        case "zm_castle":
            level.tree_names["gravityspike"] = Array("part_body", "part_guards", "part_handle");
            level.piece_names["part_body"] = "Rag Piece Body"; level.piece_names["part_guards"] = "Rag Piece Guards"; level.piece_names["part_handle"] = "Rag Piece Handle";
            level.tree_names["craft_shield_zm"] = Array("dolly", "door", "clamp");
            level.piece_names["dolly"] = "Shield Dolly"; level.piece_names["door"] = "Shield Door"; level.piece_names["clamp"] = "Shield Clamp";
            break;
        case "zm_island":
            level.tree_names["gaskmask"] = Array("part_visor", "part_filter", "part_strap");
            level.piece_names["part_visor"] = "Gasmask Visor"; level.piece_names["part_filter"] = "Gasmask Filter"; level.piece_names["part_strap"] = "Gasmask Strap";
            level.tree_names["craft_shield_zm"] = Array("dolly", "door", "clamp");
            level.piece_names["dolly"] = "Shield Dolly"; level.piece_names["door"] = "Shield Door"; level.piece_names["clamp"] = "Shield Clamp";
            level.alternate_names["ww1_found"] = ::PickupWWPart; level.alternate_names["ww2_found"] = ::PickupWWPart; level.alternate_names["ww3_found"] = ::PickupWWPart;
            level.alternate_names["wwup1_found"] = ::PickupWWPart; level.alternate_names["wwup2_found"] = ::PickupWWPart; level.alternate_names["wwup3_found"] = ::PickupWWPart;
            level.alternate_names["valve1_found"] = ::PickupMachinePart; level.alternate_names["valve2_found"] = ::PickupMachinePart; level.alternate_names["valve3_found"] = ::PickupMachinePart;
            level.alternate_names["bucket"] = ::PickupBucket;
            break;
        case "zm_stalingrad":
            level.tree_names["dragonride"] = Array("part_codes", "part_transmitter", "part_map");
            level.piece_names["part_codes"] = "Network Codes"; level.piece_names["part_transmitter"] = "Network Transmitter"; level.piece_names["part_map"] = "Network Map";
            level.tree_names["craft_shield_zm"] = Array("dolly", "door", "clamp");
            level.piece_names["dolly"] = "Shield Dolly"; level.piece_names["door"] = "Shield Door"; level.piece_names["clamp"] = "Shield Clamp";
            break;
        case "zm_genesis":
            level.tree_names["craft_shield_zm"] = Array("dolly", "door", "clamp");
            level.piece_names["dolly"] = "Shield Dolly"; level.piece_names["door"] = "Shield Door"; level.piece_names["clamp"] = "Shield Clamp";
            level.alternate_names["totem"] = ::PickupKeeperPart; level.alternate_names["head"] = ::PickupKeeperPart; level.alternate_names["gem"] = ::PickupKeeperPart;
            break;
        case "zm_tomb":
            level.tree_names["elemental_staff_water"] = Array("gem", "upper_staff", "middle_staff", "lower_staff");
            level.tree_names["elemental_staff_lightning"] = Array("gem", "upper_staff", "middle_staff", "lower_staff");
            level.tree_names["elemental_staff_fire"] = Array("gem", "upper_staff", "middle_staff", "lower_staff");
            level.tree_names["elemental_staff_air"] = Array("gem", "upper_staff", "middle_staff", "lower_staff");
            level.piece_names["gem"] = "Staff Gem"; level.piece_names["upper_staff"] = "Staff Upper"; level.piece_names["middle_staff"] = "Staff Middle"; level.piece_names["lower_staff"] = "Staff Lower";
            level.tree_names["gramophone"] = Array("vinyl_player", "vinyl_master", "vinyl_air", "vinyl_ice", "vinyl_fire", "vinyl_elec");
            level.piece_names["vinyl_player"] = "Gramophone"; level.piece_names["vinyl_master"] = "Master Disc"; level.piece_names["vinyl_air"] = "Wind Disc";
            level.piece_names["vinyl_ice"] = "Ice Disc"; level.piece_names["vinyl_fire"] = "Fire Disc"; level.piece_names["vinyl_elec"] = "Lightning Disc";
            level.tree_names["equip_dieseldrone"] = Array("body", "brain", "engine");
            level.piece_names["body"] = "Drone Body"; level.piece_names["brain"] = "Drone Brain"; level.piece_names["engine"] = "Drone Engine";
            level.tree_names["craft_shield_zm"] = Array("dolly", "door", "clamp");
            level.piece_names["dolly"] = "Shield Dolly"; level.piece_names["door"] = "Shield Door"; level.piece_names["clamp"] = "Shield Clamp";
            level.alternate_names["shovel"] = ::PickupShovel;
            break;
    }
    for(i = GetArrayKeys(level.tree_names).size; i >= 0 ; i--)
    {
        key = GetArrayKeys(level.tree_names)[i];
        level.names_index[i] = key;
    }

    level.craft_names_used = [];
    level.craft_nums_used = [];
    level.interface_crafts = [];
    switch(level.script)
    {
        case "zm_zod":
            level.interface_crafts = Array("memento_boxer", "memento_detective", "memento_femme", "memento_magician", "relic_boxer", "relic_detective", "relic_femme", "relic_magician", "part_heart", "part_skeleton", "part_xenomatter", "fuse_01", "fuse_02", "fuse_03", "dolly", "door", "clamp");
            break;
        case "zm_castle":
            level.interface_crafts = Array("part_body", "part_guards", "part_handle", "dolly", "door", "clamp");
            break;
        case "zm_island":
            level.interface_crafts = Array("part_visor", "part_filter", "part_strap", "dolly", "door", "clamp");
            break;
        case "zm_stalingrad":
            level.interface_crafts = Array("part_codes", "part_transmitter", "part_map", "dolly", "door", "clamp");
            break;
        case "zm_genesis":
            level.interface_crafts = Array("dolly", "door", "clamp");
            break;
        case "zm_tomb":
            level.interface_crafts = Array("gem", "upper_staff", "middle_staff", "lower_staff", "vinyl_player", "vinyl_master", "vinyl_air", "vinyl_ice", "vinyl_fire", "vinyl_elec", "body", "brain", "engine", "dolly", "door", "clamp");
            break;
    }
}

PickupAllPieces()
{
    if(!level.tree_names.size || level.craftables_picked_up)
    {
        thread WriteToScreen("No Craftables Available");
        return;
    }
    else thread WriteToScreen("Picking Up All Craftables");
    level.craftables_picked_up = 1;
    foreach(craft in level.zombie_include_craftables)
    {
        foreach(piece in craft.a_piecestubs)
        {
            skip = 0;
            switch(level.script)
            {
                case "zm_zod":
                    if(IsSubStr(piece.pieceName, "relic"))
                    {
                        self thread FinishRitual(GetNumFromName(piece.pieceName));
                        skip = 1;
                    }
                    if(IsSubStr(piece.pieceName, "memento"))
                    {
                        self thread ActivateMemento(piece, piece.pieceName);
                        skip = 1;
                    }
                    break;
                case "zm_castle":
                    if(piece.pieceName == "part_body") level flag::set("gravityspike_part_body_found");
                    if(piece.pieceName == "part_guards") level flag::set("gravityspike_part_guards_found");
                    if(piece.pieceName == "part_handle") level flag::set("gravityspike_part_handle_found");
                    break;
                case "zm_tomb":
                    if(piece.pieceName == "gem")
                    {
                        self thread PickupStaffGem(piece);
                        skip = 1;
                    }
                    break;
            }
            if(skip || piece.craftableName == "second_idgun") continue;
            if(IsDefined(piece.pieceSpawn)) self thread zm_craftables::player_take_piece(piece.pieceSpawn);
        }
    }
    switch(level.script)
    {
        case "zm_zod":
            self PickupKey();
            self PickupSprayer();
            break;
        case "zm_island":
            self PickupBucket();
            self PickupMachineTools();
            self PickupWWParts();
            break;
        case "zm_genesis":
            self PickupKeeperParts();
            break;
        case "zm_tomb":
            self PickupShovel();
            break;
    }
}

ProcessPiecePickup(num, add_num = 0, pickup_all_tree = 0)
{
    if(num >= level.interface_crafts.size + add_num)
    {
        self thread PickupAlternatePiece(num - level.interface_crafts.size);
        return;
    }
    craft_name = level.names_index[num];
    piece_name = level.tree_names[craft_name][add_num];
    if(!IsDefined(craft_name) || !IsDefined(piece_name))
    {
        thread WriteToScreen("Craftable Not Found");
        return;
    }
    if(pickup_all_tree)
    {
        thread WriteToScreen("Picking Up All Parts For: " + craft_name);
        foreach(piece in level.tree_names[craft_name])
        {
            if(!IsDefined(level.craft_names_used[craft_name])) level.craft_names_used[craft_name] = Array(piece);
            else ArrayInsert(level.craft_names_used[craft_name], piece, level.craft_names_used[craft_name].size);
            self thread PickupPieceFromNames(craft_name, piece);
        }
        return;
    }
    if(IsInArray(level.craft_names_used[craft_name], piece_name) || level.craftables_picked_up)
    {
        thread WriteToScreen("Part Already Picked Up");
        return;
    }
    if(!IsDefined(level.craft_names_used[craft_name])) level.craft_names_used[craft_name] = Array(piece_name);
    else ArrayInsert(level.craft_names_used[craft_name], piece_name, level.craft_names_used[craft_name].size);
    self thread PickupPieceFromNames(craft_name, piece_name);
}

PickupSinglePiece(num, param_num = undefined)
{
    if(num >= level.tree_names.size || num < 0)
    {
        thread WriteToScreen("Invalid Input");
        return;
    }
    craft_name = level.names_index[num];
    if(IsDefined(param_num) && IsArray(level.tree_names[craft_name])) piece_name = level.tree_names[craft_name][param_num];
    else piece_name = level.tree_names[craft_name];
    if(!IsDefined(craft_name) || !IsDefined(piece_name))
    {
        thread WriteToScreen("Craftable Not Found");
        return;
    }
    self thread PickupPieceFromNames(craft_name, piece_name);
}

PickupPieceFromNames(craft_name, piece_name)
{
    craft = level.zombie_include_craftables[craft_name];
    foreach(piece in craft.a_piecestubs)
    {
        if(piece.pieceName == piece_name)
        {
            switch(level.script)
            {
                case "zm_zod":
                    if(IsSubStr(piece.pieceName, "relic"))
                    {
                        self thread FinishRitual(GetNumFromName(piece.pieceName));
                        return;
                    }
                    else if(IsSubStr(piece.pieceName, "memento"))
                    {
                        self thread ActivateMemento(piece, piece.pieceName);
                        return;
                    }
                    break;
                case "zm_castle":
                    if(piece.pieceName == "part_body") level flag::set("gravityspike_part_body_found");
                    if(piece.pieceName == "part_guards") level flag::set("gravityspike_part_guards_found");
                    if(piece.pieceName == "part_handle") level flag::set("gravityspike_part_handle_found");
                    break;
                case "zm_tomb":
                    if(piece.pieceName == "gem")
                    {
                        self thread PickupStaffGem(piece);
                        return;
                    }
                    break;
            }
            thread WriteToScreen("Picking Up " + level.piece_names[piece_name]);
            if(IsDefined(piece.pieceSpawn)) self thread zm_craftables::player_take_piece(piece.pieceSpawn);
            return;
        }
    }
}

PickupAlternatePiece(num)
{
    if(IsInArray(level.craft_nums_used, num))
    {
        thread WriteToScreen("Part Already Picked Up");
        return;
    }
    level.craft_nums_used[level.craft_nums_used.size] = num;
    parts = GetArrayKeys(level.alternate_names);
    if(!IsDefined(level.alternate_names[parts[num]]))
    {
        thread WriteToScreen("Pickup Function Not Found");
        return;
    }
    island_parts = Array("ww1_found", "ww2_found", "ww3_found", "wwup1_found", "wwup2_found", "wwup3_found");
    switch(level.script)
    {
        case "zm_zod":
            self thread [[level.alternate_names[parts[num]]]]();
            break;
        case "zm_island":
            if(parts[num] == "bucket") self thread [[level.alternate_names[parts[num]]]]();
            else self thread [[level.alternate_names[parts[num]]]](parts[num]);
            break;
        case "zm_genesis":
            self thread [[level.alternate_names[parts[num]]]](parts[num]);
            break;
        case "zm_tomb":
            self thread [[level.alternate_names[parts[num]]]]();
            break;
    }
}