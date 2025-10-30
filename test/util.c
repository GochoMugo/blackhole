/* test!skip-gen */

void assert_file_exists(const char *file_path) {
  struct stat file_stats;
  assert_non_null(file_path);
  assert_ok(stat(file_path, &file_stats));
}

void execute_script(const char *file_path) {
  assert_non_null(file_path);
  int rc = system(file_path);
  assert_int_not_equal(rc, -1);
  if (0 != WEXITSTATUS(rc)) {
    fprintf(stderr, " [X] script failed: %s\n", file_path);
    fail();
  }
}

char *path_join(const char *segment1, const char *segment2) {
  char *path = NULL;
  assert_ok(contra_path_join(&path, segment1, segment2));
  assert_non_null(path);
  return path;
}

void tests_common_reset(void) { bh_error_clear(); }
