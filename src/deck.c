
#include "cards.h"
#include "deck.h"

#include <stdlib.h>
#include <stdio.h>


/**
 * a deck is an array based stack of cards (or i guess a linked list would work)
 * it contains at least one pack of cards (0..51), but typically for a real game there will be more
 *
 * decks can be shuffled and cards can be drawn from the top
 * 
 */

/** 
 * allocates and returns a shuffled deck of cards, the argument sets how many packs (0..51) of cards there are
 * in the deck.
 */ 
deck* get_deck(int n_packs)
{
  int i;
  deck *d;
  if( (d = malloc(sizeof(deck))) == NULL){
  perror("malloc deck failed"); exit(2);
  }
  
  
  d->n_cards = NSUITS*NTYPES*n_packs;
  d->cards = NULL;
  
  if( (d->cards = malloc(sizeof(int)*d->n_cards)) == NULL){
      perror("malloc cards failed"); exit(2);
  }

  d->top_card = 0; /* top is the first one */

  /** fill the deck in a totally un shuffled order */
  for(i = 0; i < d->n_cards; i++){
   d->cards[i] = (i % (NSUITS*NTYPES));
  }

  /* now shuffle it */
  //shuffle_deck(d);
  
  return(d);  
}

void free_deck(deck* d)
{
  free(d->cards);
  free(d);
}

/* sets top_card back to zero and then shuffles the whole deck
 */ 
void reset_deck(deck *d)
{
  d->top_card = 0;
  shuffle_deck(d);
}


/*
 * shuffle the cards in the deck from top_card -> n_cards
 * fisher yaaaaaaates
 *
 * \fix replace the rng with a better one
 *  
 * To shuffle an array a of n elements (indices 0..n-1):
 * for i from n − 1 downto 1 do
 *      j ← random integer with 0 ≤ j ≤ i
 *     exchange a[j] and a[i]
 *
 */
void shuffle_deck(deck *d)
{
  int i, j;
  int nshuffle = n_cards_remaining(d);
  int temp;
  for(i = d->n_cards - 1; i > d->top_card; i--){
    j = ((int)random()) % (nshuffle-i) + d->top_card;

    //printf("%d %d %d\n", i, j, ((int)random()) % (nshuffle-i)); 
    /* swappo */
    temp = d->cards[i];
    d->cards[i] = d->cards[j];
    d->cards[j] = temp;
  }
}


/*
 * returns the top card in the deck and increments the top card counter
 * returns -1 if the deck is empty
 */
int draw_card(deck *d)
{
  int retval;
  if(d->top_card < d->n_cards){
    retval = d->cards[d->top_card];
    d->top_card++;
    return(retval);
  } else {
    /* maybe log this? */
    return -1;
  }
}

int n_cards_remaining(deck *d)
{
return (d ->n_cards - d->top_card);
}


/* prints the deck from the current top card */
void print_deck(deck *d)
{
  int i;
  char buffer[4];
  for(i = d->top_card; i < d->n_cards; i++){
    card_to_str_short(d->cards[i], buffer);
    printf("%d %s\n", i, buffer);
  }
}  
  
