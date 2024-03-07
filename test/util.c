/* test!skip-gen */
#include "main.h"


void assert_output_equal(const char *executable, const char *output_filepath, int mode) {
    FILE *file_actual = popen(executable, "r");
    FILE *file_expected = fopen(output_filepath, "r");
    char *line_actual = NULL, *line_expected = NULL;
    size_t size_actual = 0, size_expected = 0;
    size_t read_actual, read_expected;

    assert_non_null(file_actual);
    assert_non_null(file_expected);

    /* compare the two streams line by line */
    while (1) {
        read_actual = getline(&line_actual, &size_actual, file_actual);
        read_expected = getline(&line_expected, &size_expected, file_expected);
        if (read_actual == read_expected && EOF == read_expected) break;
        switch (mode) {
        case 0:
            assert_int_equal(read_actual, read_expected);
            assert_string_equal(line_actual, line_expected);
            break;
        case 1:
            assert_true(read_actual >= read_expected);
            assert_non_null(strstr(line_actual, line_expected));
        }
    }

    pclose(file_actual);
    fclose(file_expected);
}


void tests_common_reset() {
    bh_error_clear();
}


char *path_join(const char *segment1, const char *segment2) {
    int ret_code = 0;
    char *path = NULL;

    ret_code = contra_path_join(&path, segment1, segment2);
    return path;
}
