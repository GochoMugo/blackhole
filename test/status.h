#ifndef _BH_tests_status_h_
#define _BH_tests_status_h_ 1

#define setup_each    tests_bh_status_setup_each
#define teardown_each tests_bh_status_teardown_each

int tests_bh_status_setup_each(void **state);
int tests_bh_status_teardown_each(void **state);


/* `bh_status_print()` prints the status of the directory */
void tests_bh_status_print_ok(void **state);


static const struct CMUnitTest tests_bh_status[] = {
    cmocka_unit_test_setup_teardown(tests_bh_status_print_ok, setup_each, teardown_each),
};


#undef setup_each
#undef teardown_each

#endif
