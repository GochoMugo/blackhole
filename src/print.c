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
bh_print_status(bh_daemon *daemon) {
    const bh_error *err = bh_error_get();

   if (NULL == err->message) {
        bh_print_ok();
        return;
    }

    /** we need to flush the stdout buffer, to ensure correct ordering of
     * msgs, since stderr is unbuffered and will be printed immediately */
    fflush(stdout);
    fprintf(stderr, "\n\tERROR: %s\n", err->message);
}


void
bh_print_status_and_exit(bh_daemon *daemon) {
    const bh_error *err = bh_error_get();

    if (NULL == err->message) {
        bh_print_ok();
        return;
    }

    bh_print_status(daemon);
    bh_exit(daemon, false);
}
