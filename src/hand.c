
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
 * however if there is an ace then we should recurse and account for both values
 * 
 * for a hand containing n_aces the maximum number of different scores is 2^(n_aces) 
 */
int score_hand(hand *h)
{
  int *score_list;
  int n_aces = h->n_aces;
  int n_slots = (n_aces == 0) ? 1 : (2 <<(n_aces-1));
  int i;

  score_list = malloc_checked(sizeof(int)*n_slots);

  for(i = 0; i < n_slots; i++){
    score_list[i] = 0;
  }
  
  score_helper(h, score_list, 0);

  /* want to either pick the highest non bust score,
   * or the lowest low score */
  
  free(score_list);
  return(h->score);
}

/**
 * iterate through the hand list, if the card is a regular card just increment 
 * the score, otherwise branch and fill out a new list element when we come to an ace */
void score_helper(hand *h, int *score_list_ptr, int hand_index)
{
  int card = h->cards[hand_index];
  int val = get_value(card);
  int temp_score;
  int temp_index;
  if(val > 0){
    *score_list_ptr += val;
    if(hand_index < h->n_cards){
      score_helper(h, score_list_ptr, hand_index++);
    }
  } else { /* it's an ace */
    temp_score = *score_list_ptr; /* save the old score */
    temp_index = hand_index; /* save the old index */

    *score_list_ptr += 1; /* ace low */
    if(hand_index < h->n_cards){
      /* running this branch will modify hand index*/
      score_helper(h, score_list_ptr, hand_index++);
    }
    
    score_list_ptr++; /* next slot in the list */
    *score_list_ptr += 11 + temp_score; /* add the old score and the high value */
    if(temp_index < h->n_cards){
      score_helper(h, score_list_ptr, temp_index++);
    }

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
