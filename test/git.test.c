#include "git.test.h"

#define _assert_file_exists(path) assert_file_exists(path); free(path);
#define _execute_script(path) execute_script(path); free(path);
#define _reset() \
  _execute_script(path_join(((TestState *) *state)->path_data_dir, "reset.sh")); \
  _setup_test((TestState *) *state)

static char *TEST_DIR = NULL;

typedef struct {
  bh_daemon *daemon;
  bh_git_repository_manager *repo_manager;
  char *path_data_dir;
  char *path_script_reset;
} TestState;

int tests_bh_git_setup_each(void **state) {
  git_libgit2_init();

  TEST_DIR = getenv("TEST_DIR");
  assert_non_null(TEST_DIR);

  TestState *test_state = (TestState *) malloc(sizeof(TestState));
  assert_non_null(test_state);

  test_state->daemon = NULL;
  test_state->repo_manager = NULL;
  test_state->path_data_dir = NULL;
  test_state->path_script_reset = NULL;

  test_state->path_data_dir = path_join(TEST_DIR, "data/bh-git");
  setenv("TEST_DIR_DATA_GIT", test_state->path_data_dir, 1);

  *state = test_state;
  tests_common_reset();
  return 0;
}

void _setup_test(TestState *test_state) {
  assert_ok(bh_daemon_new(&(test_state->daemon), test_state->path_data_dir));
  assert_non_null(test_state->daemon);

  assert_ok(bh_git_repository_manager_new(&(test_state->repo_manager), test_state->daemon->config));
  assert_non_null(test_state->repo_manager);
}

int tests_bh_git_teardown_each(void **state) {
  TestState *test_state = (TestState *) *state;
  if (NULL != test_state) {
    if (NULL != test_state->repo_manager) bh_git_repository_manager_free(&(test_state->repo_manager));
    if (NULL != test_state->daemon) bh_daemon_free(&(test_state->daemon));
    if (NULL != test_state->path_script_reset) free(test_state->path_script_reset);
    if (NULL != test_state->path_data_dir) free(test_state->path_data_dir);
    free(test_state);
  }
  git_libgit2_shutdown();
  tests_common_reset();
  return 0;
}

/**
 * `bh_git_repository_manager_new()` creates a new repository manager.
 * Check all the properties to ensure they are set correctly.
 */
void tests_bh_git_repository_manager_new_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_repository_manager_new_ok");
  _reset();
  TestState *test_state = (TestState *) *state;
  assert_non_null(test_state->repo_manager->repository);
  assert_non_null(test_state->repo_manager->remotes);
  assert_non_null(test_state->repo_manager->origin);
  assert_int_equal(test_state->repo_manager->num_of_remotes, 2);
}

/**
 * `bh_git_repository_free()` must free the repository manager.
 * Ensure it is NULL afterwards.
 */
void tests_bh_git_repository_free_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_repository_free_ok");
  _reset();
  TestState *test_state = (TestState *) *state;
  bh_git_repository_manager_free(&(test_state->repo_manager));
  assert_null(test_state->repo_manager);
}

/**
 * `bh_git_fetch_origin()` fetches changes from origin.
 * First, create a new file, commit it and push.
 * Then reset to the 1st commit.
 * Now run the function and expect to pull a new file in the commit.
 */
void tests_bh_git_fetch_origin_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_fetch_origin_ok");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "push-new-and-reset.sh"));
  assert_ok(bh_git_fetch_origin(test_state->repo_manager));
  _assert_file_exists(path_join(test_state->path_data_dir, "new.txt"));
}

/**
 * `bh_git_fetch_origin()` returns error 'BH_GITERR_PULL_ORIGIN' if an
 * error occurs pulling from origin.
 * We use a bad URL here.
 * We need to create a new manager since we are changing the remote URL
 * that the old manager has already loaded.
 */
void tests_bh_git_fetch_origin_bad_origin(void **state) {
  skip_if_filtered_out("tests_bh_git_fetch_origin_bad_origin");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "set-bad-origin.sh"));
  bh_git_repository_manager *new_manager = NULL;
  assert_ok(bh_git_repository_manager_new(&new_manager, test_state->daemon->config));
  assert_non_null(new_manager);
  assert_int_equal(bh_git_fetch_origin(new_manager), BH_GITERR_PULL_ORIGIN);
  bh_git_repository_manager_free(&new_manager);
}

/**
 * `bh_git_merge()` works correctly by merging new changes from the origin.
 * Push a new file (commit 2).
 * Update the file and push again (commit 3).
 * Reset to commit 2.
 * Try merging now.
 */
void tests_bh_git_merge_origin_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_merge_origin_ok");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "push-3.sh"));
  git_commit *new_commit = NULL;
  assert_ok(bh_git_merge_origin(&new_commit, test_state->repo_manager));
  assert_non_null(new_commit);
  _execute_script(path_join(test_state->path_data_dir, "merged-ok-3.sh"));
}

/**
 * `bh_git_merge_origin()` does nothing if there are no changes to merge.
 */
void tests_bh_git_merge_origin_equal(void **state) {
  skip_if_filtered_out("tests_bh_git_merge_origin_equal");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  git_commit *new_commit;
  assert_ok(bh_git_merge_origin(&new_commit, test_state->repo_manager));
  assert_null(new_commit);
}

/**
 * `bh_git_merge_origin()` return error BH_GITERR_MERGE_CONFLICTS if
 * merge conflicts occur.
 */
void tests_bh_git_merge_origin_conflicts(void **state) {
  skip_if_filtered_out("tests_bh_git_merge_origin_conflicts");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "cause-merge-conflicts.sh"));
  git_commit *new_commit;
  assert_int_equal(bh_git_merge_origin(&new_commit, test_state->repo_manager),
                   BH_GITERR_MERGE_CONFLICTS);
  assert_null(new_commit);
}

/**
 * `bh_git_commit_changes()` commits any uncommitted changes.
 * Create a new file and run the function. A new commit should be created.
 */
void tests_bh_git_commit_changes_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_commit_changes_ok");
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "touch.sh"));
  git_commit *new_commit = NULL;
  assert_ok(bh_git_commit_changes(&new_commit, test_state->repo_manager, NULL,
                                  test_state->daemon->signature));
  assert_non_null(new_commit);
  _execute_script(path_join(test_state->path_data_dir, "committed-ok.sh"));
}

/**
 * `bh_git_commit_changes()` does nothing if there are no changes to commit.
 */
void tests_bh_git_commit_changes_unchanged(void **state) {
  skip_if_filtered_out("tests_bh_git_commit_changes_unchanged");
  _reset();
  TestState *test_state = (TestState *) *state;
  git_commit *new_commit = NULL;
  assert_int_equal(bh_git_commit_changes(&new_commit, test_state->repo_manager, NULL,
                                         test_state->daemon->signature),
                   BH_GITERR_NO_CHANGES);
  assert_null(new_commit);
}

/**
 * `bh_git_push()` pushes changes to origin and remotes.
 * Create a new file and run function. Check to see if pushing occurred.
 */
void tests_bh_git_push_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_push_ok");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "commit-new.sh"));
  assert_ok(bh_git_push(test_state->repo_manager, NULL, false));
  _execute_script(path_join(test_state->path_data_dir, "pushed-ok.sh"));
}

/**
 * `bh_git_push()` returns error BH_GITERR_PUSH_ORIGIN if can not push to
 * origin.
 * Use a bad url for origin.
 * We need to create a new manager since we are changing the remote URL
 * that the old manager has already loaded.
 */
void tests_bh_git_push_bad_origin(void **state) {
  skip_if_filtered_out("tests_bh_git_push_bad_origin");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "set-bad-origin.sh"));
  bh_git_repository_manager *new_manager = NULL;
  assert_ok(bh_git_repository_manager_new(&new_manager, test_state->daemon->config));
  assert_non_null(new_manager);
  assert_int_equal(bh_git_push(new_manager, NULL, false),
                   BH_GITERR_PUSH_ORIGIN);
  bh_git_repository_manager_free(&new_manager);
}

/**
 * `bh_git_push()` returns error BH_GITERR_PUSH_REMOTE if can not push to
 * remote.
 * Use a bad url for remote.
 * We need to create a new manager since we are changing the remote URL
 * that the old manager has already loaded.
 */
void tests_bh_git_push_bad_remote(void **state) {
  skip_if_filtered_out("tests_bh_git_push_bad_remote");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "set-bad-origin2.sh"));
  bh_git_repository_manager *new_manager = NULL;
  assert_ok(bh_git_repository_manager_new(&new_manager, test_state->daemon->config));
  assert_non_null(new_manager);
  assert_int_equal(bh_git_push(new_manager, NULL, false),
                   BH_GITERR_PUSH_REMOTE);
  bh_git_repository_manager_free(&new_manager);
}

/**
 * `bh_git_push()` can also avoid pushing to remotes.
 * Push to origin and check if the origin and remote get out of sync.
 */
void tests_bh_git_push_dont_remote(void **state) {
  skip_if_filtered_out("tests_bh_git_push_dont_remote");
  skip_if_no_network();
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "commit-new.sh"));
  assert_ok(bh_git_push(test_state->repo_manager, NULL, true));
  _execute_script(path_join(test_state->path_data_dir, "origins-unsynced.sh"));
}

/**
 * `bh_git_checkout_branch()` checkouts the branch.
 */
void tests_bh_git_checkout_branch_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_checkout_branch_ok");
  _reset();
  TestState *test_state = (TestState *) *state;
  assert_ok(bh_git_checkout_branch(test_state->repo_manager, "new"));
  _execute_script(path_join(test_state->path_data_dir, "on-new-branch.sh"));
}

/**
 * `bh_git_is_dirty()` returns true if directory is dirty.
 * Create a new file and check if it is dirty.
 */
void tests_bh_git_is_dirty_true(void **state) {
  skip_if_filtered_out("tests_bh_git_is_dirty_true");
  _reset();
  TestState *test_state = (TestState *) *state;
  _execute_script(path_join(test_state->path_data_dir, "touch.sh"));
  int dirty = false;
  assert_ok(bh_git_is_dirty(&dirty, test_state->repo_manager));
  assert_int_equal(dirty, true);
}

/**
 * `bh_git_is_dirty()` returns false if directory is not dirty.
 * Just check now.
 */
void tests_bh_git_is_dirty_false(void **state) {
  skip_if_filtered_out("tests_bh_git_is_dirty_false");
  _reset();
  TestState *test_state = (TestState *) *state;
  int dirty = -1;
  assert_ok(bh_git_is_dirty(&dirty, test_state->repo_manager));
  assert_int_equal(dirty, false);
}
