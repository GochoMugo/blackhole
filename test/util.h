#ifndef _BH_tests_util_h_
#define _BH_tests_util_h_ 1


/**
 * Assert that the 'executable' produces the same output as recorded
 * in the output file at 'output_filepath'.
 *
 * Modes define how the comparisons are done. They include:
 *  0: the strings must be exactly equal
 *  1. the string from the run process contains the string in the output file
 *
 * @param executable The executable to be run
 * @param output_filepath Path to a txt file with expected output
 * @param mode Mode to use. 0 for strict, 1 for lenient.
 */
void assert_output_equal(const char *executable, const char *output_filepath, int mode);


/**
 * Perform common resets for test cases. This is preferrably run before the
 * each test case.
 */
void tests_common_reset(void);


/**
 * Join two path segments. This is here to ensure we do NOT have to
 * change all current occurrences of 'path_join' to 'contra_path_join'.
 *
 * @param  segment1
 * @param  segment2
 * @return Joined path
 */
char *path_join(const char *segment1, const char *segment2);


#endif
