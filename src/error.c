/**
 * #!gochomugo
 * Play with Errors
 */
#include "main.h"


/**
 * The global error variable
 */
static bh_error bh_err;


void
bh_error_set(int ret_code) {
    /* If an error is already set, do not overwrite it */
    if (NULL != bh_err.message) {
        return;
    }

    /* Errors from BlackHole */
    if (BH_ERR_START <= ret_code && BH_ERR_END > ret_code) {
        bh_err.internal_error = ret_code;
        asprintf(&bh_err.message, "[BH] %s", bh_error_msgs[abs(BH_ERR_START - ret_code)]);
        return;
    }

    /* Errors from LibGit2 */
    bh_err.git_error = giterr_last();
    if (NULL != bh_err.git_error) {
        /* We need to copy the error message, rather than just point to it
         * since `git_libgit2_shutdown()` tends to discard it */
        asprintf(&bh_err.message, "[LIBGIT2] %s", bh_err.git_error->message);
        return;
    }

    /** Errors from OS or any other routine */
    asprintf(&bh_err.message, "[MISC] %s", strerror(errno));
    errno = 0;
    return;
}


const bh_error *
bh_error_get(void) {
    return &bh_err;
}


void
bh_error_clear(void) {
    bh_err.message = NULL;
    bh_err.internal_error = 0;
    bh_err.git_error = NULL;
}
