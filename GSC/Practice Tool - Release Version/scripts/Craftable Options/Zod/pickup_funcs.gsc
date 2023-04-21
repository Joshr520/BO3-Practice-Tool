GetNumFromName(name)
{
    if(IsSubStr(name, "magician")) return 0;
    if(IsSubStr(name, "femme")) return 1;
    if(IsSubStr(name, "detective")) return 2;
    if(IsSubStr(name, "boxer")) return 3;
}

ActivateMemento(piece, str)
{
    switch(str)
    {
        case "memento_magician":
            if(!level flag::get("power_on" + 20))
            {
                thread WriteToScreen("Activating Fountain Pen");
                self ActivatePen();
            }
            wait(10);
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Golden Fountain Pen");
            self thread zm_craftables::player_get_craftable_piece(piece.craftablename, piece.pieceName);
            break;
        case "memento_detective":
            if(!level flag::get("power_on" + 23) || IsDefined(level.zod_smashables["memento_detective_drop"].m_a_clip[0]))
            {
                thread WriteToScreen("Activating Badge");
                self ActivateBadge();
            }
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Badge");
            self thread zm_craftables::player_get_craftable_piece(piece.craftablename, piece.pieceName);
            break;
        case "memento_femme":
            self ActivateHairpiece();
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Hairpiece");
            self thread zm_craftables::player_get_craftable_piece(piece.craftablename, piece.pieceName);
            break;
        case "memento_boxer":
            self ActivateBelt();
            if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Championship Belt");
            self thread zm_craftables::player_get_craftable_piece(piece.craftablename, piece.pieceName);
            break;
    }
}

ActivatePen()
{
    level flag::set("power_on" + 20);
}

ActivateBadge()
{
	level flag::set("power_on" + 23);
    if(IsDefined(level.zod_smashables["memento_detective_drop"].m_a_clip[0])) level.zod_smashables["memento_detective_drop"].m_e_trigger UseBy(self);
    wait 1;
}

ActivateHairpiece()
{
    if(!IsDefined(level.zod_smashables["memento_femme_drop"].m_a_clip[0])) return;
    thread WriteToScreen("Activating Hairpiece");
    level.zod_smashables["memento_femme_drop"].m_e_trigger UseBy(self);
    wait 2.75;
}

ActivateBelt()
{
    if(!IsDefined(level.zod_smashables["memento_boxer_drop"].m_a_clip[0])) return;
    thread WriteToScreen("Activating Belt");
    level.zod_smashables["memento_boxer_drop"].m_e_trigger UseBy(self);
    wait 3;
}