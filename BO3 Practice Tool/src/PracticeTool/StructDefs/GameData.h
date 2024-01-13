#pragma once

#include <string>
#include <vector>

namespace BO3PracticeTool
{
	inline std::vector<std::string> t7Maps = { "Shadows of Evil", "The Giant", "Der Eisendrache", "Zetsubou No Shima", "Gorod Krovi",
		"Revelations", "Nacht Der Untoten", "Verruckt", "Shi No Numa", "Kino Der Toten", "Ascension", "Shangri-La", "Moon", "Origins" };
	inline std::vector<std::string> t7MapsInternal = { "zm_zod", "zm_factory", "zm_castle", "zm_island", "zm_stalingrad", "zm_genesis",
		"zm_prototype", "zm_asylum", "zm_sumpf", "zm_theater", "zm_cosmodrome", "zm_temple", "zm_moon", "zm_tomb" };

	struct Perk {
		std::string Name;
		std::string InternalName;
	};

	inline Perk jug = { "Juggernog", "specialty_armorvest"};
	inline Perk speed = { "Speed Cola", "specialty_fastreload" };
	inline Perk dtap = { "Double Tap", "specialty_doubletap2" };
	inline Perk qr = { "Quick Revive", "specialty_quickrevive" };
	inline Perk stam = { "Stamin-Up", "specialty_staminup" };
	inline Perk mkick = { "Mule Kick", "specialty_additionalprimaryweapon" };
	inline Perk wwine = { "Widow's Wine", "specialty_widowswine" };
	inline Perk deadshot = { "Deadshot Daiquiri", "specialty_deadshot" };
	inline Perk echerry = { "Electric Cherry", "specialty_electriccherry" };

	inline std::unordered_map<std::string, std::vector<Perk>> perks = {
		{ "zm_zod", {
			{ jug, speed, dtap, qr, stam, mkick, wwine }
		}},
		{ "zm_factory", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot }
		}},
		{ "zm_castle", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine, echerry }
		}},
		{ "zm_island", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine, echerry }
		}},
		{ "zm_stalingrad", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine, echerry }
		}},
		{ "zm_genesis", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine, echerry }
		}},
		{ "zm_prototype", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_asylum", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_sumpf", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_theater", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_cosmodrome", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_temple", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_moon", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine }
		}},
		{ "zm_tomb", {
			{ jug, speed, dtap, qr, stam, mkick, deadshot, wwine, echerry }
		}}
	};
}