#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "useful.h"
#include "cards.h"
#include "deck.h"
#include "hand.h"

#include "game.h"

/**
 * ccs, chris.colemansmith@gmail.com
 * 20.07.2014
 * 
 * very basic blackjack game implementation 
 */

int main (int argc, char* argv[]){

  int n_packs = 1; /* this should be an argument */
  int player_pot = 100; /* how many bits the player starts with */
  int win_score = 0;
  
  deck * d = get_deck(n_packs);
  /* set the seed here */
  srandom(get_seed_noblock()); /* set the seed using the non blocking interface*/
  //srandom(1);
  shuffle_deck(d);
  
  win_score += play_round(d, &player_pot);
  printf("# final pot %d\tnwins %d\n", player_pot, win_score);
  
  free_deck(d);
  
  return(EXIT_SUCCESS);
}

/* stub */
int get_player_bet(int player_pot)
{
  char input[256];
  char buffer[256];
  char err_buffer[256];
  int betval = 1;
  int bet_input = 0;
  int valid_flag = 0;


  sprintf(buffer, "[player] (pot %d) bet amount? ", player_pot);
  
  do{
    output_message(buffer);    
    fgets(input, sizeof(input), stdin);
    //printf("# input read [%s]\n", input);
    if(sscanf(input, "%d", &bet_input) > 0){
      betval = bet_input;
      if(betval > 0 && betval <= player_pot){
        valid_flag = 1; 
      } else {
        sprintf(err_buffer, "enter a number in range [1,%d]", player_pot);
        output_error(err_buffer);
      }
    } else {
      sprintf(err_buffer, "enter a number in range [1,%d]", player_pot);
      output_error(err_buffer);
    }
  } while (valid_flag == 0);
  
  return betval;
}


void do_player_actions(deck *d, hand* h, int* player_bet){
  /* do nothing */
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
  if( (hand_is_blackjack(player) == 0)
      && (hand_is_blackjack(dealer) != 0)){
    player_wins_blackjack( player_pot, player_bet);
    retval = 1;
    goto END;
  } else if( (hand_is_blackjack(dealer) == 0) &&
             (hand_is_blackjack(player) != 0)){
    strcpy(buffer, "[dealer] BLACKJACK!\n");
    output_message(buffer);
    player_lose_simple(player_pot, player_bet);
    retval = -1;
    goto END;
  } else if( (hand_is_blackjack(dealer)== 0)
             && (hand_is_blackjack(player) == 0)){
    hand_push(player_pot);
    retval = 0;
    goto END;    
  }

  output_message("[player] what will you do?\n");
  
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
    } else {
      /* push the card into the hand */
      if(add_card_to_hand(h, temp_card) < 0 ){
         /* yeah should check that this actually works */
        fprintf(stderr, "# err, too many cards (%d) in hand\n", h->n_cards);
        exit(EXIT_FAILURE);
      }
      count++;
    }
  }
  score_hand(h);
}

/* stub */
void output_message(char *msg)
{
  printf("%s", msg);
}

void output_error(char* msg)
{
  fprintf(stderr, "##err##: %s\n", msg);
}
