/**
 * #!gochomugo
 * Hooks are just Awesome!
 */
#ifndef _BH_hook_h_
#define _BH_hook_h_ 1

/**
 * Execute the hook named 'name'
 *
 * @param daemon The Daemon information
 * @param event The event in consideration
 * @param message Message to pass to hook
 * @return 0 or error code
 */
int
bh_hook_exec(const bh_daemon *daemon,
        const char *event,
        const char *message);


#endif
