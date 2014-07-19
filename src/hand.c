
#include "hand.h"
#include "cards.h"

#include <stdio.h>
#include <stdlib.h>

/** 
 * A hand is a set of cards held by a player (dealer, human players etc are not distinguished).
 * cards can be added and the score can be computed.
 * 
 * Scoring is made somewhat complicated by the dual role of the aces, Since they can be 
 * either high or low. This is probably where I should check the rules about how this works
 */


/**
 * allocate a hand, the cards are all set to -1, marking not a card */
hand* get_hand(void)
{
  hand* h = malloc_checked(sizeof(hand));
  reset_hand(h);
  return(h);
}

void free_hand(hand *h)
{
  free(h);
}


/**
 * clears a hand, so you can play again!
 */
void reset_hand(hand *h)
{
  int i;
  h->n_cards = 0;
  h->score = 0;
  h->n_aces = 0;
  for(i = 0; i < MAXCARDSHAND; i++){
    h->cards[i] = -1; /* not a card, however zero is a card */
  }
}

/**
 * a one line print out of the hand */
void print_hand(hand* h)
{
  int i;
  char buffer[4];
  printf("# hand: (");
  for(i = 0; i < h->n_cards; i++){
    card_to_str_short(h->cards[i], buffer);
    printf("%s ", buffer);
  }
  printf(") score: %d\n", h->score);
}

/** 
 * scores the cards in a hand,
 * updates h->score and also returns the score
 * 
 * we can get the value of the cards in the hand from cards.c:get_value(card)
 * however if there is an ace then we should branch and try both values
 * 
 * for a hand containing n_aces the maximum number of different scores is 2^(n_aces) 
 */
int score_hand(hand *h)
{
  int *score_list;
  int n_aces = h->n_aces;

  if(n_aces > 0){
    score_list = malloc_checked(sizeof(int) * (2<<(n_aces-1)));
  } else {
    score_list = malloc_checked(sizeof(int)); /* no list */
  }
}


/**
 * try and add the given card to the hand,
 * increments the number of cards in the hand and the number of aces (if you happen to add one)
 * 
 *
 * returns -1 on failure
 */ 
int add_card_to_hand(hand* h, int card)
{
  if(h->n_cards < MAXCARDSHAND){
    h->cards[h->n_cards] = card;
    h->n_cards++;

    if(get_type(card) == 0) /* it's an ace */
      h->n_aces++;

    /* update the score */
    score_hand(h);
    return(card); 
  } else {
    /* cant add a card to the hand */
    fprintf(stderr, "# can't add card to hand, it is already full\n");
    return -1;
  }
}
