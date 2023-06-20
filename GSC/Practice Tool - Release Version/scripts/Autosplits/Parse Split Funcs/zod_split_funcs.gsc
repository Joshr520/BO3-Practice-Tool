SplitMagicianRitual()
{
    self WaitRitual("magician");
}

SplitFemmeRitual()
{
    self WaitRitual("femme");
}

SplitDetectoveRitual()
{
    self WaitRitual("detective");
}

SplitBoxerRitual()
{
    self WaitRitual("boxer");
}

SplitPAPRitual()
{
    self WaitRitual("pap");
}

SplitCanalsRift()
{
    self WaitRiftTP("canal", "top");
}

SplitFootlightRift()
{
    self WaitRiftTP("theater", "top");
}

SplitWaterfrontRift()
{
    self WaitRiftTP("slums", "top");
}

SplitRiftCanals()
{
    self WaitRiftTP("canal", "bottom");
}

SplitRiftFootlight()
{
    self WaitRiftTP("theater", "bottom");
}

SplitRiftWaterfront()
{
    self WaitRiftTP("slums", "bottom");
}

SplitCanalsEgg()
{
    self WaitEgg(0);
}

SplitFootlightEgg()
{
    self WaitEgg(1);
}

SplitWaterfrontEgg()
{
    self WaitEgg(3);
}

SplitRiftEgg()
{
    self WaitEgg(4);
}

SplitPickupSword()
{
    self WaitSword(1);
}

SplitJunctionOvum()
{
    self WaitForOvum(3);
}

SplitCanalsOvum()
{
    self WaitForOvum(1);
}

SplitFootlightOvum()
{
    self WaitForOvum(2);
}

SplitWaterfrontOvum()
{
    self WaitForOvum(0);
}

SplitUpgradedSword()
{
    self WaitSword(2);
}

SplitBook()
{
    self WaitBook();
}

SplitFlag1()
{
    self WaitFlag(1);
}

SplitFlag2()
{
    self WaitFlag(2);
}

SplitFlag3()
{
    self WaitFlag(3);
}

SplitFlag4()
{
    self WaitFlag(4);
}