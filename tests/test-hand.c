#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include <math.h>

#include "deck.h"
#include "cards.h"
#include "useful.h"

#include "hand.h"

START_TEST (test_hand_basic)
{ 
  hand *h = get_hand();

  ck_assert_int_eq(h->n_cards, 0);
  ck_assert_int_eq(h->n_aces, 0);
  ck_assert_int_eq(h->score, 0);
  
  print_hand(h);

  /* now lets add some cards,
     this will also try and score the hand but we
     don't care about that right now
  */
  add_card_to_hand(h, 33);
  add_card_to_hand(h, 23);
  add_card_to_hand(h, 27);

  ck_assert_int_eq(h->n_cards, 3);
  ck_assert_int_eq(h->n_aces, 0);

  add_card_to_hand(h, 2); /* now we add an ace */
  ck_assert_int_eq(h->n_aces, 1);
  
  print_hand(h);
  
  free_hand(h);
} 
END_TEST  

/**
 * test the scoring functions */

START_TEST (test_hand_score)
{
  int test_card = 33;
  int expected = 0;
  hand *h = get_hand();

  /* empty hand, no score */
  ck_assert_msg(score_hand(h) ==  0, "expected empty hands to have zero score, got %d", score_hand(h)); 

  /* one card, easy score */
  add_card_to_hand(h, test_card);
  score_hand(h);
  expected = get_value(test_card);
  ck_assert_msg(h->score == expected, "expected score %d, got %d", expected, h->score);

  /* two cards, no aces */
  add_card_to_hand(h, test_card+8);
  score_hand(h);
  expected += get_value(test_card+8);
  ck_assert_msg(h->score == expected, "expected score %d, got %d", expected, h->score);

  /* one card, an ace */
  reset_hand(h);
  add_card_to_hand(h, 1);
  print_hand(h);
  score_hand(h);

  /* two cards, an ace and the test_card */
  reset_hand(h);
  add_card_to_hand(h,1);
  add_card_to_hand(h, test_card);
  print_hand(h);
  score_hand(h);

  /* two cards, test_card then ace */
  reset_hand(h);
  add_card_to_hand(h, test_card);
  add_card_to_hand(h,1);
  print_hand(h);
  score_hand(h);

  reset_hand(h);
  add_card_to_hand(h, test_card);
  add_card_to_hand(h,1);
  add_card_to_hand(h, test_card-8);
  print_hand(h);
  score_hand(h);

  /* two aces */
  /* score list should be: 1+1, 1+11, 11+11 */
  /* tain't working */
  reset_hand(h);
  add_card_to_hand(h,1);
  add_card_to_hand(h,2);
  print_hand(h);
  score_hand(h);

  
  
  
  free_hand(h);
} 
END_TEST

Suite* hand_suite (void) {
  Suite *suite = suite_create("hand");
  TCase *tcase = tcase_create("BasicHandFns");

  /* add tests here */
  tcase_add_test(tcase, test_hand_basic);
  tcase_add_test(tcase, test_hand_score);
        
  suite_add_tcase(suite, tcase);
  return suite;
}


int main (int argc, char *argv[]) {
  /* fairly generic test runner */
  int number_failed;
  Suite *suite = hand_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return number_failed;
}

