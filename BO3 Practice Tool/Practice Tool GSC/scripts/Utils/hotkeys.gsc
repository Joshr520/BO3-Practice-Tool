autoexec InitHotkeys()
{
    system::wait_till("all");
    
    hotkey_controls = hud::createserverfontstring("default", 1.25);
    hotkey_controls hud::setpoint("TOPLEFT", "TOPLEFT", 5, 5);
    hotkey_controls.alpha = 0;
    hotkey_controls SetText("Hide Keybinds: ^9F3\n");
    hotkey_controls thread MonitorHotkeyAlpha();

    level.hotkey_height = hotkey_controls.height;
    level.hotkeys = [];

    while (!level.hotkeys.size) {
        wait 0.05;
    }
    hotkey_controls.alpha = 1;
    thread HotkeyVisibility();
}

RegisterHotkey(key, label, text_or_value, func_on_trigger, callback_text = undefined, ent = level)
{
    if (IsDefined(level.hotkeys[key])) {
        DebugPrint("error", "Hotkey: " + key + " already defined");
        return;
    }

    level.hotkeys[key] = true;
    hotkey = SpawnStruct();
    hotkey.key = key;
    hotkey.func_on_trigger = func_on_trigger;
    if (IsDefined(callback_text)) {
        hotkey.callback_text = callback_text;
    }

    hotkey.hud_elem = hud::createserverfontstring("default", 1.25);
    hotkey.hud_elem hud::setpoint("TOPLEFT", "TOPLEFT", 5, 5 + level.hotkey_height);
    if (IsDefined(label)) {
        hotkey.hud_elem.label = IString(label);
        level.hotkey_height += hotkey.hud_elem.height;
    }
    if (IsDefined(text_or_value)) {
        if (IsInt(text_or_value)) {
            hotkey.hud_elem SetValue(text_or_value);
        }
        else {
            hotkey.hud_elem SetText(text_or_value);
        }
        level.hotkey_height += hotkey.hud_elem.height;
    } 

    ent thread RunHotkey(hotkey);
}

RunHotkey(hotkey)
{
    hotkey.hud_elem thread MonitorHotkeyAlpha();

    for (;;) {
        WaitForHotkey(hotkey.key);
        self [[ hotkey.func_on_trigger ]]();
        if (IsDefined(hotkey.callback_text)) {
            self [[ hotkey.callback_text ]](hotkey.hud_elem);
        }
    }
}

HotkeyVisibility()
{
    for (;;) {
        WaitF3();
        level notify("hotkey_alpha");
        WaitF3();
        level notify("hotkey_alpha");
    }
}

MonitorHotkeyAlpha()
{
    for (;;) {
        level waittill("hotkey_alpha", text);
        self.alpha = !self.alpha;
    }
}