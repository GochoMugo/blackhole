/**
 * #!gochomugo
 * Configure Vigour
 */
#ifndef _BH_config_h_
#define _BH_config_h_ 1


/**
 * Create a new Configuration object.
 *
 * @param config The loaded configuration object
 * @param rootdir Path to a blackhole directory
 * @return 0 or error code
 */
int bh_config_new(bh_config **config, const char *rootdir);


/**
 * Free a Configuration object.
 *
 * @param config A Configuration object
 */
void bh_config_free(bh_config **config);


#endif
