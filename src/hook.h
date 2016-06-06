/**
 * #!gochomugo
 * Hooks are just Awesome!
 */
#ifndef _BH_hook_h_
#define _BH_hook_h_ 1

/**
 * Execute the hook named 'name'
 *
 * @param hooks_path Path to the 'hooks' directory
 * @param event The event in consideration
 * @param daemon The Daemon information
 * @param message Message to pass to hook
 * @return 0 or error code
 */
int
bh_hook_exec(const char *hooks_path,
        const char *event,
        const bh_daemon *daemon,
        const char *message);


#endif
