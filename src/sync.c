/**
 * #!gochomugo
 * Sync with other Humans
 */
#include "main.h"


int bh_sync__handle_error(const bh_daemon *daemon, const char *event);
int bh_sync__hook_exec(const bh_daemon *daemon, const bh_error *error, const char *event);
void bh_sync__counter_reset(const bh_daemon *daemon, ...);


int
bh_run_sync(bh_daemon *daemon) {
    int ret_code = 0;
    git_reference *current_ref = NULL;
    int origin_only = true;
    git_commit *new_commit_after_merge = NULL;
    git_commit *new_commit_after_commit = NULL;
    int event_raised = false;

#define raise_event(event, error_code) \
    if (error_code == ret_code) { \
        bh_sync__handle_error(daemon, event); \
        event_raised = true; \
    }
#define otherwise(events...) \
    if (false == event_raised) { \
        if (0 > ret_code) bh_sync__handle_error(daemon, BH_EVENT_FATALERRORS); \
        else bh_print_ok(); \
    } \
    return_err(ret_code); \
    bh_sync__counter_reset(daemon, ##events, NULL)

    /** Fetch changes */
    bh_print_marker("Fetching changes");
    ret_code = bh_git_fetch_origin(daemon->manager);
    raise_event(BH_EVENT_PULLERRORS, BH_GITERR_PULL_ORIGIN);
    otherwise(BH_EVENT_PULLERRORS);

    /** Get the current branch name */
    bh_print_marker("Getting the current reference");
    ret_code = git_repository_head(&current_ref, daemon->manager->repository);
    otherwise(NULL);

    /** If we are on master branch, run merge */
    if (0 == strcmp("refs/heads/master", git_reference_name(current_ref))) {
        /** This allows us to push to other remotes, other than 'origin' */
        origin_only = false;

        /** Merge any changes from remote-tracking branch */
        bh_print_marker("Merging changes from remote-tracking branch");
        ret_code = bh_git_merge_origin(&new_commit_after_merge, daemon->manager);
        if (BH_GITERR_MERGE_CONFLICTS == ret_code) {
            raise_event(BH_EVENT_MERGECONFLICTSERRORS, BH_GITERR_MERGE_CONFLICTS);
            bh_error_clear();

            /** Checking out the machine branch */
            ret_code = bh_git_checkout_branch(daemon->manager, daemon->config->name);
        }
        otherwise(NULL);
    }

    /** Commit local changes */
    bh_print_marker("Committing local changes");
    ret_code = bh_git_commit_changes(&new_commit_after_commit, daemon->manager, new_commit_after_merge, daemon->signature);
    if (ret_code == BH_GITERR_NO_CHANGES) {
        puts("no changes");
        return_ok(ret_code);
    }
    otherwise(NULL);

    /** Push changes */
    bh_print_marker("Pushing changes");
    ret_code = bh_git_push(daemon->manager, current_ref, origin_only);
    raise_event(BH_EVENT_PUSHORIGINERRORS, BH_GITERR_PUSH_ORIGIN);
    raise_event(BH_EVENT_PUSHREMOTEERRORS, BH_GITERR_PUSH_REMOTE);
    otherwise(BH_EVENT_PUSHORIGINERRORS, BH_EVENT_PUSHREMOTEERRORS);

_on_error
_cleanup
    if (NULL != new_commit_after_merge) git_commit_free(new_commit_after_merge);
    if (NULL != new_commit_after_commit) git_commit_free(new_commit_after_commit);
    if (NULL != current_ref) git_reference_free(current_ref);
    return ret_code;
#undef raise_event
#undef otherwise
}


/**
 * Handle an error during sync. This basically prints the error messages
 * and executes the corresponding hook.
 *
 * @param daemon Current daemon
 * @param event Event being raised
 * @param return 0 on success. <0 otherwise.
 */
int
bh_sync__handle_error(const bh_daemon *daemon, const char *event) {
    int ret_code = 0;
    bh_error *error = NULL;

    ret_code = bh_error_copy(&error);
    return_err(ret_code);

    assert(error);

    bh_print_error(error);

    ret_code = bh_sync__hook_exec(daemon, error, event);
    return_err(ret_code);

_on_error
_cleanup
    return ret_code;
}


/**
 * Execute the hook corresponding to the 'event', due to the 'error'.
 * This ticks the event's counter and executes the associated hook if
 * the current interval allows.
 *
 * @param Daemon Current daemon
 * @param error Error in consideration
 * @param event Event raised
 * @return 0 on success. <0 otherwise.
 */
int bh_sync__hook_exec(const bh_daemon *daemon, const bh_error *error, const char *event) {
    int ret_code = 0;
    int run = false;
    int intervals = 10;
    char *interval_key = NULL;

    ret_code = asprintf(&interval_key, "intervals:%s", event);
    return_err(ret_code);

    intervals = iniparser_getint(daemon->config->dict, interval_key, intervals);

    ret_code = bh_counter_tick(&run, daemon->paths.counters, event, intervals);
    return_err(ret_code);

    if (true == run) {
        ret_code = bh_hook_exec(daemon, event, error->message);
        return_err(ret_code);
    }

    ret_code = run;

_on_error
_cleanup
    if (NULL != interval_key) free(interval_key);
    return ret_code;
}


/**
 * Reset the counters.
 *
 * @param daemon Current daemon
 * @param ... Events whose counters are to be reset
 */
void
bh_sync__counter_reset(const bh_daemon *daemon, ...) {
    va_list events;
    char *event;

    va_start(events, daemon);
    while(NULL != (event = va_arg(events, char*)))
        bh_counter_reset(daemon->paths.counters, event);
    va_end(events);
}
