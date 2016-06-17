/**
 * #!gochomugo
 * Configure Vigour
 */
#include "main.h"


int
bh_config_new(bh_config **config, const char* rootdir) {
    int ret_code = 0;
    char *bh_path = NULL;
    char *config_path = NULL;
    bh_config *c = NULL;

    c = malloc(sizeof(bh_config));
    if (NULL == c) {
        return_err_now(BH_CONFIGERR_NOMEM);
    }

    c->path = NULL;
    c->name = NULL;
    c->email = NULL;
    c->runstate_path = NULL;
    c->dict = NULL;

    bh_path = path_join(rootdir, BH_PATH_ROOTDIR);
    config_path = path_join(bh_path, BH_PATH_CONFIG);

    c->dict = iniparser_load(config_path);
    if (NULL == c->dict) {
        return_err_now(BH_CONFIGERR_LOADFILE);
    }

    c->path = rootdir;
    c->name = iniparser_getstring(c->dict, "daemon:name", BH_DAEMON_DEFAULT_NAME);
    c->email = iniparser_getstring(c->dict, "daemon:email", BH_DAEMON_DEFAULT_EMAIL);
    c->runstate_path = path_join(bh_path, iniparser_getstring(c->dict, "runstate:path", ""));

    *config = c;

    goto cleanup;
on_error:
    if (NULL != c) bh_config_free(c);
    goto cleanup;
cleanup:
    if (NULL != bh_path) free(bh_path);
    if (NULL != config_path) free(config_path);
    return ret_code;
}


void
bh_config_free(bh_config **config) {
    bh_config *c = *config;
    if (NULL == c) return;
    /** config->name, config->email are freed with `iniparser_freedict()` below */
    if (NULL != c->dict) iniparser_freedict((dictionary *) c->dict);
    if (NULL != c->path) free(c->path);
    if (NULL != c->runstate_path) free(c->runstate_path);
    free(c);
    *config = NULL;
}
