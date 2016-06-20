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
    char *cmd = path_join(path, "run.out");
    char *path_output = path_join(path, "output.txt");
    FILE *file_actual = popen(cmd, "r");
    FILE *file_expected = fopen(path_output, "r");
    char *line_actual, *line_expected;
    size_t size_actual = 0, size_expected = 0;
    size_t read_actual, read_expected;

    assert_non_null(file_actual);
    assert_non_null(file_expected);

    /* compare the two streams line by line */
    while (1) {
        read_actual = getline(&line_actual, &size_actual, file_actual);
        read_expected = getline(&line_expected, &size_expected, file_expected);
        if (EOF == read_expected) break;
        assert_int_equal(read_actual, read_expected);
        assert_string_equal(line_actual, line_expected);
    }

    pclose(file_actual);
    fclose(file_expected);
}
