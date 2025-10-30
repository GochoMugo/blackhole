#ifndef _BH_tests_util_h_
#define _BH_tests_util_h_ 1

/** Asserts that the file at the provided path does exist. */
void assert_file_exists(const char *file_path);

/** Assert the expression is 0 or truthy. */
#define assert_ok(expr) assert_int_equal(0 <= expr, 1)

/** Executes the file at the provided path and asserts it did not fail. */
void execute_script(const char *filepath);

/** Skip the test if env var TEST_FILTER is set and does not contain provided string. */
#define skip_if_filtered_out(str)                                              \
  if (NULL != getenv("TEST_FILTER") &&                                         \
      NULL == strstr(str, getenv("TEST_FILTER")))                              \
    skip();

/** Skip the test if env var TEST_NO_NETWORK is set. */
#define skip_if_no_network()                                                   \
  if (NULL != getenv("TEST_NO_NETWORK"))                                       \
    skip();

/**
 * Perform common resets for test cases. This is preferrably run before and
 * after each test case.
 */
void tests_common_reset(void);

/**
 * Join two path segments. This is a convenience function that returns
 * the new path instead of a return code.
 *
 * @param  segment1
 * @param  segment2
 * @return Joined path
 */
char *path_join(const char *segment1, const char *segment2);

#endif
