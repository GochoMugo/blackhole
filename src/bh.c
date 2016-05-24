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
    bh_daemon *daemon = NULL;

    flag_str((const char**) &path, "path", "path to a blackhole directory");
    flag_bool(&do_status, "status", "show status");
    flag_bool(&do_sync, "sync", "run sync");
    flag_parse(argc, argv, BH_VERSION);

    ret_code = bh_daemon_new(&daemon, path);
    return_err(ret_code);

    if (0 != do_status) {
        bh_status_print(daemon);
        goto cleanup;
    }

    if (0 != do_sync) {
        ret_code = bh_run_sync(daemon);
        return_err(ret_code);
        goto cleanup;
    }

    goto cleanup;
on_error:
    goto cleanup;
cleanup:
    bh_exit(daemon, true);
    return ret_code;
}
