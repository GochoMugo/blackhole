#include "hook.test.h"

static char *TEST_DIR = NULL;

typedef struct {
  bh_daemon *daemon;
  char *path_data_dir;
} TestState;

int tests_bh_hook_setup_each(void **state) {
  TEST_DIR = getenv("TEST_DIR");
  assert_non_null(TEST_DIR);

  TestState *test_state = (TestState *) malloc(sizeof(TestState));
  assert_non_null(test_state);

  test_state->daemon = NULL;
  test_state->path_data_dir = NULL;

  test_state->path_data_dir = path_join(TEST_DIR, "data/bh-hook");

  assert_ok(bh_daemon_new(&(test_state->daemon), test_state->path_data_dir));
  assert_non_null(test_state->daemon);

  *state = test_state;
  tests_common_reset();
  return 0;
}

int tests_bh_hook_teardown_each(void **state) {
  TestState *test_state = (TestState *) *state;
  if (NULL != test_state) {
    if (NULL != test_state->daemon) bh_daemon_free(&(test_state->daemon));
    if (NULL != test_state->path_data_dir) free(test_state->path_data_dir);
    free(test_state);
  }
  tests_common_reset();
  return 0;
}

/**
 * `bh_hook_exec()` executes the path passing the parameters.
 * The hook at 'data/bh-hook/hooks/hook.sh' is used for the event 'hook.sh'.
 * Use 'test message' for the message; it is expected by the script.
 */
void tests_bh_hook_exec_ok(void **state) {
  skip_if_filtered_out("tests_bh_hook_exec_ok");
  TestState *test_state = (TestState *) *state;
  assert_ok(bh_hook_exec(test_state->daemon, "hook.sh", "test message"));
}

/**
 * `bh_hook_exec()` returns an error of 'BH_HOOKERR_NO_HOOK' if the hook
 * is not found/
 */
void tests_bh_hook_exec_missing(void **state) {
  skip_if_filtered_out("tests_bh_hook_exec_missing");
  TestState *test_state = (TestState *) *state;
  assert_int_equal(bh_hook_exec(test_state->daemon, "404.sh", "missing"),
                   BH_HOOKERR_NO_HOOK);
}

/**
 * `bh_hook_exec()` returns the error, `BH_HOOKERR_FAILED`, if the hook fails.
 * The script 'fail.sh' exits with a status code of 1.
 */
void tests_bh_hook_exec_fail(void **state) {
  skip_if_filtered_out("tests_bh_hook_exec_fail");
  TestState *test_state = (TestState *) *state;
  assert_int_equal(bh_hook_exec(test_state->daemon, "fail.sh", "fails"),
                   BH_HOOKERR_FAILED);
}

/**
 * `bh_hook_exec()` return the error, `BH_HOOKERR_EXEC`, if execution of
 * the hook fails. The script 'exec.sh' can not be executed.
 */
void tests_bh_hook_exec_execfail(void **state) {
  skip_if_filtered_out("tests_bh_hook_exec_execfail");
  skip();
}
