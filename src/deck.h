#ifndef _INC_DECK_
#define _INC_DECK_


typedef struct deck{
  int * cards;
  int top_card;
  int n_cards;
} deck;

deck* get_deck(int n_packs);
void free_deck(deck* d);
void reset_deck(deck *d);
void shuffle_deck(deck* d);
int draw_card(deck* d);
int n_cards_remaining(deck *d);
void print_deck(deck* d);

#endif
