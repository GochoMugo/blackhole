#ifndef _BH_tests_error_h_
#define _BH_tests_error_h_ 1

#define setup_each    tests_bh_error_setup_each
#define teardown_each tests_bh_error_teardown_each

int tests_bh_error_setup_each(void **state);
int tests_bh_error_teardown_each(void **state);


/* `bh_error_set` should set a BH error as expected */
void tests_bh_error_set_bh(void **state);
/* `bh_error_set` should set a LibGit2 error as expected */
void tests_bh_error_set_libgit2(void **state);
/* `bh_error_set` should set a System error as expected */
void tests_bh_error_set_errno(void **state);
/* `bh_error_set` should not set a different error, if it is already set */
void tests_bh_error_set_already_set(void **state);
/* `bh_error_get` returns NULL, if error is unset */
void tests_bh_error_get_null_if_unset(void **state);
/* `bh_error_get` return pointer to the same, global variable */
void tests_bh_error_get_pointer(void **state);
/* `bh_error_clear` reset the global error variable */
void tests_bh_error_clear_resets(void **state);


static const struct CMUnitTest tests_bh_error[] = {
    cmocka_unit_test_setup_teardown(tests_bh_error_set_bh, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_error_set_libgit2, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_error_set_errno, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_error_set_already_set, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_error_get_null_if_unset, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_error_get_pointer, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_error_clear_resets, setup_each, teardown_each),
};


#endif
