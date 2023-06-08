SplitGenSpawn()
{
    WaitGenOn(2);
}

SplitGenDE()
{
    WaitGenOn(3);
}

SplitGenVerruckt()
{
    WaitGenOn(4);
}

SplitGenMob()
{
    WaitGenOn(1);
}

SplitKeeperStart()
{
    WaitKeeperStart();
}

SplitEnterBeast()
{
    WaitBeastEnter();
}

SplitLeaveBeastMob()
{
    WaitBeastLeave(1);
}

SplitLeaveBeastVerruckt()
{
    WaitBeastLeave(4);
}

SplitLeaveBeastDE()
{
    WaitBeastLeave(3);
}

SplitLeaveBeastSpawn()
{
    WaitBeastLeave(2);
}

SplitReelPickup1()
{
    WaitReel(1, true);
}

SplitReelPlace1()
{
    WaitReel(1, false);
}

SplitReelPickup2()
{
    WaitReel(2, true);
}

SplitReelPlace2()
{
    WaitReel(2, false);
}

SplitReelPickup3()
{
    WaitReel(3, true);
}

SplitReelPlace3()
{
    WaitReel(3, false);
}

SplitHouseTP()
{
    WaitSophiaTP(0);
}

SplitEgg1()
{
    WaitEggFill(1);
}

SplitEgg2()
{
    WaitEggFill(2);
}

SplitEgg3()
{
    WaitEggFill(3);
}

SplitEgg4()
{
    WaitEggFill(4);
}

SplitRune1()
{
    WaitRunePickup(1);
}

SplitRune2()
{
    WaitRunePickup(2);
}

SplitRune3()
{
    WaitRunePickup(3);
}

SplitRune4()
{
    WaitRunePickup(4);
}

SplitBoss1()
{
    WaitBoss1();
}

SplitSymbols()
{
    WaitSymbols();
}

SplitBossRush()
{
    WaitBossRush();
}

SplitBoss2()
{
    WaitSophiaTP(1);
}