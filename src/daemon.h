/**
 * #!gochomugo
 * Daemon Demon
 */
#ifndef _BH_daemon_h_
#define _BH_daemon_h_ 1


/**
 * Create daemon information.
 *
 * @param name The newly-created daemon
 * @param path Path to the Repository
 * @return 0 or error code
 */
int bh_daemon_new(bh_daemon **daemon, char *path);


/**
 * Free a daemon.
 *
 * @param daemon Daemon
 */
void bh_daemon_free(bh_daemon *daemon);


#endif
