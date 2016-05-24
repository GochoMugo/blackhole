/**
 * #!gochomugo
 * Daemon Demon
 */
#include "main.h"


int
bh_daemon_new(bh_daemon **daemon, char *path) {
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
    d->paths.counters = path_join(d->config->runstate_path, "counters");
    if (NULL == d->paths.counters) return_err_now(BH_DAEMONERR_PATHRESLV);

    /* Resolve internal path to 'counters' */
    d->paths.hooks = path_join(d->config->runstate_path, "hooks");
    if (NULL == d->paths.hooks) return_err_now(BH_DAEMONERR_PATHRESLV);

    /* Create a new repository manager */
    ret_code = bh_git_repository_manager_new(&(d->manager), d->config);
    return_err(ret_code);

    /* Create the daemon signature */
    ret_code = git_signature_now(&(d->signature), d->config->name, d->config->email);
    return_err(ret_code);

    *daemon = d;

    goto cleanup;
on_error:
    if (NULL != d) bh_daemon_free(d);
    goto cleanup;
cleanup:
    return ret_code;
}


void
bh_daemon_free(bh_daemon *daemon) {
    if (NULL == daemon) return;
    if (NULL != daemon->config) bh_config_free(daemon->config);
    if (NULL != daemon->paths.counters) free(daemon->paths.counters);
    if (NULL != daemon->paths.hooks) free(daemon->paths.hooks);
    if (NULL != daemon->manager) bh_git_repository_manager_free(daemon->manager);
    if (NULL != daemon->signature) git_signature_free(daemon->signature);
    free(daemon);
    daemon = NULL;
}
