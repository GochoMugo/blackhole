/**
 * #!gochomugo
 * Play with Errors
 */
#include "main.h"


/**
 * The global error variable
 */
static bh_error bh_err;


/* We need to copy the error message, rather than just point to it
 * since functions like `git_libgit2_shutdown()` tends to discard it
 */
void
bh_error_set(int ret_code) {
    /* If an error is already set, do not overwrite it. Also, if
     * `ret_code` is not less than zero, just return. */
    if (0 != bh_err.code || 0 <= ret_code) {
        return;
    }

    /* Errors from BlackHole */
    if (true == bh_error_class(ret_code, BH_ERR, BH_ERR_END)) {
        bh_err.code = ret_code;
        asprintf(&bh_err.message, "[BH] %s", bh_error_msgs[abs(BH_ERR_START - ret_code)]);
        return;
    }

    /* Errors from LibGit2 */
    const git_error *git_err = giterr_last();
    if (NULL != git_err) {
        bh_err.code = BH_GITERR;
        asprintf(&bh_err.message, "[LIBGIT2] %s", git_err->message);
        return;
    }

    /** Errors from OS or any other routine */
    bh_err.code = BH_ERR;
    asprintf(&bh_err.message, "[MISC] %s", strerror(errno));
    errno = 0;
    return;
}


const bh_error *
bh_error_get(void) {
    if (NULL == bh_err.message) return NULL;
    return &bh_err;
}


void
bh_error_clear(void) {
    if (NULL != bh_err.message) free(bh_err.message);
    bh_err.code = 0;
    bh_err.message = NULL;
}


int
bh_error_class(int error_code, int error_class, int error_end) {
    int diff = error_code - error_class;
    int length = error_end - error_class;
    return diff >= 0 && diff < length ? true : false;
}


int
bh_error_copy(bh_error **out) {
    int ret_code = 0;
    bh_error *e;

    if (NULL == bh_err.message) {
        *out = NULL;
        return_ok(ret_code);
    }

    e = malloc(sizeof(bh_error));
    if (NULL == e) {
        return_err_now(BH_DAEMONERR_NOMEM);
    }

    e->code = bh_err.code;

    ret_code = asprintf(&(e->message), "%s", bh_err.message);
    return_err(ret_code);

    *out = e;

_on_error
_cleanup
    return ret_code;
}


void
bh_error_free(bh_error **error) {
    bh_error *e = *error;
    if (NULL == e) return;
    if (NULL != e->message) free(e->message);
    free(e);
    *error = NULL;
}
