/**
 * #!gochomugo
 * Print and Splash
 */
#include "main.h"


void
bh_print_marker(const char *message) {
    time_t n = time(NULL);
    struct tm *now = localtime(&n);
    char now_str[32];
    strftime(now_str, 32, "%FT%T%z", now);
    printf("%s # %s: ", now_str, message);
    fflush(stdout);
}


void
bh_print_end(void) {
    puts("------------------------");
}


void
bh_print_ok(void) {
    puts("ok");
}


void
bh_print_error(const bh_error *error) {
    if (NULL == error) return;
    /** we need to flush the stdout buffer, to ensure correct ordering of
     * msgs, since stderr is unbuffered and will be printed immediately */
    fflush(stdout);
    fprintf(stderr, "\n\tERROR: %s\n", error->message);
}
