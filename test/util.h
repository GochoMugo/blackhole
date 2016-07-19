#ifndef _BH_tests_util_h_
#define _BH_tests_util_h_ 1


/**
 * Assert that the 'executable' produces the same output as recorded
 * in the output file at 'output_filepath'.
 *
 * @param executable The executable to be run
 * @param output_filepath Path to a txt file with expected output
 */
void assert_output_equal(const char *executable, const char *output_filepath);

#endif
