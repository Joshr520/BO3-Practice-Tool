autoexec InitBGBPresets()
{
    bgb_pack = [];
    line = compiler::getline("Practice Tool/BGB Preset.txt", true);
	while (line != "n/a" && line != "end") {
        bgb_pack[bgb_pack.size] = line;
        line = compiler::getline("Practice Tool/BGB Preset.txt", false);
    }

    if (bgb_pack.size != 5) {
        if (bgb_pack.size > 0) {
            DebugPrint("error", "BGB preset doesn't contain 5 gums");
        }
        return;
    }
}

detour bgb<scripts\zm\_zm_bgb.gsc>::bgb_end_game()
{
}