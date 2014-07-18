#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "cards.h"

/**
 * ccs, 18.07.2014
 * 
 * helpful functions for dealing with cards 
 * we can represent a card as an int 0..51 (in a standard deck).
 * or more generally 0..(NSUITS*NTYPES)-1,
 * unsigned short ints would be more memory efficient (buuuutt...)
 * 
 * 
 * After picking an ordering of suits S1, S2, S3, S4 then
 *
 *       |        0 |     1 |      2 |      3 |
 *       | Diamonds | Clubs | Hearts | Spades |
 *
 * integer division and modulus will of the card will return the suit and type
 * here type means 'jack', 'ace' , 2, taking the ordering 
 *
 *   type     |   0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |  9 |   10 |    11 |   12 |
 *   card     | ace | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | jack | queen | king |
 * 
 * if aces are wild then we have to be careful with mapping type to an actual score used to
 * determine the outcome of a hand
 * 
 * the output of 'test-cards' lists all the cards from 0..NSUITS*NTYPES which is neat 
 */

int get_suit( int card)
{
  assert(card >= 0 && card < NSUITS*NTYPES);
  return(card % NSUITS);
}

int get_type( int card)
{
  assert(card >= 0 && card < NSUITS*NTYPES);  
  return(card / NSUITS);
}

char suit_to_char(int suit)
{
  assert(suit >= 0 && suit < 4);
  switch(suit){
  case 0:
    return('D');
  case 1:
    return('C');
  case 2:
    return('H');
  case 3:
    return('S');
  default:
    /* raise an error here? */
    fprintf(stderr, "# err in suit_to_char, expected suit in [0..3], got %d\n", suit);
    exit(EXIT_FAILURE);
  }
}


/** 
 * buffer should be allocated to at least 9 chars in size, (diamonds has 8 plus '\0')
 * \fix should probably return a string instead, this is a very annoying interface
 */ 
void suit_to_str(int suit, char* buffer)
{
  assert(suit >= 0 && suit < 4);
  /* fprintf(stderr, "buff size: %lu\n",  sizeof(buffer)/sizeof(char)); */
  /* assert(sizeof(buffer)/sizeof(buffer[0]) >= 9); */
  
  switch(suit){
  case 0:
    sprintf(buffer, "Diamonds");
    break;
  case 1:
    sprintf(buffer, "Clubs");
    break;
  case 2:
    sprintf(buffer, "Hearts");
    break;
  case 3:
    sprintf(buffer, "Spades");
    break;
  default:
    fprintf(stderr, "# err in suit_to_str, expected suit in [0..3], got %d\n", suit);
    exit(EXIT_FAILURE);
  }
}
 

char type_to_char(int type)
{
  assert(type >= 0 && type < 13);
  
  switch(type){
  case 0:
    return('A');
  case 10:
    return('J');
  case 11:
    return('Q');
  case 12:
    return('K');
  default:
    return((char)(((int)'0')+type+1));
  }
}  


/**
 * \fix this should return a string 
 */
void type_to_str(int type, char* buffer)
{
  assert(type >= 0 && type < 13);

  switch(type){
  case 0:
    sprintf(buffer, "Ace");
    break;
  case 10:
    sprintf(buffer, "Jack");
    break;
  case 11:
    sprintf(buffer, "Queen");
    break;
  case 12:
    sprintf(buffer, "King");
    break;
  default: 
    sprintf(buffer, "%d", type+1);
    break;
  }
  
}

