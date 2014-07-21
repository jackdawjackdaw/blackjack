
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "useful.h"
#include "cards.h"
#include "deck.h"
#include "hand.h"

#include "player-actions.h"

#include "game.h"


/**
 * tries to read player input (forever) until
 * defaults to 1.
 * you finally get something valid */
int get_player_bet(int player_pot)
{
  char input[256];
  char buffer[256];
  char err_buffer[256];
  int betval_default = 1;
  int betval = 1;
  int bet_input = 0;
  int valid_flag = 0;


  sprintf(buffer, "[player] (pot %d) bet amount? (%d) ", player_pot, betval_default);
  
  do{
    output_message(buffer);    
    fgets(input, sizeof(input), stdin);
    
    if(input[0] == '\n'){
      /* have to have an explicit default incase you get say multiple bad inputs and then 
       * a default choice*/
      betval = betval_default;
      break;
    }
    
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


extern int global_card_count;

/**
 * poll player until you get a valid input, either
 * nothing -> defaults to stick
 * h<whatever> -> hit
 * s<whatever> -> stick
 * d<whatever> -> double
 */
enum player_actions get_player_action()
{
  char input[256];
  char instr[256];
  char buffer[256];  

  while(1){
    sprintf(buffer, "[player] (h)it, (s)tick, (d)ouble? (count: %d) ", global_card_count);
//output_message("[player] (h)it, (s)tick, (d)ouble? ");
    output_message(buffer);
    fgets(input, sizeof(input), stdin);

    sscanf(input, "%s", instr);
    printf("# instr %s\n", instr);
    
    switch(instr[0]){
    case '\n':
    case 's':
    case 'S':
        return stick;
    case 'h':
    case 'H':        
        return hit;
    case 'd':
    case 'D':        
        return doubleup;
    }
  }
}


/**
 * it'd be very nice to show the player P (bust) as a fn of the deck,
 * but i'm really out of time for that */
void do_player_actions(deck *d, hand* h, int* player_bet)
{

  int double_flag = 0;
  //int valid_flag = 0;
  enum player_actions act;
    
  while(1){
    if(h->score > 21 && h->low_score > 21) /* they're bust*/
      break;

    act = get_player_action();

    switch(act){
    case hit:
      player_hit(d, h, player_bet);
      break;
    case stick:
      if(double_flag == 0){
        player_stick(d, h, player_bet);
      } else {
        output_message("[player] must hit after doubling!\n");
      }
      break;
    case doubleup:
      if(double_flag == 0){
        player_double(d, h, player_bet);
        double_flag = 1;
      } else {
        output_message("[player] must hit after doubling!\n");
      }
      break;
    default:
      player_stick(d, h, player_bet);
      break;
    }
    
    /* now get out of the forever loop */
    if(double_flag == 1 && (act == hit))
      break;
    if(act == stick && double_flag == 0)
      break;
  }
}

void player_stick(deck *d, hand *h, int *player_bet)
{
  //output_message("[player] STICK\n");
}

void player_hit(deck *d, hand *h, int *player_bet)
{
  char buffer[256];
  /* draw a card for the player */
  draw_cards_to_hand(1, d, h);
  score_hand(h);

  strcpy(buffer, "[player] ");
  hand_to_string(h, buffer);
  output_message(buffer);
}

void player_double(deck *d, hand *h, int *player_bet)
{
  char buffer[256];
  *player_bet *= 2;

  sprintf(buffer, "[player] DOUBLED bet is now %d\n", *player_bet);
  output_message(buffer);
}

