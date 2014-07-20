#ifndef _INC_HAND_
#define _INC_HAND_

/** 
 * how many cards can you possibly hold before going bust?
 * well this somewhat depends on how many packs are in the deck, for a single
 * pack this would be 12: 4 aces (4), 4 twos (8), 3 threes (9), for a perfect 21
 *
 * don't you win if you manage to get a 5 card hand?
 */ 
#define MAXCARDSHAND 16 

typedef struct hand{
  int n_cards;
  int n_aces; 
  /**
   * This could be replaced with a linked list, to support more generic games
   */
  int cards[MAXCARDSHAND];
  int score; /* how much the current hand is worth */
}hand;

hand* get_hand(void);
void free_hand(hand* h);

void reset_hand(hand* h);

int score_hand(hand* h); 
void score_helper(hand* h, int n_slots, int *score_list);

void print_hand(hand* h);

int add_card_to_hand(hand* h, int card);



#endif
