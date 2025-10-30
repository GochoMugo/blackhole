#include "daemon.test.h"

static char *TEST_DIR = NULL;
static char *path = NULL;
static bh_daemon *test_daemon = NULL;

int tests_bh_daemon_setup_each(void **state) {
  TEST_DIR = getenv("TEST_DIR");
  assert_non_null(TEST_DIR);

  path = path_join(TEST_DIR, "data/bh-daemon");
  assert_non_null(path);

  assert_ok(bh_daemon_new(&test_daemon, path));
  assert_non_null(test_daemon);

  tests_common_reset();
  return 0;
}

int tests_bh_daemon_teardown_each(void **state) {
  if (NULL != test_daemon) bh_daemon_free(&test_daemon);
  if (NULL != path) free(path);
  return 0;
}

/**
 * `bh_daemon_new()` creates a new `bh_config` on the `config` property.
 */
void tests_bh_daemon_new_config(void **state) {
  skip_if_filtered_out("tests_bh_daemon_new_config");
  assert_non_null(test_daemon->config);
  assert_string_equal(test_daemon->config->name, "test");
}

/**
 * `bh_daemon_new()` creates a `bh_git_manager` on the `manager` property.
 */
void tests_bh_daemon_new_manager(void **state) {
  skip_if_filtered_out("tests_bh_daemon_new_manager");
  assert_non_null(test_daemon->manager);
}

/**
 * `bh_daemon_new()` resolves paths.
 */
void tests_bh_daemon_new_paths(void **state) {
  skip_if_filtered_out("tests_bh_daemon_new_paths");
  assert_non_null(strstr(test_daemon->paths.counters, path_join(path, ".blackhole/counters")));
  assert_non_null(strstr(test_daemon->paths.hooks, path_join(path, ".blackhole/hooks")));
}

/**
 * `bh_daemon_new()` creates a new git signature on the `signature`
 * property.
 */
void tests_bh_daemon_new_signature(void **state) {
  skip_if_filtered_out("tests_bh_daemon_new_signature");
  assert_non_null(test_daemon->signature);
  assert_string_equal(test_daemon->signature->name, "test");
  assert_string_equal(test_daemon->signature->email, "test@example.com");
}

/**
 * `bh_daemon_free()` frees the daemon.
 */
void tests_bh_daemon_free_frees(void **state) {
  skip_if_filtered_out("tests_bh_daemon_free_frees");
  bh_daemon_free(&test_daemon);
  assert_null(test_daemon);
}
