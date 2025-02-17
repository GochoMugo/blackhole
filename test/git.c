#include "main.h"

#define get_state()                                                            \
  state_t *s = (state_t *)*state;                                              \
  execute_script(path_script_reset);
#define skip_if_no_network()                                                   \
  if (true == no_network)                                                      \
    skip();

static int no_network = -1;
static bh_daemon *daemon_git = NULL;
static bh_config *config = NULL;
static const char *path = NULL;
static const char *path_script_reset = NULL;
typedef struct {
  bh_git_repository_manager *manager;
} state_t;

void execute_script(const char *path_script) {
  int ret_code = -1;
  assert_non_null(path_script);
  ret_code = system(path_script);
  assert_int_not_equal(ret_code, -1);
  if (0 != WEXITSTATUS(ret_code)) {
    printf(" [X] script failed: %s\n", path_script);
    fail();
  }
}

void file_exists(const char *file_path) {
  struct stat file_stats;
  assert_non_null(file_path);
  assert_ok(stat(file_path, &file_stats));
}

int tests_bh_git_setup_each(void **state) {
  char *test_dir = NULL;
  char *no_network_env = NULL;
  state_t *new_state;

  new_state = (state_t *)malloc(sizeof(state_t));
  assert_non_null(new_state);

  if (-1 == no_network) {
    no_network_env = getenv("TEST_NO_NETWORK");
    no_network = NULL == no_network_env ? false : true;
  }
  assert_int_not_equal(no_network, -1);

  if (NULL == path) {
    test_dir = getenv("TEST_DIR");
    path = path_join(test_dir, "data/bh-git");
    setenv("TEST_DIR_DATA_GIT", path, 1);
  }
  assert_non_null(path);

  if (NULL == path_script_reset) {
    path_script_reset = path_join(path, "reset.sh");
  }
  assert_non_null(path_script_reset);

  if (NULL == daemon_git) {
    int ret_code = bh_daemon_new(&daemon_git, path);
    config = daemon_git->config;
  }
  assert_non_null(daemon_git);
  assert_non_null(config);

  assert_ok(bh_git_repository_manager_new(&(new_state->manager), config));
  assert_non_null(new_state->manager);

  *state = new_state;
  tests_common_reset();
  return 0;
}

int tests_bh_git_teardown_each(void **state) {
  state_t *s = (state_t *)*state;
  bh_git_repository_manager_free(&(s->manager));
  free(s);
  bh_error_clear();
  return 0;
}

/**
 * `bh_git_repository_manager_new()` creates a new repository manager.
 * Check all the properties to ensure they are set correctly.
 */
void tests_bh_git_repository_manager_new_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_repository_manager_new_ok");
  get_state();
  assert_non_null(s->manager->repository);
  assert_non_null(s->manager->remotes);
  assert_non_null(s->manager->origin);
  assert_int_equal(s->manager->num_of_remotes, 2);
}

/**
 * `bh_git_repository_new()` returns an error 'BH_GITERR_NULL_PATH' if
 * a null path is given.
 */
void tests_bh_git_repository_manager_new_null_path(void **state) {
  skip_if_filtered_out("tests_bh_git_repository_manager_new_null_path");
  bh_git_repository_manager *new_manager;
  bh_config *new_config = NULL;
  assert_ok(bh_config_new(&new_config, path));
  assert_non_null(new_config);
  new_config->path = NULL;
  assert_int_equal(bh_git_repository_manager_new(&new_manager, new_config),
                   BH_GITERR_NULL_PATH);
  assert_null(new_manager);
  bh_git_repository_manager_free(&new_manager);
  bh_config_free(&new_config);
}

/**
 * `bh_git_repository_free()` must free the repository manager.
 * Ensure it is NULL afterwards.
 */
void tests_bh_git_repository_free_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_repository_free_ok");
  get_state();
  bh_git_repository_manager_free(&(s->manager));
  assert_null(s->manager);
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
  get_state();
  execute_script(path_join(path, "push-new-and-reset.sh"));
  assert_ok(bh_git_fetch_origin(s->manager));
  file_exists(path_join(path, "new.txt"));
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
  get_state();
  execute_script(path_join(path, "set-bad-origin.sh"));
  bh_git_repository_manager *new_manager = NULL;
  assert_ok(bh_git_repository_manager_new(&new_manager, config));
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
  get_state();
  execute_script(path_join(path, "push-3.sh"));
  git_commit *new_commit = NULL;
  assert_ok(bh_git_merge_origin(&new_commit, s->manager));
  assert_non_null(new_commit);
  execute_script(path_join(path, "merged-ok-3.sh"));
}

/**
 * `bh_git_merge_origin()` does nothing if there are no changes to merge.
 */
void tests_bh_git_merge_origin_equal(void **state) {
  skip_if_filtered_out("tests_bh_git_merge_origin_equal");
  skip_if_no_network();
  get_state();
  git_commit *new_commit;
  assert_ok(bh_git_merge_origin(&new_commit, s->manager));
  assert_null(new_commit);
}

/**
 * `bh_git_merge_origin()` return error BH_GITERR_MERGE_CONFLICTS if
 * merge conflicts occur.
 */
void tests_bh_git_merge_origin_conflicts(void **state) {
  skip_if_filtered_out("tests_bh_git_merge_origin_conflicts");
  skip_if_no_network();
  get_state();
  execute_script(path_join(path, "cause-merge-conflicts.sh"));
  git_commit *new_commit;
  assert_int_equal(bh_git_merge_origin(&new_commit, s->manager),
                   BH_GITERR_MERGE_CONFLICTS);
  assert_null(new_commit);
}

/**
 * `bh_git_commit_changes()` commits any uncommitted changes.
 * Create a new file and run the function. A new commit should be created.
 */
void tests_bh_git_commit_changes_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_commit_changes_ok");
  get_state();
  execute_script(path_join(path, "touch.sh"));
  git_commit *new_commit = NULL;
  assert_ok(bh_git_commit_changes(&new_commit, s->manager, NULL,
                                  daemon_git->signature));
  assert_non_null(new_commit);
  execute_script(path_join(path, "committed-ok.sh"));
}

/**
 * `bh_git_commit_changes()` does nothing if there are no changes to commit.
 */
void tests_bh_git_commit_changes_unchanged(void **state) {
  skip_if_filtered_out("tests_bh_git_commit_changes_unchanged");
  get_state();
  git_commit *new_commit;
  assert_int_equal(bh_git_commit_changes(&new_commit, s->manager, NULL,
                                         daemon_git->signature),
                   BH_GITERR_NO_CHANGES);
}

/**
 * `bh_git_push()` pushes changes to origin and remotes.
 * Create a new file and run function. Check to see if pushing occurred.
 */
void tests_bh_git_push_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_push_ok");
  skip_if_no_network();
  get_state();
  execute_script(path_join(path, "commit-new.sh"));
  assert_ok(bh_git_push(s->manager, NULL, false));
  execute_script(path_join(path, "pushed-ok.sh"));
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
  get_state();
  execute_script(path_join(path, "set-bad-origin.sh"));
  bh_git_repository_manager *new_manager = NULL;
  assert_ok(bh_git_repository_manager_new(&new_manager, config));
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
  get_state();
  execute_script(path_join(path, "set-bad-remote.sh"));
  bh_git_repository_manager *new_manager = NULL;
  assert_ok(bh_git_repository_manager_new(&new_manager, config));
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
  get_state();
  execute_script(path_join(path, "commit-new.sh"));
  assert_ok(bh_git_push(s->manager, NULL, true));
  execute_script(path_join(path, "origin-remote-unsync.sh"));
}

/**
 * `bh_git_checkout_branch()` checkouts the branch.
 */
void tests_bh_git_checkout_branch_ok(void **state) {
  skip_if_filtered_out("tests_bh_git_checkout_branch_ok");
  get_state();
  assert_ok(bh_git_checkout_branch(s->manager, "new"));
  execute_script(path_join(path, "on-new-branch.sh"));
}

/**
 * `bh_git_is_dirty()` returns true if directory is dirty.
 * Create a new file and check if it is dirty.
 */
void tests_bh_git_is_dirty_true(void **state) {
  skip_if_filtered_out("tests_bh_git_is_dirty_true");
  get_state();
  execute_script(path_join(path, "touch.sh"));
  int dirty = false;
  assert_ok(bh_git_is_dirty(&dirty, s->manager));
  assert_int_equal(dirty, true);
}

/**
 * `bh_git_is_dirty()` returns false if directory is not dirty.
 * Just check now.
 */
void tests_bh_git_is_dirty_false(void **state) {
  skip_if_filtered_out("tests_bh_git_is_dirty_false");
  get_state();
  int dirty = -1;
  assert_ok(bh_git_is_dirty(&dirty, s->manager));
  assert_int_equal(dirty, false);
}
