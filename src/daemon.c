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

    ret_code = bh_config_new(&(d->config), path);
    return_err(ret_code);

    /* Resolve internal path to 'counters' directory */
    ret_code = contra_path_join(&(d->paths.counters), d->config->runstate_path, "counters");
    if (NULL == d->paths.counters) return_err_now(BH_DAEMONERR_PATHRESLV);

    /* Resolve internal path to 'counters' */
    ret_code = contra_path_join(&(d->paths.hooks), d->config->runstate_path, "hooks");
    if (NULL == d->paths.hooks) return_err_now(BH_DAEMONERR_PATHRESLV);

    /* Create a new repository manager */
    ret_code = bh_git_repository_manager_new(&(d->manager), d->config);
    return_err(ret_code);

    /* Create the daemon signature */
    ret_code = git_signature_now(&(d->signature), d->config->name, d->config->email);
    return_err(ret_code);

    *daemon = d;

_on_error
    if (NULL != d) bh_daemon_free(&d);
_cleanup
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
