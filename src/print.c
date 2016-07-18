/**
 * #!gochomugo
 * Print and Splash
 */
#include "main.h"


void
bh_print_marker(const char *message) {
    printf("# %s: ", message);
}


void
bh_print_ok(void) {
    puts("ok");
}


void
bh_print_status(bh_daemon *daemon, const bh_error *error) {
   if (NULL == error) {
        bh_print_ok();
        return;
    }

    /** we need to flush the stdout buffer, to ensure correct ordering of
     * msgs, since stderr is unbuffered and will be printed immediately */
    fflush(stdout);
    fprintf(stderr, "\n\tERROR: %s\n", error->message);
}


void
bh_print_status_and_exit(bh_daemon *daemon) {
    bh_error *error = NULL;

    if (NULL == bh_error_get()) {
        bh_print_ok();
        return;
    }

    /** copy the error to ensure it is not cleared by other functions */
    bh_error_copy(&error);

    bh_print_status(daemon, error);
    bh_exit(daemon, error);
}
