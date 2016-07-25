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
 * Print the end line.
 */
void bh_print_end(void);


/**
 * Print 'ok' to indicate success.
 */
void bh_print_ok(void);


/**
 * Print the error message, from 'error'.
 * If 'error' is NULL, nothing is printed.
 *
 * @param error Current error
 */
void bh_print_error(const bh_error *error);


#endif
