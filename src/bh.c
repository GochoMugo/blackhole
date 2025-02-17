/**
 * #!gochomugo
 * Blackhole, the Backup Utility
 */
#include "main.h"


int
main(int argc, const char **argv) {
    int ret_code = 0;
    char *path = getenv("PWD");
    bool do_status = 0;
    bool do_sync = 0;
    bool do_version_full = 0;
    bh_daemon *daemon = NULL;

    flag_str((const char**) &path, "path", "path to a blackhole directory");
    flag_bool(&do_status, "status", "show status");
    flag_bool(&do_sync, "sync", "run sync");
    flag_bool(&do_version_full, "version-full", "show full version information");
    flag_parse(argc, argv, BH_VERSION);

    if (0 != do_version_full) {
        printf("Blackhole:    %s\n", BH_VERSION);
        printf("LibGit2:      %s\n", LIBGIT2_VERSION);
        printf("Contra:       %s\n", CONTRA_VERSION);
        return_ok(0);
    }

    return_err(bh_daemon_new(&daemon, path));

    if (0 != do_status) {
        return_err(bh_status_print(daemon));
    } else if (0 != do_sync) {
        return_err(bh_run_sync(daemon));
    }

on_error:
    bh_print_error(bh_error_get());
cleanup:
    bh_daemon_free(&daemon);
    return 0 <= ret_code ? EXIT_SUCCESS : EXIT_FAILURE;
}
