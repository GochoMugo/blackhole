#include "main.h"

static char *TEST_DIR = NULL;
static char *path = NULL;
static bh_daemon *d = NULL;

int tests_bh_daemon_setup_each(void **state) {
    int ret_code = 0;
    if (NULL == TEST_DIR) TEST_DIR = getenv("TEST_DIR");
    if (NULL == path) path = path_join(TEST_DIR, "data/bh-daemon");
    if (NULL == d) ret_code = bh_daemon_new(&d, path);

    assert_int_equal(ret_code, 0);
    assert_non_null(TEST_DIR);
    assert_non_null(path);
    assert_non_null(d);
    return 0;
}

int tests_bh_daemon_teardown_each(void **state) {
    return 0;
}


/**
 * `bh_daemon_new()` creates a new `bh_config` on the `config` property.
 */
void tests_bh_daemon_new_config(void **state) {
    assert_non_null(d->config);
    assert_string_equal(d->config->name, "test");
}


/**
 * `bh_daemon_new()` creates a `bh_git_manager` on the `manager` property.
 */
void tests_bh_daemon_new_manager(void **state) {
    assert_non_null(d->manager);
}


/**
 * `bh_daemon_new()` resolves paths.
 */
void tests_bh_daemon_new_paths(void **state) {
    assert_non_null(strstr(d->paths.counters, path));
    assert_non_null(strstr(d->paths.counters, "counters"));
    assert_non_null(strstr(d->paths.hooks, path));
    assert_non_null(strstr(d->paths.hooks, "hooks"));
}


/**
 * `bh_daemon_new()` creates a new git signature on the `signature`
 * property.
 */
void tests_bh_daemon_new_signature(void **state) {
    assert_non_null(d->signature);
    assert_string_equal(d->signature->name, "test");
    assert_string_equal(d->signature->email, "test@example.com");
}


/**
 * `bh_daemon_free()` frees the daemon.
 */
void tests_bh_daemon_free_frees(void **state) {
    bh_daemon_free(&d);
    assert_null(d);
}
