#include "main.h"


static bh_daemon *daemon_hooks = NULL;
static const char *path = NULL;

int tests_bh_hook_setup_each(void **state) {
    char *test_dir = NULL;

    if (NULL == path) {
        test_dir = getenv("TEST_DIR");
        path = path_join(test_dir, "data/bh-hook");
    }

    if (NULL == daemon_hooks) {
        assert_ok(bh_daemon_new(&daemon_hooks, path));
    }

    assert_non_null(path);
    assert_non_null(daemon_hooks);
    tests_common_reset();
    return 0;
}


int tests_bh_hook_teardown_each(void **state) {
    return 0;
}


/**
 * `bh_hook_exec()` executes the path passing the parameters.
 * The hook at 'data/bh-hook/hooks/hook.sh' is used for the eveent 'hook.sh'.
 * Use 'test message' for the message; it is expected by the script.
 */
void tests_bh_hook_exec_ok(void **state) {
    skip_if_filtered_out("tests_bh_hook_exec_ok");
    assert_ok(bh_hook_exec(daemon_hooks, "hook.sh", "test message"));
}


/**
 * `bh_hook_exec()` returns an error of 'BH_HOOKERR_NO_HOOK' if the hook
 * is not found/
 */
void tests_bh_hook_exec_missing(void **state) {
    skip_if_filtered_out("tests_bh_hook_exec_missing");
    assert_int_equal(bh_hook_exec(daemon_hooks, "404.sh", "missing"), BH_HOOKERR_NO_HOOK);
}


/**
 * `bh_hook_exec()` returns the error, `BH_HOOKERR_FAILED`, if the hook fails.
 * The script 'fail.sh' exits with a status code of 1.
 */
void tests_bh_hook_exec_fail(void **state) {
    skip_if_filtered_out("tests_bh_hook_exec_fail");
    assert_int_equal(bh_hook_exec(daemon_hooks, "fail.sh", "fails"), BH_HOOKERR_FAILED);
}


/**
 * `bh_hook_exec()` return the error, `BH_HOOKERR_EXEC`, if execution of
 * the hook fails. The script 'exec.sh' can not be executed.
 */
void tests_bh_hook_exec_execfail(void **state) {
    skip_if_filtered_out("tests_bh_hook_exec_execfail");
    skip();
}
