#ifndef _BH_tests_hook_h_
#define _BH_tests_hook_h_ 1

#define setup_each    tests_bh_hook_setup_each
#define teardown_each tests_bh_hook_teardown_each

int tests_bh_hook_setup_each(void **state);
int tests_bh_hook_teardown_each(void **state);


/* `bh_hook_exec()` executes the hook, passing the expected parameters */
void tests_bh_hook_exec_ok(void **state);
/* `bh_hook_exec()` returns `BH_HOOKERR_NO_HOOK` if hook is missing */
void tests_bh_hook_exec_missing(void **state);
/* `bh_hook_exec()` returns `BH_HOOKERR_FAILED` if hook exits with a non-zero
 * status code */
void tests_bh_hook_exec_fail(void **state);
/* `bh_hook_exec()` returns `BH_HOOKERR_EXEC` if hook could not be executed. */
void tests_bh_hook_exec_execfail(void **state);


static const struct CMUnitTest tests_bh_hook[] = {
    cmocka_unit_test_setup_teardown(tests_bh_hook_exec_ok, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_hook_exec_missing, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_hook_exec_fail, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_hook_exec_execfail, setup_each, teardown_each),
};


#undef setup_each
#undef teardown_each

#endif
