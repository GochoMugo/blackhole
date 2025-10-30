#include "status.test.h"

static char *TEST_DIR = NULL;

typedef struct {
  char *data_dir;
  char *stderr;
  char *stdout;
} TestState;

int tests_bh_status_setup_each(void **state) {
  TEST_DIR = getenv("TEST_DIR");
  assert_non_null(TEST_DIR);

  TestState *test_state = (TestState *) malloc(sizeof(TestState));
  assert_non_null(test_state);

  test_state->data_dir = NULL;
  test_state->stderr = NULL;
  test_state->stdout = NULL;

  test_state->data_dir = path_join(TEST_DIR, "data/bh-status");

  char *path_stdout = path_join(test_state->data_dir, "stdout.tmp.txt");
  char *path_stderr = path_join(test_state->data_dir, "stderr.tmp.txt");

  assert_ok(contra_fs_read_file(&(test_state->stderr), path_stderr));
  assert_ok(contra_fs_read_file(&(test_state->stdout), path_stdout));

  free(path_stderr);
  free(path_stdout);

  *state = test_state;
  tests_common_reset();
  return 0;
}

int tests_bh_status_teardown_each(void **state) {
  TestState *test_state = (TestState *) *state;
  if (NULL != test_state) {
    if (NULL != test_state->data_dir) free(test_state->data_dir);
    if (NULL != test_state->stderr) free(test_state->stderr);
    if (NULL != test_state->stdout) free(test_state->stdout);
    free(test_state);
  }
  return 0;
}

/**
 * `bh_status_print()` outputs the status of the blackhole.
 */
void tests_bh_status_print_ok(void **state) {
  skip_if_filtered_out("tests_bh_status_print_ok");
  TestState *test_state = (TestState *) *state;
  char *out_rootdir = NULL;
  assert_non_null(strstr(test_state->stdout, "                name : test"));
  asprintf(&out_rootdir, "             rootdir : %s", test_state->data_dir);
  assert_non_null(strstr(test_state->stdout, out_rootdir));
  assert_non_null(strstr(test_state->stdout, "              branch : master"));
  assert_non_null(strstr(test_state->stdout, "               dirty : yes"));
  assert_non_null(strstr(test_state->stdout, "         pull.errors : 1"));
  assert_non_null(strstr(test_state->stdout, "  push-origin.errors : 2"));
  assert_non_null(strstr(test_state->stdout, "  push-remote.errors : 3"));
}
