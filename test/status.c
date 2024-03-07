#include "main.h"


static bh_daemon *d = NULL;
static const char *path = NULL;

int tests_bh_status_setup_each(void **state) {
    char *test_dir = NULL;

    if (NULL == path) {
        test_dir = getenv("TEST_DIR");
        path = path_join(test_dir, "data/bh-status");
    }

    if (NULL == d) {
        assert_ok(bh_daemon_new(&d, path));
    }

    assert_non_null(path);
    assert_non_null(d);
    tests_common_reset();
    return 0;
}


int tests_bh_status_teardown_each(void **state) {
    return 0;
}


/**
 * `bh_status_print()` outputs the status of the blackhole.
 * The file at 'data/bh-status/output.txt' contains the expected output
 * of `bh_status_print()` in the directory `data/bh-status`.
 * Compare the actual and expected output.
 */
void tests_bh_status_print_ok(void **state) {
    skip_if_filtered_out("tests_bh_status_print_ok");
    char *cmd = path_join(path, "run.out");
    char *path_output = path_join(path, "output.txt");
    assert_output_equal(cmd, path_output, 0);
}
