BuildLightningStaff()
{
    self thread PickupSinglePiece(5, 0);
    self thread PickupSinglePiece(5, 1);
    self thread PickupSinglePiece(5, 2);
    self thread PickupSinglePiece(5, 3);
    wait 2;
    self thread PickupSinglePiece(2, 5);
    foreach(stub in level.a_uts_craftables)
	{
        if(!IsDefined(stub) || !IsDefined(stub.equipname)) continue;
        if(stub.equipname == "elemental_staff_lightning")
        {
            stub.usetime = 0;
            self BuildAndActivateTrigger(stub);
            break;
        }
    }
    wait 3;
}

CompleteLightningChords()
{
    chord_order = Array("a_minor", "e_minor", "d_minor");
    foreach(chord_name in chord_order)
    {
        chord = struct::get("piano_chord_" + chord_name, "script_noteworthy");
        level.a_piano_keys_playing = chord.notes;
        level notify("piano_key_played");
        level flag::wait_till("piano_chord_ringing");
        level flag::wait_till_clear("piano_chord_ringing");
        wait 1;
    }
}

CompleteLightningRelays()
{
    level flag::set("electric_puzzle_2_complete");
}