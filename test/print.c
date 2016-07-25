#include "main.h"


static const char *path = NULL;


int tests_bh_print_setup_each(void **state) {
    char *test_dir = NULL;

    if (NULL == path) {
        test_dir = getenv("TEST_DIR");
        path = path_join(test_dir, "data/bh-print");
    }

    assert_non_null(path);
    tests_common_reset();
    return 0;
}


int tests_bh_print_teardown_each(void **state) {
    return 0;
}


/**
 * `bh_print_marker()`, `bh_print_ok()` and `bh_print_status()`
 * should print as expected. The file at 'data/bh-print/output.txt'
 * contains the expected output of executing the executable 'run.out'.
 */
void tests_bh_print_ok(void **state) {
    /* we need to redirect stderr to stdout for popen to catch it */
    char *cmd = path_join(path, "run.out 2>&1");
    char *path_output = path_join(path, "output.txt");
    assert_output_equal(cmd, path_output, 1);
}
