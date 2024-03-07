#include "main.h"

static char *TEST_DIR = NULL;
static char *path = NULL;
static bh_config *config = NULL;

int tests_bh_config_setup_each(void **state) {
    int ret_code = 0;
    if (NULL == TEST_DIR) TEST_DIR = getenv("TEST_DIR");
    if (NULL == path) path = path_join(TEST_DIR, "data/bh-config");
    if (NULL == config) ret_code = bh_config_new(&config, path);

    assert_int_equal(ret_code, 0);
    assert_non_null(TEST_DIR);
    assert_non_null(path);
    assert_non_null(config);
    tests_common_reset();
    return 0;
}

int tests_bh_config_teardown_each(void **state) {
    return 0;
}


/**
 * `bh_config_new()` loads the configuration file into a dictionary structure
 * onto the 'dict' property.
 */
void tests_bh_config_new_dict(void **state) {
    skip_if_filtered_out("tests_bh_config_new_dict");
    assert_non_null(config->dict);
}


/**
 * `bh_config_new()` creates a new configuration object with 'path'
 * property to the blackhole root directory.
 */
void tests_bh_config_new_path(void **state) {
    skip_if_filtered_out("tests_bh_config_new_path");
    assert_string_equal(config->path, path);
}


/**
 * `bh_config_new()` loads the name of the daemon, from the configuration
 * file in the blackhole directory. This is to 'test' for testing
 * purposes.
 */
void tests_bh_config_new_daemon_name(void **state) {
    skip_if_filtered_out("tests_bh_config_new_daemon_name");
    assert_string_equal(config->name, "test");
}


/**
 * `bh_config_new()` loads the email of the daemon, from the configuration
 * file. This is set to `test@example.com` in config file during testing.
 */
void tests_bh_config_new_daemon_email(void **state) {
    skip_if_filtered_out("tests_bh_config_new_daemon_email");
    assert_string_equal(config->email, "test@example.com");
}


/**
 * `bh_config_new()` loads the runstate path from the configuration file.
 * This is set to 'runstate' in config file during testing.
 */
void tests_bh_config_new_runstate_path(void **state) {
    skip_if_filtered_out("tests_bh_config_new_runstate_path");
    assert_non_null(strstr(config->runstate_path, "runstate"));
}


/**
 * `bh_config_new()` returns `BH_CONFIGERR_LOADFILE` if configuration is
 * not found.
 */
void tests_bh_config_new_missing_configfile(void **state) {
    skip_if_filtered_out("tests_bh_config_new_missing_configfile");
    int error_code = 0;

    bh_config *config_missing = NULL;
    error_code = bh_config_new(&config_missing, "/404");
    assert_int_equal(error_code, BH_CONFIGERR_LOADFILE);
}


/**
 * `bh_config_free()` NULLifies the `bh_config` object.
 */
void tests_bh_config_free_null(void **state) {
    skip_if_filtered_out("tests_bh_config_free_null");
    bh_config_free(&config);
    assert_null(config);
}
