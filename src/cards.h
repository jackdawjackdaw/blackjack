#ifndef _INC_CARDS_
#define _INC_CARDS_

#DEFINE NSUITS 4
#DEFINE NTYPES 13

char suit_to_char(int suit);
void suit_to_str(int suit, char* buffer);
char type_to_char(int type);
void type_to_str(int type, char* buffer);

#endif _INC_CARDS_
