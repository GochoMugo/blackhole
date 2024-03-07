#include "main.h"


void common_reset(void) {
    bh_error_clear();
    if (NULL != giterr_last()) giterr_clear();
    errno = 0;

    assert_null(bh_error_get());
    assert_null(giterr_last());
    assert_int_equal(errno, 0);
}


int tests_bh_error_setup_each(void **state) {
    common_reset();
    tests_common_reset();
    return 0;
}


int tests_bh_error_teardown_each(void **state) {
    common_reset();
    return 0;
}


/**
 * We know that `BH_ERR` defines the code for our first error
 * in our error list. Use it to see if the function set the error
 * appropriately.
 */
void tests_bh_error_set_bh(void **state) {
    skip_if_filtered_out("tests_bh_error_set_bh");
    const bh_error *err;
    bh_error_set(BH_ERR);
    err = bh_error_get();
    assert_int_equal(BH_ERR_START, err->code);
    assert_non_null(strstr(err->message, "[BH]"));
}


/**
 * LibGit2 offers the function `giterr_set_str` that allows us to
 * set up an error, which we shall eventually check for.
 * Also, remember, libgit2 returns -1 on errors.
 */
void tests_bh_error_set_libgit2(void **state) {
    skip_if_filtered_out("tests_bh_error_set_libgit2");
    const char *message = "sample error message";
    const bh_error *err;
    giterr_set_str(1, message);
    bh_error_set(-1);
    err = bh_error_get();
    assert_int_equal(err->code, BH_GITERR);
    assert_non_null(strstr(err->message, message));
    assert_non_null(strstr(err->message, "[LIBGIT2]"));
}


/**
 * System errors and other miscellaneous errors, set using `errno`
 * are interpreted using `strerror` (or alike). Set errno to `ENOENT`
 * and check for it.
 * Also, most syscalls return -1 on error.
 */
void tests_bh_error_set_errno(void **state) {
    skip_if_filtered_out("tests_bh_error_set_errno");
    const bh_error *err;
    errno = ENOENT;
    bh_error_set(-1);
    err = bh_error_get();
    assert_int_equal(err->code, BH_ERR);
    assert_non_null(strstr(err->message, "[MISC]"));
}


/**
 * Setting an error, when an error has already been set, has no
 * effect.
 */
void tests_bh_error_set_already_set(void **state) {
    skip_if_filtered_out("tests_bh_error_set_already_set");
    char *message = NULL;
    const bh_error *err;
    bh_error_set(BH_ERR);
    err = bh_error_get();
    assert_int_equal(err->code, BH_ERR);
    asprintf(&message, "%s", err->message);
    bh_error_set(BH_ERR + 1);
    assert_string_equal(err->message, message);;
}


/**
 * Setting an error, using a return code greater than or equal to 0.
 * is ignored.
 */
void tests_bh_error_set_zero(void **state) {
    skip_if_filtered_out("tests_bh_error_set_zero");
    bh_error_set(0);
    assert_null(bh_error_get());
    bh_error_set(1);
    assert_null(bh_error_get());
}


/**
 * Getting an error returns NULL, if it is not set.
 */
void tests_bh_error_get_null_if_unset(void **state) {
    skip_if_filtered_out("tests_bh_error_get_null_if_unset");
    assert_null(bh_error_get());
}


/**
 * Returns pointer to the same, global error variable, if it is set.
 */
void tests_bh_error_get_pointer(void **state) {
    skip_if_filtered_out("tests_bh_error_get_pointer");
    const bh_error *ptr1, *ptr2;

    bh_error_set(BH_ERR_START);
    ptr1 = bh_error_get();
    bh_error_clear();

    bh_error_set(BH_ERR_START + 1);
    ptr2 = bh_error_get();

    assert_ptr_equal(ptr1, ptr2);
}


/**
 * Clearing an error message, resets the error variable.
 */
void tests_bh_error_clear_resets(void **state) {
    skip_if_filtered_out("tests_bh_error_clear_resets");
    const bh_error *err = NULL;

    bh_error_set(BH_ERR);
    err = bh_error_get();
    bh_error_clear();

    assert_int_equal(err->code, 0);
    assert_null(err->message);
}


/**
 * `bh_error_class()` should return `true` for `BH_GITERR`, a generic class,
 * as  a class of `BH_GITERR`.
 */
void tests_bh_error_class_true_for_class_itself(void **state) {
    skip_if_filtered_out("tests_bh_error_class_true_for_class_itself");
    assert_true(bh_error_class(BH_GITERR, BH_GITERR, BH_GITERR_END));
}


/**
 * `bh_error_class()` should return `true` for `BH_GITERR_PULL_ORIGIN`
 * as an error of class `BH_GITERR`.
 */
void tests_bh_error_class_true(void **state) {
    skip_if_filtered_out("tests_bh_error_class_true");
    assert_true(bh_error_class(BH_GITERR_PULL_ORIGIN, BH_GITERR, BH_GITERR_END));
}


/**
 * `bh_error_class()` should return `false` for `BH_GITERR_PULL_ORIGIN`
 * as an error of class `BH_DAEMONERR`.
 */
void tests_bh_error_class_false(void **state) {
    skip_if_filtered_out("tests_bh_error_class_false");
    assert_false(bh_error_class(BH_GITERR_PULL_ORIGIN, BH_DAEMONERR, BH_DAEMONERR_END));
}


/**
 * `bh_error_copy()` copies the alredy-set error into a new error struct.
 */
void tests_bh_error_copy_ok(void **state) {
    skip_if_filtered_out("tests_bh_error_copy_ok");
    bh_error *error = NULL;
    bh_error_set(BH_ERR);
    assert_ok(bh_error_copy(&error));
    assert_int_equal(error->code, BH_ERR);
    assert_string_equal(error->message, bh_error_get()->message);
    bh_error_free(&error);
}


/**
 * `bh_error_copy()` assigns `NULL` if no error is currently set.
 */
void tests_bh_error_copy_null(void **state) {
    skip_if_filtered_out("tests_bh_error_copy_null");
    bh_error *error;
    assert_ok(bh_error_copy(&error));
    assert_null(error);
    bh_error_free(&error);
}


/**
 * `bh_error_free()` frees a copied error struct. And without freeing the
 * original error struct.
 */
void tests_bh_error_free_frees(void **state) {
    skip_if_filtered_out("tests_bh_error_free_frees");
    bh_error *error = NULL;
    bh_error_set(BH_ERR);
    assert_ok(bh_error_copy(&error));
    assert_non_null(error);
    bh_error_free(&error);
    assert_null(error);
    assert_non_null(bh_error_get());
}
