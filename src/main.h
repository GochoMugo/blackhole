/**
 * #!gochomugo
 * The Blackhole Mother
 */
#ifndef _BH_main_h_
#define _BH_main_h_ 1
#define BH_VERSION "0.0.0"

#define _GNU_SOURCE
#define true 1
#define false 0

#define BH_DAEMON_DEFAULT_NAME "blackhole-daemon"
#define BH_DAEMON_DEFAULT_EMAIL "blackhole-daemon@local"

#define BH_PATH_ROOTDIR ".blackhole"
#define BH_PATH_CONFIG "config.ini"

#define BH_EVENT_PULLERRORS "pull.errors"
#define BH_EVENT_MERGECONFLICTSERRORS "merge-conflicts.errors"
#define BH_EVENT_PUSHORIGINERRORS "push-origin.errors"
#define BH_EVENT_PUSHREMOTEERRORS "push-remote.errors"
#define BH_EVENT_FATALERRORS "fatal.errors"

#define return_ok(ret_code) { ret_code = 0; goto cleanup; }
#define return_err(ret_code) if (0 > ret_code) { bh_error_set(ret_code); goto on_error; }
#define return_err_ext(ret_code, custom_err) if (0 > ret_code) { ret_code = custom_err; bh_error_set(ret_code); goto on_error; }
#define return_err_now(custom_err) { ret_code = custom_err; bh_error_set(ret_code); goto on_error; }

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>
#include <iniparser.h>

#include "../deps/path-join/path-join.h"
#include "../deps/flag/flag.h"
#include "../deps/mkdirp/mkdirp.h"
#include "../deps/console-colors/console-colors.h"

#include "error.h"
#include "types.h"
#include "config.h"
#include "counter.h"
#include "daemon.h"
#include "exit.h"
#include "git.h"
#include "hook.h"
#include "print.h"
#include "status.h"
#include "sync.h"


#endif
