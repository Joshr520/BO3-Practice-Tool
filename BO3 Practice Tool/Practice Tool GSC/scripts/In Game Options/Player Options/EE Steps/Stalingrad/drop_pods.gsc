BuildDragonride()
{
    self CollectCraftible("dragonride", "all");
    level flag::wait_till("power_on");
    wait 0.5;
    stub = zm_craftables::find_craftable_stub("dragonride");
    self zm_craftables::player_craft(stub.craftablespawn);
    thread zm_unitrigger::unregister_unitrigger(stub);
    stub [[ stub.craftablestub.onfullycrafted ]]();
}