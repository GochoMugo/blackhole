/**
 * #!gochomugo
 * Daemon Demon
 */
#include "main.h"


int
bh_daemon_new(bh_daemon **daemon, const char *path) {
    int ret_code = 0;
    bh_daemon *d = NULL;

    d = (bh_daemon *) malloc(sizeof(bh_daemon));
    if (NULL == d) return_err_now(BH_DAEMONERR_NOMEM);

    d->config = NULL;
    d->paths.counters = NULL;
    d->paths.hooks = NULL;
    d->manager = NULL;
    d->signature = NULL;

    return_err(bh_config_new(&(d->config), path));

    /* Resolve internal path to 'counters' directory */
    return_err_ext(contra_path_join(&(d->paths.counters), d->config->runstate_path, "counters"), BH_DAEMONERR_PATHRESLV);

    /* Resolve internal path to 'counters' */
    return_err_ext(contra_path_join(&(d->paths.hooks), d->config->runstate_path, "hooks"), BH_DAEMONERR_PATHRESLV);

    /* Create a new repository manager */
    return_err(bh_git_repository_manager_new(&(d->manager), d->config));

    /* Create the daemon signature */
    return_err_ext(git_signature_now(&(d->signature), d->config->name, d->config->email), BH_DAEMONERR_SIGNATURE);

    *daemon = d;

on_error:
    if (NULL != d) bh_daemon_free(&d);
cleanup:
    return ret_code;
}


void
bh_daemon_free(bh_daemon **daemon) {
    bh_daemon *d = *daemon;
    if (NULL == d) return;
    if (NULL != d->config) bh_config_free(&(d->config));
    if (NULL != d->paths.counters) free(d->paths.counters);
    if (NULL != d->paths.hooks) free(d->paths.hooks);
    if (NULL != d->manager) bh_git_repository_manager_free(&(d->manager));
    if (NULL != d->signature) git_signature_free(d->signature);
    free(d);
    *daemon = NULL;
}
