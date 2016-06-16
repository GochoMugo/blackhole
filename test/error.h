#ifndef _BH_tests_error_h_
#define _BH_tests_error_h_ 1

/* `bh_error_set` should set a BH error as expected */
void tests_bh_error_set_bh(void **state);

static const struct CMUnitTest tests_bh_error[] = {
    cmocka_unit_test(tests_bh_error_set_bh),
};

#endif
