#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include <math.h>

#include "deck.h"


START_TEST (test_deck_basic)
{
  int card;
  int i;
  deck *d = get_deck(1);
  ck_assert_int_eq(d->n_cards, 52);
  ck_assert_int_eq(d->top_card, 0);

  print_deck(d);
  
  card = draw_card(d);
  ck_assert_int_eq(d->n_cards, 52);
  ck_assert_int_eq(d->top_card, 1);
  ck_assert_int_eq(card, 0);
  ck_assert_int_eq(n_cards_remaining(d), 51);

  for(i = 0; i < 10; i++){
    card = draw_card(d);
  }
  ck_assert_int_eq(d->top_card, 11);
  ck_assert_int_eq(n_cards_remaining(d), 52-11);

  printf("# after drawing 11 cards \n");
  print_deck(d);
  
  free_deck(d);

  /* now repeat for a bigger deck */
  d = get_deck(3);
  ck_assert_int_eq(d->n_cards, 52*3);
  ck_assert_int_eq(d->top_card, 0); 
  free_deck(d);
  
} 
END_TEST

/* this only works for a 3 card deck */



START_TEST (test_deck_shuffle)
{
  deck *d = get_deck(1);
  int i, idx;
  int nshuffles = 2<<12;
  int perm;
  int expected;
  
  /* lets make a special deck of only 4 cards */
  d -> n_cards = 3;
  ck_assert_int_eq(n_cards_remaining(d), 3);

  srandom(1); /* set the seed */
  
  printf("# unshuffled tiny deck\n");
  print_deck(d);
  
  shuffle_deck(d);
  
  printf("# shuffled deck\n");
  print_deck(d);

  /* we'll well can try and do something like shuffling a lot of 
   * times and count the permutations we get */
  int permCounts[6] = {0,0,0,0,0,0};
  for(i = 0; i < nshuffles; i++){
    shuffle_deck(d);

    perm = (d->cards[0]+1)*100 + (d->cards[1]+1)*10 + (d->cards[2]+1);

    //printf("%d %d\n", i, perm);

    /* i'm sure there's a way to do this properly in knuth 4, btu 
     * this will do as a rough test */
    switch(perm){
    case 123:
      idx = 0;
      break;
    case 132:
      idx = 1;
      break;
    case 213:
      idx = 2;
      break;
    case 231:
      idx = 3;
      break;
    case 312:
      idx = 4;
      break;
    case 321:
      idx = 5;
      break;
    }
    permCounts[idx]++;
  }

  expected = nshuffles / 6;

  
  for(i = 0; i < 6; i++){
    /* (obs-expected)^2 / expected */
    printf("%d %d %lf\n", i, permCounts[i], pow((permCounts[i] - expected),2.0)/(expected) );
  }
  
  free_deck(d);
}
END_TEST


Suite* deck_suite (void) {
  Suite *suite = suite_create("Deck");
  TCase *tcase = tcase_create("BasicDeckFns");

  /* add tests here */
  tcase_add_test(tcase, test_deck_basic);
  tcase_add_test(tcase, test_deck_shuffle);  
        
  suite_add_tcase(suite, tcase);
  return suite;
}


int main (int argc, char *argv[]) {
  /* fairly generic test runner */
  int number_failed;
  Suite *suite = deck_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return number_failed;
}
