/**
 * #!gochomugo
 * The Blackhole Mother
 */
#ifndef _BH_blackhole_h_
#define _BH_blackhole_h_ 1

#define BH_VERSION "0.0.0-develop"

#define BH_DAEMON_DEFAULT_NAME "blackhole-daemon"
#define BH_DAEMON_DEFAULT_EMAIL "blackhole-daemon@local"

#define BH_PATH_ROOTDIR ".blackhole"
#define BH_PATH_CONFIG "config.ini"

#define BH_EVENT_PULLERRORS "pull.errors"
#define BH_EVENT_MERGECONFLICTSERRORS "merge-conflicts.errors"
#define BH_EVENT_PUSHORIGINERRORS "push-origin.errors"
#define BH_EVENT_PUSHREMOTEERRORS "push-remote.errors"
#define BH_EVENT_FATALERRORS "fatal.errors"

#include "blackhole/error.h"
#include "blackhole/types.h"
#include "blackhole/config.h"
#include "blackhole/counter.h"
#include "blackhole/daemon.h"
#include "blackhole/git.h"
#include "blackhole/hook.h"
#include "blackhole/print.h"
#include "blackhole/status.h"
#include "blackhole/sync.h"

#endif
