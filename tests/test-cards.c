#include <stdio.h>
#include <string.h>
#include <check.h>

#include "cards.h"


START_TEST (test_suit_short)
{

  fail_if(suit_to_char(get_suit(0)) != 'D', "failed to get suit short");
  fail_if(suit_to_char(get_suit(5)) != 'C', "failed to get suit short");
  fail_if(suit_to_char(get_suit(2)) != 'H', "failed to get suit short");
} 
END_TEST

START_TEST (test_suit_string)
{
  char buffer[256];

  suit_to_str(get_suit(0), buffer);
  ck_assert_str_eq(buffer, "Diamonds");

  suit_to_str(get_suit(4), buffer);
  ck_assert_str_eq(buffer, "Diamonds");

  suit_to_str(get_suit(5), buffer);
  ck_assert_str_eq(buffer, "Clubs");

  suit_to_str(get_suit(6), buffer);
  ck_assert_str_eq(buffer, "Hearts");

  suit_to_str(get_suit(11), buffer);
  ck_assert_str_eq(buffer, "Spades");

}
END_TEST


START_TEST (test_type_short)
{

  fail_if(type_to_char(get_type(0)) != 'A', "failed to get type short");
  fail_if(type_to_char(get_type(10)) != '3', "failed to get type short");
  fail_if(type_to_char(get_type(20)) != '6', "failed to get type short");
  fail_if(type_to_char(get_type(40)) != 'J', "failed to get type short");
  
} 
END_TEST

START_TEST (test_type_string)
{
  char buffer[256];

  type_to_str(get_type(0), buffer);
  ck_assert_str_eq(buffer, "Ace");

  type_to_str(get_type(10), buffer);
  ck_assert_str_eq(buffer, "3");

  type_to_str(get_type(37), buffer);
  ck_assert_str_eq(buffer, "10");

  type_to_str(get_type(40), buffer);
  ck_assert_str_eq(buffer, "Jack");

  type_to_str(get_type(44), buffer);
  ck_assert_str_eq(buffer, "Queen");
  
  type_to_str(get_type(50), buffer);
  ck_assert_str_eq(buffer, "King");
  
}
END_TEST

START_TEST (test_type_dummy)
{
  int i;
  char type_buffer[32];
  char suit_buffer[32];
  for(i = 0; i < NSUITS*NTYPES; i++){
    type_to_str(get_type(i), type_buffer);
    suit_to_str(get_suit(i), suit_buffer);
    printf("%d %s %s\n", i, type_buffer, suit_buffer);
  }
} 
END_TEST     


Suite* card_suite (void) {
        Suite *suite = suite_create("Core");
        TCase *tcase = tcase_create("BasicCardFns");

        /* add tests here */
        tcase_add_test(tcase, test_suit_short);
        tcase_add_test(tcase, test_suit_string);

        tcase_add_test(tcase, test_type_short);
        tcase_add_test(tcase, test_type_string);

        tcase_add_test(tcase, test_type_dummy);
        
        suite_add_tcase(suite, tcase);
        return suite;
}

int main (int argc, char *argv[]) {
  /* fairly generic test runner */
        int number_failed;
        Suite *suite = card_suite();
        SRunner *runner = srunner_create(suite);
        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);
        return number_failed;
}
