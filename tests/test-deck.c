#include <stdio.h>
#include <string.h>
#include <check.h>

#include "deck.h"


START_TEST (test_deck_init)
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



Suite* deck_suite (void) {
        Suite *suite = suite_create("Deck");
        TCase *tcase = tcase_create("BasicDeckFns");

        /* add tests here */
        tcase_add_test(tcase, test_deck_init);
        
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
