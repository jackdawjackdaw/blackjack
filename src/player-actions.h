#ifndef _INC_PLAYER_ACTIONS_
#define _INC_PLAYER_ACTIONS_

enum player_actions{
  stick,
  hit,
  doubleup
};

enum player_actions get_player_action();

int get_player_bet(int player_pot);
enum player_actions get_player_action();
void do_player_actions(deck *d, hand* h, int* player_bet);

void player_double(deck *d, hand *h, int *player_bet);
void player_hit(deck *d, hand *h, int *player_bet);
void player_stick(deck *d, hand *h, int *player_bet);


#endif
