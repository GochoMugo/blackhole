#ifndef _BH_tests_daemon_h
#define _BH_tests_daemon_h_ 1

#define setup_each    tests_bh_daemon_setup_each
#define teardown_each tests_bh_daemon_teardown_each

int tests_bh_daemon_setup_each(void **state);
int tests_bh_daemon_teardown_each(void **state);


/* `bh_daemon_new()` loads and creates a configuration object */
void tests_bh_daemon_new_config(void **state);
/* `bh_daemon_new()` creates a git manager */
void tests_bh_daemon_new_manager(void **state);
/* `bh_daemon_new()` resolves internal paths */
void tests_bh_daemon_new_paths(void **state);
/* `bh_daemon_new()` creates a git signature */
void tests_bh_daemon_new_signature(void **state);
/* `bh_daemon_free()` frees the daemon */
void tests_bh_daemon_free_frees(void **state);


static const struct CMUnitTest tests_bh_daemon[] = {
    cmocka_unit_test_setup_teardown(tests_bh_daemon_new_config, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_daemon_new_manager, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_daemon_new_paths, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_daemon_new_signature, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_daemon_free_frees, setup_each, teardown_each),
};


#undef setup_each
#undef teardown_each

#endif
