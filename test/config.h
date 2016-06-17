#ifndef _BH_tests_config_h
#define _BH_tests_config_h_ 1

#define setup_each    tests_bh_config_setup_each
#define teardown_each tests_bh_config_teardown_each

int tests_bh_config_setup_each(void **state);
int tests_bh_config_teardown_each(void **state);


/* `bh_config_new()` loads the configuration file */
void tests_bh_config_new_dict(void **state);
/* `bh_config_new()` sets path to blackhole directory */
void tests_bh_config_new_path(void **state);
/* `bh_config_new()` loads daemon name from configuration */
void tests_bh_config_new_daemon_name(void **state);
/* `bh_config_new()` loads daemon email from configuration */
void tests_bh_config_new_daemon_email(void **state);
/* `bh_config_new()` loads the runstate path from configuration */
void tests_bh_config_new_runstate_path(void **state);
/* `bh_config_new()` returns an error, 'BH_CONFIGERR_LOADFILE', if
 * configuration file is missing */
void tests_bh_config_new_missing_configfile(void **state);
/* `bh_config_free()` makes the configuration object NULL */
void tests_bh_config_free_null(void **state);


static const struct CMUnitTest tests_bh_config[] = {
    cmocka_unit_test_setup_teardown(tests_bh_config_new_dict, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_config_new_path, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_config_new_daemon_name, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_config_new_daemon_email, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_config_new_runstate_path, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_config_new_missing_configfile, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_config_free_null, setup_each, teardown_each),
};


#undef setup_each
#undef teardown_each

#endif
