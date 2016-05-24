/**
 * #!gochomugo
 * Hooks are just Awesome!
 */
#include "main.h"


int
bh_hook_exec(const char *hooks_path, const char *event, const char *message) {
    int ret_code = 0;
    char *path = NULL;
    char *command = NULL;
    FILE *hook_out = NULL;
    char *line = NULL;
    size_t len = 0;

    path = path_join(hooks_path, event);
    if (NULL == path) {
        return_err_now(BH_DAEMONERR_PATHRESLV);
    }

    ret_code = asprintf(&command, "%s '%s'", path, message);
    return_err(ret_code);

    hook_out = popen(command, "r");
    if (NULL == hook_out) {
        return_err_now(BH_HOOKERR_HOOK_FAILED);
    }

    ret_code = getline(&line, &len, hook_out);
    return_err(ret_code);

    if (NULL != strstr(line, ": not found")) {
        return_err_now(BH_HOOKERR_NO_HOOK);
    }

    goto cleanup;
on_error:
    goto cleanup;
cleanup:
    if (NULL != line) free(line);
    if (NULL != hook_out) pclose(hook_out);
    if (NULL != command) free(command);
    if (NULL != path) free(path);
    return ret_code;
}
