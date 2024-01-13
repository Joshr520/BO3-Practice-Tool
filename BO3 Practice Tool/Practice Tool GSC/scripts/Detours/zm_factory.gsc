// stop removal of perks from map
detour zm_factory<scripts\zm\zm_factory.gsc>::function_e0f73644()
{
	if (math::cointoss()) {
		level._custom_perks["specialty_deadshot"].perk_machine_power_override_thread = @zm_factory<scripts\zm\zm_factory.gsc>::function_49e223a9;
	}
	else {
		level._custom_perks["specialty_staminup"].perk_machine_power_override_thread = @zm_factory<scripts\zm\zm_factory.gsc>::function_6000324c;
	}
}