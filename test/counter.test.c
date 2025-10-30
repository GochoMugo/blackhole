#include "counter.test.h"

static char *TEST_DIR = NULL;
static char *path_counters = NULL;

int tests_bh_counter_setup_each(void **state) {
  TEST_DIR = getenv("TEST_DIR");
  assert_non_null(TEST_DIR);

  path_counters = path_join(TEST_DIR, "data/bh-counter/.blackhole/counters");
  assert_non_null(path_counters);

  tests_common_reset();
  return 0;
}

int tests_bh_counter_teardown_each(void **state) {
  if (NULL != path_counters) free(path_counters);
  return 0;
}

/**
 * `bh_counter_get()` reads the counter file.
 * 'zero.counter' contains '0' while 'one.counter' contains '1'.
 */
void tests_bh_counter_get_reads(void **state) {
  skip_if_filtered_out("tests_bh_counter_get_reads");
  int zero, one;

  assert_ok(bh_counter_get(&zero, path_counters, "zero.counter"));
  assert_ok(bh_counter_get(&one, path_counters, "one.counter"));

  assert_int_equal(zero, 0);
  assert_int_equal(one, 1);
}

/**
 * `bh_counter_get()` returns zero for a missing counter file.
 * There is no counter file named '404.counter'
 */
void tests_bh_counter_get_missing(void **state) {
  skip_if_filtered_out("tests_bh_counter_get_missing");
  int count;
  assert_ok(bh_counter_get(&count, path_counters, "404.counter"));
  assert_int_equal(count, 0);
}

/**
 * `bh_counter_get()` returns zero for a file that is blank i.e. has zero
 * length. 'blank.counter' is a zero-length counter file.
 */
void tests_bh_counter_get_blank(void **state) {
  skip_if_filtered_out("tests_bh_counter_get_blank");
  int count;
  assert_ok(bh_counter_get(&count, path_counters, "blank.counter"));
  assert_int_equal(count, 0);
}

/**
 * `bh_counter_get()` return error BH_COUNTERR_FREAD if the file could
 * not be read. 'nan.counter' contains a character that is not a number.
 */
void tests_bh_counter_get_nan(void **state) {
  skip_if_filtered_out("tests_bh_counter_get_nan");
  int count = -2;
  int ret_code = bh_counter_get(&count, path_counters, "nan.counter");
  assert_int_equal(ret_code, BH_COUNTERERR_FREAD);
  assert_int_equal(count, -2);
}

/**
 * `bh_counter_get()` return error BH_COUNTERR_FREAD if the file is
 * binary.
 */
void tests_bh_counter_get_binary(void **state) {
  skip_if_filtered_out("tests_bh_counter_get_binary");
  int count = -2;
  int ret_code = bh_counter_get(&count, path_counters, "binary.tmp.counter");
  assert_int_equal(ret_code, BH_COUNTERERR_FREAD);
  assert_int_equal(count, -2);
}

/**
 * `bh_counter_get()` return error BH_COUNTERERR_FOPEN if the file
 * could not be read. 'unreadable.tmp.counter' has permissions set to
 * deny us access to read it.
 */
void tests_bh_counter_get_read_perm(void **state) {
  skip_if_filtered_out("tests_bh_counter_get_read_perm");
  int count = -2;
  int ret_code =
      bh_counter_get(&count, path_counters, "unreadable.tmp.counter");
  assert_int_equal(ret_code, BH_COUNTERERR_FOPEN);
  assert_int_equal(count, -2);
}

/**
 * `bh_counter_increment` increments to 1, if the counter file is missing.
 * Increment using the counter file 'missing.tmp.counter'. It is deleted
 * if existent, in setup.sh.
 */
void tests_bh_counter_increment_new(void **state) {
  skip_if_filtered_out("tests_bh_counter_increment_new");
  char *counter = "missing.tmp.counter";
  int count = -1;
  assert_ok(bh_counter_increment(&count, path_counters, counter));
  assert_int_equal(count, 1);
  count = -1;
  assert_ok(bh_counter_get(&count, path_counters, counter));
  assert_int_equal(count, 1);
}

/**
 * `bh_counter_increment()` continually increments the counter.
 * Use counter file 'continue.tmp.counter'. Loop a couple of times
 * and watch the counter increment.
 */
void tests_bh_counter_increment_continue(void **state) {
  skip_if_filtered_out("tests_bh_counter_increment_continue");
  char *counter = "continue.tmp.counter";
  int count = -1;
  int i = 1;
  for (; i < 5; i++) {
    assert_ok(bh_counter_increment(&count, path_counters, counter));
    assert_int_equal(count, i);
  }
}

/**
 * `bh_counter_reset()` resets a counter by removing the associating
 * counter file.
 * Use counter file 'reset.tmp.counter'. Create it with
 * `bh_counter_increment()` then reset it. Check to ensure the counter
 * returns '0'.
 */
void tests_bh_counter_reset_reset(void **state) {
  skip_if_filtered_out("tests_bh_counter_reset_reset");
  char *counter = "reset.tmp.counter";
  int count = -1;
  assert_ok(bh_counter_increment(&count, path_counters, counter));
  assert_int_equal(count, 1);
  assert_ok(bh_counter_reset(path_counters, counter));
  count = -1;
  assert_ok(bh_counter_get(&count, path_counters, counter));
  assert_int_equal(count, 0);
}

/**
 * `bh_counter_reset()` succeeds if the counter does not exist (yet).
 * Use counter file '404.counter'. It is not existent on disk.
 */
void tests_bh_counter_reset_missing(void **state) {
  skip_if_filtered_out("tests_bh_counter_reset_missing");
  char *counter = "404.counter";
  assert_ok(bh_counter_reset(path_counters, counter));
}

/**
 * `bh_counter_tick()` sets the tick to 'true' if the count satisfies
 * the intervals. Use a pre-computed scenario for this.
 */
void tests_bh_counter_tick_tick(void **state) {
  skip_if_filtered_out("tests_bh_counter_tick_tick");
  char *counter = "tick.tmp.counter";
  int tick = -1;
  int output[] = {-1, true, false, true, false};
  int i = 1;
  int l = 5;
  for (; i < l; i++) {
    assert_ok(bh_counter_tick(&tick, path_counters, counter, 2));
    assert_int_equal(tick, output[i]);
    tick = -1;
  }
}
