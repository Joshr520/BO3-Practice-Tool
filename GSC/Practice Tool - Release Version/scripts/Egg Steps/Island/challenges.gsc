FinishChallenge(num)
{
    while(!self flag::exists("flag_player_collected_reward_1")) wait 0.05;
    switch(num)
    {
        case 1:
            for(i = 0; i < self._challenges.challenge_1.n_count; i++)
            {
                self notify(self._challenges.challenge_1.str_notify);
                wait 0.05;
            }
            break;
        case 2:
            for(i = 0; i < self._challenges.challenge_2.n_count; i++)
            {
                self notify(self._challenges.challenge_2.str_notify);
                wait 0.05;
            }
            break;
        case 3:
            for(i = 0; i < self._challenges.challenge_3.n_count; i++)
            {
                self notify(self._challenges.challenge_3.str_notify);
                wait 0.05;
            }
            break;
    }
}

FinishChallenges()
{
    self thread FinishChallenge(1);
    self thread FinishChallenge(2);
    self thread FinishChallenge(3);
}