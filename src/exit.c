/**
 * #!gochomugo
 * BlackHole Exits
 */
#include "main.h"


/**
 * Runs the appropriate hook, if allowed.
 *
 * @param daemon The current daemon
 * @param error_code Error code
 * @param event The event in consideration
 * @return 1 if run, 0 if not.
 */
int bh_exit__hook_run(bh_daemon *daemon, int error_code, const char *event) {
    int ret_code = 0;
    int run = false;
    int intervals = 10;
    char *interval_key = NULL;
    const bh_error *bh_err = bh_error_get();

    ret_code = asprintf(&interval_key, "intervals:%s", event);
    return_err(ret_code);

    intervals = iniparser_getint(daemon->config->dict, interval_key, intervals);

    /* If the set error and 'error_code' match, we will consider running
     * the hook */
    if (error_code == bh_err->internal_error ||
            (NULL != bh_err->git_error && error_code == bh_err->git_error->klass)) {
        ret_code = bh_counter_tick(&run, daemon->paths.counters, event, intervals);
        return_err(ret_code);

        if (true == run) {
            ret_code = bh_hook_exec(daemon->paths.hooks, event, daemon, bh_err->message);
            return_err(ret_code);
        }

        ret_code = 1;
    } else {
        ret_code = bh_counter_reset(daemon->paths.counters, event);
        return_err(ret_code);

        ret_code = 0;
    }

    goto cleanup;
on_error:
    goto cleanup;
cleanup:
    if (NULL != interval_key) free(interval_key);
    return ret_code;
}


void
bh_exit__hooks(bh_daemon *daemon) {
    int ret_code = 0;
    int runs = 0;
    const bh_error *bh_err = bh_error_get();

    ret_code = bh_exit__hook_run(daemon, BH_GITERR_PULL_ORIGIN, BH_EVENT_PULLERRORS);
    runs += ret_code;
    ret_code = bh_exit__hook_run(daemon, BH_GITERR_MERGE_CONFLICTS, BH_EVENT_MERGECONFLICTSERRORS);
    runs += ret_code;
    ret_code = bh_exit__hook_run(daemon, BH_GITERR_PUSH_ORIGIN, BH_EVENT_PUSHORIGINERRORS);
    runs += ret_code;
    ret_code = bh_exit__hook_run(daemon, BH_GITERR_PUSH_REMOTE, BH_EVENT_PUSHREMOTEERRORS);
    runs += ret_code;

    /* If no other hook has been executed for the set error, we execute the
     * fatal.errors hook */
    if (0 == runs && NULL != bh_err->message) bh_hook_exec(daemon->paths.hooks, BH_EVENT_FATALERRORS, daemon, bh_err->message);
}


void bh_exit(bh_daemon *daemon, int success) {
    bh_exit__hooks(daemon);
    exit(true == success ? EXIT_SUCCESS : EXIT_FAILURE);
}
