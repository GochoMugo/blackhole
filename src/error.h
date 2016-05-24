/**
 * #!gochomugo
 * Play with Errors
 */
#ifndef _BH_error_h_
#define _BH_error_h_ 1


#define BH_ERR_START -101


/**
 * Errors
 */
typedef enum {
    BH_GITERR_NULL_PATH = BH_ERR_START,     /** Provided path was found to be NULL */
    BH_GITERR_PULL_ORIGIN,                  /** Could not pull from origin */
    BH_GITERR_NO_UPDATES,                   /** No updates were found in remote */
    BH_GITERR_MERGE_CONFLICTS,              /** Merge conflicts occurred */
    BH_GITERR_NO_CHANGES,                   /** No changes were found */
    BH_GITERR_PUSH_ORIGIN,                  /** Push to 'origin' failed */
    BH_GITERR_PUSH_REMOTE,                  /** Push to remote (other than 'origin') failed */
    BH_GITERR_NOMEM,                        /** Allocating memory failed, in git module */
    BH_CONFIGERR_NOMEM,                     /** Allocating memory failed, in config module */
    BH_CONFIGERR_LOADFILE,                  /** Failed to load configuration file */
    BH_DAEMONERR_NOMEM,                     /** Allocating memory failed, in daemon module  */
    BH_DAEMONERR_PATHRESLV,                 /** Failed to resolve internal path */
    BH_HOOKERR_NO_HOOK,                     /** Hook not registered */
    BH_HOOKERR_HOOK_FAILED,                 /** Hook failed */
    BH_COUNTERERR_FOPEN,                    /** Failed to open counter file */
    BH_COUNTERERR_FREAD,                    /** Could not read counter file */
    BH_COUNTERERR_FWRITE,                   /** Could not write to counter file */
    BH_COUNTERERR_UNLINK,                   /** Could not unlink counter file */
    BH_COUNTERERR_ENOENT,                   /** File backing the counter is missing */
    BH_ERR_END
} bh_error_id;


/**
 * These are the error messages for the above defined errors. They are
 * expected into the same order and number as the error IDs defined
 * above
 */
static char *bh_error_msgs[] = {
    "null path to repository",
    "error pulling from origin",
    "no updates found in remote",
    "error in merge due to conflicts",
    "no changes to commit",
    "error in push to origin",
    "error in push to a redundant remote",
    "error in creating memory for repository manager",
    "error in creating memory for config",
    "error loading configuration file",
    "error in creating memory for daemon",
    "error resolving internal paths",
    "hook not found",
    "hook exited unsuccessfully",
    "error reading the counter file",
    "error writing to counter file",
    "error unlinking counter file",
    "counter file missing",
};


/**
 * BH error. A custom error type is being used to accommodate
 * errors from git functions and other routines, such as `malloc`.
 */
typedef struct {
    /** error message */
    char *message;

    /** BH internal error */
    int internal_error;

    /** the raw actual git error, if available */
    const git_error *git_error;
} bh_error;


/**
 * Sets the error on the global variable to the
 * last error that occurred. Ensure to call this function
 * before other invocations that might manipulate 'errno'
 * or git's last error (from `giterr_last()`). Also, if an error has not
 * been cleared, it won't be overwritten even if a subsequent invocation
 * passes a different error. Therefore you will need to clear it.
 *
 * @param ret_code Return code
 */
void bh_error_set(int ret_code);


/**
 * Return the error
 *
 * @return the error global variable
 */
const bh_error *bh_error_get(void);


/**
 * Clear the global error variable. This allows other errors to be set.
 * Otherwise you will be stuck with the set error.
 */
void bh_error_clear(void);


#endif
