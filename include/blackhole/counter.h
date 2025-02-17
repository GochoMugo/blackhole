/**
 * #!gochomugo
 * Numbers in Counters
 */
#ifndef _BH_counter_h_
#define _BH_counter_h_ 1

/**
 * Retrieve the counter's value
 *
 * @param out The destination of the current value
 * @param counters_path Path to the directory of counters
 * @param event The event in consideration
 * @return 0 or error code
 */
int bh_counter_get(int *out, const char *counters_path, const char *name);

/**
 * Increment a counter.
 *
 * @param out The destination of the new count
 * @param counters_path Path to the directory of counters
 * @param event The event in consideration
 * @return 0 or error code
 */
int bh_counter_increment(int *out, const char *counters_path, const char *name);

/**
 * Reset counter.
 *
 * @param counters_path Path to the directory of counters
 * @param event The event in consideration
 * @return 0 or error code
 */
int bh_counter_reset(const char *counters_path, const char *name);

/**
 * Tick a counter. This increments a counter, passing `true` in `tick`
 * should interval be considered 'top of the hour' i.e.
 * (new_count-1) % intervals === 0. This implies that should the
 * event be the first to be recorded in the counter, the tick will
 * always be true.
 *
 * @param tick True if in 'top of hour' state. Otherwise, false.
 * @param path Path to directory of counters
 * @param event The event in consideration
 * @param intervals Intervals of event notification
 * @return 0 or error code
 */
int bh_counter_tick(int *tick, const char *counters_path, const char *event,
                    int intervals);

#endif
