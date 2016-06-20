#include "main.h"

int main(void) {
    int failed_tests = 0;

    git_libgit2_init();

    failed_tests += cmocka_run_group_tests_name("bh_config", tests_bh_config, NULL, NULL);
    failed_tests += cmocka_run_group_tests_name("bh_counter", tests_bh_counter, NULL, NULL);
    failed_tests += cmocka_run_group_tests_name("bh_daemon", tests_bh_daemon, NULL, NULL);
    failed_tests += cmocka_run_group_tests_name("bh_error", tests_bh_error, NULL, NULL);
    failed_tests += cmocka_run_group_tests_name("bh_hook", tests_bh_hook, NULL, NULL);
    failed_tests += cmocka_run_group_tests_name("bh_status", tests_bh_status, NULL, NULL);

    git_libgit2_shutdown();

    return failed_tests > 0 ? 1 : 0;
}
