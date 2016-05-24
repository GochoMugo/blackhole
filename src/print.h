/**
 * #!gochomugo
 * Print and Splash
 */
#ifndef _BH_print_h_
#define _BH_print_h_ 1


/**
 * Print a stage marker.
 *
 * @param message Message to print to stdout
 */
void bh_print_marker(const char *message);


/**
 * Print 'ok' to indicate success.
 */
void bh_print_ok(void);


/**
 * Print the status of the global error variable
 */
void bh_print_status(bh_daemon *daemon);


/**
 * Print the status, as in `print_status()`, and exit with EXIT_FAILURE
 * if the global error variable is set
 */
void bh_print_status_and_exit(bh_daemon *daemon);


#endif
