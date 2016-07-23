/**
 * #!gochomugo
 * Hooks are just Awesome!
 */
#include "main.h"


int
bh_hook_exec(const bh_daemon *daemon, const char *event, const char *message) {
    int ret_code = 0;
    char *path = NULL;
    char *command = NULL;
    int exit_status = 0;
    size_t len = 0;

    path = path_join(daemon->paths.hooks, event);
    if (NULL == path) {
        return_err_now(BH_DAEMONERR_PATHRESLV);
    }

    ret_code = asprintf(&command, "%s '%s' '%s' '%s'", path, message,
            daemon->config->name, daemon->config->path);
    return_err(ret_code);

    ret_code = system(command);
    return_err_ext(ret_code, BH_HOOKERR_EXEC);

    exit_status = WEXITSTATUS(ret_code);
    if (0 != exit_status) {
        if (127 == exit_status) return_err_now(BH_HOOKERR_NO_HOOK);
        return_err_now(BH_HOOKERR_FAILED);
    }
    ret_code = 0; /* `system()` above sets to exit code of command */

_on_error
_cleanup
    if (NULL != command) free(command);
    if (NULL != path) free(path);
    return ret_code;
}
