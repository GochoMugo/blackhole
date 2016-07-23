/**
 * #!gochomugo
 * Sync with other Humans
 */
#include "main.h"


int
bh_run_sync(bh_daemon *daemon) {
    int ret_code = 0;
    git_reference *current_ref = NULL;
    int origin_only = true;
    git_commit *new_commit_after_merge = NULL;
    git_commit *new_commit_after_commit = NULL;

    /** Fetch changes */
    bh_print_marker("Fetching changes");
    ret_code = bh_git_fetch_origin(daemon->manager);
    bh_print_status_and_exit(daemon);

    /** Get the current branch name */
    bh_print_marker("Getting the current reference");
    ret_code = git_repository_head(&current_ref, daemon->manager->repository);
    bh_print_status_and_exit(daemon);

    /** If we are on master branch, run merge */
    if (0 == strcmp("refs/heads/master", git_reference_name(current_ref))) {
        /** This allows us to push to other remotes, other than 'origin' */
        origin_only = false;

        /** Merge any changes from remote-tracking branch */
        bh_print_marker("Merging changes from remote-tracking branch");
        ret_code = bh_git_merge_origin(&new_commit_after_merge, daemon->manager);
        if (BH_GITERR_MERGE_CONFLICTS == ret_code) {
            /** Checking out the machine branch */
            bh_error_clear();
            ret_code = bh_git_checkout_branch(daemon->manager, daemon->config->name);
        }
        bh_print_status_and_exit(daemon);
    }

    /** Commit local changes */
    bh_print_marker("Committing local changes");
    ret_code = bh_git_commit_changes(&new_commit_after_commit, daemon->manager, new_commit_after_merge, daemon->signature);
    if (ret_code == BH_GITERR_NO_CHANGES) {
        ret_code = 0;
        bh_error_clear();
        puts("no changes");
        goto cleanup;
    }
    bh_print_status_and_exit(daemon);

    /** Push changes */
    bh_print_marker("Pushing changes");
    ret_code = bh_git_push(daemon->manager, current_ref, origin_only);
    bh_print_status_and_exit(daemon);

_on_error
_cleanup
    if (NULL != new_commit_after_merge) git_commit_free(new_commit_after_merge);
    if (NULL != new_commit_after_commit) git_commit_free(new_commit_after_commit);
    if (NULL != current_ref) git_reference_free(current_ref);
    return ret_code;
}
