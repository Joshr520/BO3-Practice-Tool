PickupShovel()
{
    if(!level.craftables_picked_up) thread WriteToScreen("Picking Up Shovel");
    self.dig_vars["has_shovel"] = 1;
    level clientfield::set(("player" + self GetEntityNumber()) + "hasItem", 1);
}