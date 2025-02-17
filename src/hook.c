/**
 * #!gochomugo
 * Hooks are just Awesome!
 */
#include "main.h"

int bh_hook_exec(const bh_daemon *daemon, const char *event,
                 const char *message) {
  int ret_code = 0;
  char *path = NULL;
  char *command = NULL;
  int exit_code = 0;
  int exit_status = 0;

  return_err_ext(contra_path_join(&path, daemon->paths.hooks, event),
                 BH_DAEMONERR_PATHRESLV);

  return_err(asprintf(&command, "%s '%s' '%s' '%s'", path, message,
                      daemon->config->name, daemon->config->path));

  exit_code = system(command);
  return_err_ext(exit_code, BH_HOOKERR_EXEC);

  exit_status = WEXITSTATUS(exit_code);
  if (0 != exit_status) {
    if (127 == exit_status)
      return_err_now(BH_HOOKERR_NO_HOOK);
    return_err_now(BH_HOOKERR_FAILED);
  }

on_error:
cleanup:
  if (NULL != command)
    free(command);
  if (NULL != path)
    free(path);
  return ret_code;
}
