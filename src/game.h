#ifndef _INC_GAME_
#define _INC_GAME_

int get_player_bet(int player_pot);
int play_round(deck *d, int* player_pot);
void draw_cards_to_hand(int n_cards, deck *d, hand* h);

void player_wins_simple(int * player_pot, int player_bet);
void player_wins_blackjack(int * player_pot, int player_bet);
void player_lose_simple(int * player_pot, int player_bet);
void hand_push(int *player_pot);


void do_player_actions(deck *d, hand* h, int * player_bet);

void output_message(char *msg);
void output_error(char* msg);

#endif
