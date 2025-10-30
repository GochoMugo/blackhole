#include "print.test.h"

static char *TEST_DIR = NULL;

typedef struct {
  char *stderr;
  char *stdout;
} TestState;

int tests_bh_print_setup_each(void **state) {
  TEST_DIR = getenv("TEST_DIR");
  assert_non_null(TEST_DIR);

  TestState *test_state = (TestState *) malloc(sizeof(TestState));
  assert_non_null(test_state);

  test_state->stderr = NULL;
  test_state->stdout = NULL;

  char *path_stdout = path_join(TEST_DIR, "data/bh-print/stdout.tmp.txt");
  char *path_stderr = path_join(TEST_DIR, "data/bh-print/stderr.tmp.txt");

  assert_ok(contra_fs_read_file(&(test_state->stderr), path_stderr));
  assert_ok(contra_fs_read_file(&(test_state->stdout), path_stdout));

  free(path_stderr);
  free(path_stdout);

  *state = test_state;
  tests_common_reset();
  return 0;
}

int tests_bh_print_teardown_each(void **state) {
  TestState *test_state = (TestState *) *state;
  if (NULL != test_state) {
    if (NULL != test_state->stderr) free(test_state->stderr);
    if (NULL != test_state->stdout) free(test_state->stdout);
    free(test_state);
  }
  return 0;
}

/** `bh_print_marker()` and `bh_print_ok()` should print as expected. */
void tests_bh_print_marker_ok(void **state) {
  skip_if_filtered_out("tests_bh_print_marker_ok");
  TestState *test_state = (TestState *) *state;
  assert_non_null(strstr(test_state->stdout, "# Marker: ok"));
}

/** `bh_print_end()` should print as expected. */
void tests_bh_print_end(void **state) {
  skip_if_filtered_out("tests_bh_print_end");
  TestState *test_state = (TestState *) *state;
  assert_non_null(strstr(test_state->stdout, "------------------------"));
}

/** `bh_print_error()` should print as expected. */
void tests_bh_print_error(void **state) {
  skip_if_filtered_out("tests_bh_print_error");
  TestState *test_state = (TestState *) *state;
  assert_non_null(strstr(test_state->stderr, "	ERROR: [BH] generic error"));
}
