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
        printf("LibIniparser: %s\n", LIBINIPARSER_VERSION);
        return_ok(ret_code);
    }

    ret_code = bh_daemon_new(&daemon, path);
    return_err(ret_code);

    if (0 != do_status) {
        ret_code = bh_status_print(daemon);
        return_err(ret_code);
    }

    if (0 != do_sync) {
        ret_code = bh_run_sync(daemon);
        return_err(ret_code);
    }

_on_error
_cleanup
    bh_daemon_free(&daemon);
    return 0 <= ret_code ? EXIT_SUCCESS : EXIT_FAILURE;
}
