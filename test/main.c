#include "main.h"

int main(void) {
    int failed_tests = 0;

    git_libgit2_init();

    failed_tests += cmocka_run_group_tests_name("bh_error", tests_bh_error, NULL, NULL);

    git_libgit2_shutdown();

    return failed_tests > 0 ? 1 : 0;
}
