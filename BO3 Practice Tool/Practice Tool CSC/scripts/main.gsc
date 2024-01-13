init(localclientnum)
{
}

on_localplayer_spawned(localclientnum)
{
    for (;;) {
        WaitF5();
        start = self.origin;
        while (start == self.origin) {
            wait 0.01;
        }
        time = GetServerTime(localclientnum);
        while (Distance2D(start, self.origin) < 5000) {
            wait 0.01;
        }
        IPrintLnBold(GetServerTime(localclientnum) - time);
    }

    if (GetDvarInt("sv_running", 0)) {
        return;
    }
    compiler::livesplit("reset");

    level waittill("livesplit_start");
    compiler::livesplit("start");

    for (;;) {
        level waittill("livesplit_split");
        compiler::livesplit("split");
    }
}

// detour load<scripts\zm\_load.csc>::main()
// {
//     compiler::nprintln("Hello from csc");
//     [[ @load<scripts\zm\_load.csc>::main ]]();
// }

WaitF5()
{
    while(!compiler::waitkey(0x74)) {
        wait 0.05;
    }
}

// DebugCompiler.exe "C:\Users\joshr\source\repos\BO3 Practice Tool\BO3 Practice Tool\Practice Tool CSC\compiled.gscc" T7 scripts\shared\weapons_shared.csc --inject --noupdate
// DebugCompiler.exe "C:\Users\joshr\source\repos\BO3 Practice Tool\BO3 Practice Tool\Practice Tool CSC\compiled.gsic" T7 scripts\shared\weapons_shared.csc --inject --noupdate