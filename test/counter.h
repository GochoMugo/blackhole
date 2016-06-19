#ifndef _BH_tests_counter_h_
#define _BH_tests_counter_h_ 1

#define setup_each    tests_bh_counter_setup_each
#define teardown_each tests_bh_counter_teardown_each

int tests_bh_counter_setup_each(void **state);
int tests_bh_counter_teardown_each(void **state);


/* `bh_counter_get()` reads the counter file */
void tests_bh_counter_get_reads(void **state);
/* `bh_counter_get()` returns 0 (zero) if counter file is missing */
void tests_bh_counter_get_missing(void **state);
/* `bh_counter_get()` returns 0 (zero) if counter file is of zero length */
void tests_bh_counter_get_blank(void **state);
/* `bh_counter_get()` returns error 'BH_COUNTERERR_FREAD' if file contains
 * a non-number character */
void tests_bh_counter_get_nan(void **state);
/* `bh_counter_get()` return error 'BH_COUNTERR_FREAD' if file is binary */
void tests_bh_counter_get_binary(void **state);
/* `bh_counter_get()` returns error 'BH_COUNTERERR_FOPEN' if file lacks
 * read permissions set */
void tests_bh_counter_get_read_perm(void **state);
/* `bh_counter_increment()` increments counter to 1, if file is missing */
void tests_bh_counter_increment_new(void **state);
/* `bh_counter_increment()` continues to increment the counter */
void tests_bh_counter_increment_continue(void **state);
/* `bh_counter_reset()` resets the counter to 0. */
void tests_bh_counter_reset_reset(void **state);
/* `bh_counter_reset()` succeeds if the counter does not exist (yet). */
void tests_bh_counter_reset_missing(void **state);
/* `bh_counter_tick()` ticks */
void tests_bh_counter_tick_tick(void **state);


static const struct CMUnitTest tests_bh_counter[] = {
    cmocka_unit_test_setup_teardown(tests_bh_counter_get_reads, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_get_missing, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_get_blank, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_get_nan, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_get_binary, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_get_read_perm, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_increment_new, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_increment_continue, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_reset_reset, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_reset_missing, setup_each, teardown_each),
    cmocka_unit_test_setup_teardown(tests_bh_counter_tick_tick, setup_each, teardown_each),
};


#undef setup_each
#undef teardown_each

#endif
