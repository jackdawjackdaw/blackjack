#ifndef _INC_CARDS_
#define _INC_CARDS_

#define NSUITS 4
#define NTYPES 13

int get_suit( int card);
int get_type( int card);

char suit_to_char(int suit);
void suit_to_str(int suit, char* buffer);
char type_to_char(int type);
void type_to_str(int type, char* buffer);

void card_to_str_long(int card, char* buffer);
void card_to_str_short(int card, char* buffer);

#endif 
