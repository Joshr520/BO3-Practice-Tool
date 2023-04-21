GiveZodCharacter()
{
	if(IsDefined(level.hotjoin_player_setup) && [[level.hotjoin_player_setup]]("c_zom_farmgirl_viewhands"))
	{
		return;
	}
	self DetachAll();
	self.og_characterindex = self.characterindex;
	self.characterindex = 3;
	self.favorite_wall_weapons_list = [];
	self.talks_in_danger = 0;
	self SetCharacterBodyType(self.characterindex + 5);
	self SetCharacterBodyStyle(0);
	self SetCharacterHelmetStyle(0);
	self.favorite_wall_weapons_list[self.favorite_wall_weapons_list.size] = GetWeapon("hk416");
	self SetMoveSpeedScale(1);
	self SetSprintDuration(4);
	self SetSprintCooldown(0);
	self thread set_exert_id();
	self clientfield::set_player_uimodel("zmInventory.player_character_identity", self.characterindex);
}

GiveCastleCharacter()
{
	self DetachAll();
	self.characterindex = 2;
	self.favorite_wall_weapons_list = [];
	self.talks_in_danger = 0;
	self SetCharacterBodyType(self.characterindex);
	self SetCharacterBodyStyle(0);
	self SetCharacterHelmetStyle(0);
	self.talks_in_danger = 1;
	level.rich_sq_player = self;
	level.sndradioa = self;
	self.favorite_wall_weapons_list[self.favorite_wall_weapons_list.size] = GetWeapon("pistol_standard");
	self SetMoveSpeedScale(1);
	self SetSprintDuration(4);
	self SetSprintCooldown(0);
	self thread set_exert_id();
}

set_exert_id()
{
	self endon("disconnect");
	util::wait_network_frame();
	util::wait_network_frame();
	self zm_audio::setexertvoice(self.characterindex + 1);
}