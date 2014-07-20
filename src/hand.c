
#include "hand.h"
#include "cards.h"
#include "useful.h"

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
  hand* h = (hand*)malloc_checked(sizeof(hand));
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
  h->low_score = 0;
  h->n_aces = 0;
  for(i = 0; i < MAXCARDSHAND; i++){
    h->cards[i] = -2; /* not a card, however zero is a card */
  }
}

/**
 * a one line print out of the hand */
void print_hand(hand* h)
{
  int i;
  char buffer[4];

  if(h->n_cards == 0){
    printf("# empty hand\n");
  } else {
    printf("# hand: ( ");
    for(i = 0; i < h->n_cards; i++){
      card_to_str_short(h->cards[i], buffer);
      printf("%s ", buffer);
    }
    printf(") score-hi: %d score-lo: %d\n", h->score, h->low_score);
  }
}

/** 
 * scores the cards in a hand,
 * updates h->score and also returns the score
 * 
 * we can get the value of the cards in the hand from cards.c:get_value(card)
 * however if there is an ace then we should recurse and account for both values
 * 
 * for a hand containing n_aces the maximum number of different high-low choices is bounded from above by
 * 2^(n_aces)
 *
 * we could set a high and low score?
 */
int score_hand(hand *h)
{
  int *score_list;
  int n_aces = h->n_aces;
  int n_slots = (n_aces == 0) ? 1 : (2 <<(n_aces-1));
  int i;

  printf("n_slots %d\n", n_slots);
  
  if(h->n_cards == 0){
    return(0);
  }
  
  score_list = malloc_checked(sizeof(int)*n_slots);

  for(i = 0; i < n_slots; i++)
    score_list[i] = 0;
  
  score_helper(h, n_slots, score_list);

  /* want to either pick the highest non bust score,
   * or the lowest low score */

  if(n_slots == 1){
    h->score = score_list[0];
    h->low_score = h->score;
  } else {
    for(i = 0; i < n_slots; i++){
      printf("%d ", score_list[i]);
      /* sets score to the highest non bust value  */
      if(score_list[i] > h->score && score_list[i] <= 21)
        h->score = score_list[i];
    }
    printf("\n");

    /* this one is always the lowest */
    h->low_score = score_list[n_slots-1];
  }
  
  free(score_list);
  return(h->score);
}

/* computes the list of scores,
 * 
 * first makes a list of the values of the current hand,
 * then iterates through this list n_slots times,
 * on each pass 
 * the score is computed with negative values (aces) being treated as high (i.e -1)
 * on each pass the value of the first negative ace (high) is flipped from high to low i.e from -1 to 1.
 */
void score_helper(hand* h, int n_slots, int *score_list)
{
  int i,j;
  int flip_flag ;
  int *val_list = malloc_checked(sizeof(int)*h->n_cards);
  int temp_score ;
  for(i = 0; i < h->n_cards; i++)
    val_list[i] = get_value(h->cards[i]);

  for(i = 0; i < n_slots; i++){
    temp_score = 0;
    flip_flag = 0;
    for(j = 0; j < h->n_cards; j++){
      if(val_list[j] > 0){ 
        temp_score += val_list[j];
      } else {
        temp_score += 11; /* treat the -1 as an ace */
        if(flip_flag == 0){
          val_list[j] *= -1; /* change the sign */
          flip_flag = 1;
        } 
      }
    }
    score_list[i] = temp_score;
  }
  
  free(val_list);
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
    /* disabled until this is debugged */
    //score_hand(h);
    return(card); 
  } else {
    /* cant add a card to the hand */
    fprintf(stderr, "# can't add card to hand, it is already full\n");
    return -1;
  }
}
