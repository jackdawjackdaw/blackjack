#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>



#include "useful.h"
#include "cards.h"
#include "deck.h"
#include "hand.h"
#include "player-actions.h"

#include "game.h"

/**
 * ccs, chris.colemansmith@gmail.com
 * 20.07.2014
 * 
 * very basic blackjack game implementation 
 */

void  INThandler(int sig);

/* ooh globals */
deck *d;
int global_card_count;


/* A typical card counting system assigns a point score to each rank of card (e.g., 1 point for 2–6, 0 points
   for 7–9 and −1 point for 10–A). Whenever a card is exposed, a counter adds the score of that card to a
   running total, the 'count'; the count is used to make betting and playing decisions according to a table
   which they have learned. The count starts at 0 for a freshly shuffled deck for "balanced" counting
   systems. Unbalanced counts are often started at a value which depends on the number of decks used in the
   game.
*/


int main (int argc, char* argv[]){

  int n_packs = 1; /* this should be an argument */
  int temp = 0;
  int player_pot = 100; /* how many bits the player starts with */
  int win_score = 0;
  global_card_count = 0;

  signal(SIGINT, INThandler);
  
  if(argc > 1){
    temp = strtol(argv[1], NULL, 10);
    if(temp > 0 && temp <= 8){
      n_packs = temp;
      printf("# playing with %d packs of cards in the deck\n", n_packs);
    }
  }
      
  d = get_deck(n_packs);
  /* set the seed here */
  srandom(get_seed_noblock()); /* set the seed using the non blocking interface*/
  shuffle_deck(d);

    while(1){
    win_score += play_round(d, &player_pot);
    printf("pot: %d nwins: %d count: %d\n", player_pot, win_score, global_card_count);
    if(player_pot < 0){
      printf("game over: you've gone broke!\n");
      break;
    }
  }
  free_deck(d);
  
  return(EXIT_SUCCESS);
}

/* try and catch ctrl-c and free remaining junk */
void  INThandler(int sig)
{
     char  c;
     signal(sig, SIG_IGN);
     printf("\nDo you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y'){
       /* clear up allocated memory */
       free_deck(d);
       exit(EXIT_SUCCESS);
     } else
       signal(SIGINT, INThandler);
     getchar(); // Get new line character
}

  

/**
 *  play a round/hand of blackjack
 *
 *  what happens if the deck is empty, or close to empty?
 */
int play_round(deck *d, int* player_pot)
{
  assert(*player_pot > 0);

  char buffer[256];
  int retval = 0;
  
  hand *dealer = get_hand();
  hand *player = get_hand();

  /** player should place initial bet first  */
  int player_bet = get_player_bet(*player_pot);
  assert(player_bet > 0);

  
  draw_cards_to_hand(2, d, player);
  /* show the player */
  strcpy(buffer, "[player-init] "); 
  hand_to_string(player, buffer);
  output_message(buffer);
  
  draw_cards_to_hand(2, d, dealer);
  /* should probably only show the player the first one */
  strcpy(buffer, "[dealer-init] ");
  hand_to_string(dealer, buffer);
  output_message(buffer);

  /* should check both hands for blackjack */
  if( hand_is_blackjack(player)
      && !hand_is_blackjack(dealer)){
    player_wins_blackjack( player_pot, player_bet);
    retval = 1;
    goto END;
  } else if( hand_is_blackjack(dealer) &&
             !hand_is_blackjack(player)){
    strcpy(buffer, "[dealer] BLACKJACK!\n");
    output_message(buffer);
    player_lose_simple(player_pot, player_bet);
    retval = -1;
    goto END;
  } else if( (hand_is_blackjack(dealer))
             && (hand_is_blackjack(player) )){
    hand_push(player_pot);
    retval = 0;
    goto END;    
  }

  
  /* suppose we loop through the player actions here */
  /* this can change their bet and of course their hand */
  /* this will drop back if the player goes bust during their actions */
  do_player_actions(d, player, &player_bet);

  /* dealer isn't bust, check player, they may have gone bust in the do_player_actions loop */
  if(player->score >21 && player->low_score > 21){
    sprintf(buffer, "# you are bust!\n");
    output_message(buffer);
    player_lose_simple(player_pot, player_bet);
    
    retval = -1;
    goto END;    
  }

  output_message("[dealer] drawing up\n");
  
  /* draw up for the dealer */
  while(dealer->score < 17 /* high score is ok */
        || (dealer->score > 21 && dealer->low_score < 17)) /* or high score is bust but low score is ok*/
    {
    /* draw a card for the dealer */
    draw_cards_to_hand(1, d, dealer);
    score_hand(dealer);

    strcpy(buffer, "[dealer] ");
    hand_to_string(dealer, buffer);
    output_message(buffer);

    /* check that the dealer isn't actually bust */
    if(dealer->score > 21 && dealer->low_score > 21){
      sprintf(buffer, "# dealer is bust!\n");
      output_message(buffer);
      player_wins_simple(player_pot, player_bet);

      retval = 1;
      goto END;
    }
  }    

  strcpy(buffer, "[dealer-final] ");
  hand_to_string(dealer, buffer);
  output_message(buffer);

  strcpy(buffer, "[player-final] ");
  hand_to_string(player, buffer);
  output_message(buffer);
  
  /* ok now assert that both hands are not bust */
  
  if(player->score > 21)
    player->score = player->low_score; 
  if(dealer->score > 21)
    dealer->score = dealer->low_score;
  
  /* check for a simple player win */
  if(player->score > dealer->score ){
    player_wins_simple(player_pot, player_bet);
    retval =1;
    goto END;
  }
  else if(player->score == dealer->score){
    hand_push(player_pot);
    retval = 0;
    goto END;
  } else {
    player_lose_simple(player_pot, player_bet);
    retval = -1;
    goto END;
  }
  
  /* feels transgressive to use goto, saves a bunch of stupid repetition */
 END:
  free_hand(dealer);
  free_hand(player);
  return(retval); 
}

/* basic win loss fns */
/* output how much you won and change the pot */
/* pays out 1:1 */
void player_wins_simple(int * player_pot, int player_bet)
{
  char buffer[256];
  int win = 2*player_bet;
  *player_pot += win;
  sprintf(buffer, "# you win %d. (pot %d)\n", win, *player_pot);
  output_message(buffer);
}

/* should pay out 3:2 */
void player_wins_blackjack(int * player_pot, int player_bet)
{
  char buffer[256];
  /* well this is a bit crappy if you win this you won't actually win your full payout */
  int win = (int)(2.5*(float)player_bet);
  *player_pot += win;
  sprintf(buffer, "# BLACKJACK! you win %d. (pot %d)\n", win, *player_pot);
  output_message(buffer);
}


void player_lose_simple(int * player_pot, int player_bet)
{
  char buffer[256];
  int lost = player_bet;
  *player_pot -= lost;
  sprintf(buffer, "# you lost %d. (pot %d)\n", lost, *player_pot);
  output_message(buffer);
}


void hand_push(int *player_pot)
{
  char buffer[256];
  sprintf(buffer, "# it's a draw (pot %d)\n", *player_pot);
  output_message(buffer);
}


  
void draw_cards_to_hand(int n_cards, deck *d, hand* h)
{
  int temp_card;
  int count = 0;

  while (count <n_cards){
    temp_card = draw_card(d);
    if(temp_card < 0){
      printf("# deck is empty reshuffling & resetting \n");
      reset_deck(d);
      /* reset counting and stats here */
      global_card_count = 0;
    } else {
      /* push the card into the hand */
      if(add_card_to_hand(h, temp_card) < 0 ){
         /* yeah should check that this actually works */
        fprintf(stderr, "# err, too many cards (%d) in hand\n", h->n_cards);
        exit(EXIT_FAILURE);
      }
      /* ok now we can increment the global card counting score */
      global_card_count += get_card_count_value(temp_card);
      count++;
    }
  }
  score_hand(h);
}

/* stubs, could be replaced with nicer io */
void output_message(char *msg)
{
  printf("%s", msg);
}

void output_error(char* msg)
{
  fprintf(stderr, "##err##: %s\n", msg);
}
