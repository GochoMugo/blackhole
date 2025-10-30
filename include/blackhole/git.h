/**
 * #!gochomugo
 * Meetings with Git
 */
#ifndef _BH_git_h_
#define _BH_git_h_ 1

/**
 * Create a new repository manager. This creates the underlying
 * structures and it is up to the caller to free the manager,
 * with `bh_git_repo_manager_free()` to avoid memory leak.
 *
 * @param manager Newly-created repository manager
 * @param config Daemon configuration
 * @return 0 on success, or error code
 */
int bh_git_repository_manager_new(bh_git_repository_manager **manager,
                                  bh_config *config);

/**
 * Free a repository manager. This frees the internal structures
 * of the manager. Ensure to call this for each created manager
 * to avoid memory leak.
 *
 * @param manager Repository manager
 */
void bh_git_repository_manager_free(bh_git_repository_manager **manager);

/**
 * Fetch any changes from 'origin'.
 *
 * @param manager Repository manager
 * @return 0 on success, BH_GITERR_NO_UPDATES if no updates were found or error
 * code
 */
int bh_git_fetch_origin(bh_git_repository_manager *manager);

/**
 * Merge any changes from the main remote-tracking branch, usually
 * 'refs/remotes/origin/master'. It is expected that the merge is
 * a fast-forward, otherwise this will fail. If it fails, the
 * repository is returned to the state before the merge attempt.
 *
 * @param out The new commit pointed by HEAD
 * @param manager Repository manager
 * return 0 on success, BH_GITERR_MERGE_CONFLICTS if conflicts occur or any
 * other error code
 */
int bh_git_merge_origin(git_commit **out, bh_git_repository_manager *manager);

/**
 * Commit local changes in the repository. This adds and updates files,
 * respecting the ignore rules.
 *
 * @param new_commit Newly-created commit
 * @param manager Repository manager
 * @param commit Commit from `bh_git_merge_origin()`. If NULL, use commit
 * pointed by HEAD
 * @return 0 on success, BH_GITERR_NO_CHANGES if no local changes are available
 * or error code
 */
int bh_git_commit_changes(git_commit **new_commit,
                          bh_git_repository_manager *manager,
                          const git_commit *base_commit,
                          git_signature *daemon_signature);

/**
 * Push changes to the remotes. If 'origin_only' is true, changes are only
 * pushed to the origin. Otherwise, changes are pushed to all remotes.
 *
 * @param manager Repository manager
 * @param current_ref Reference to the current branch. Assign NULL to use HEAD.
 * @param origin_only Whether to push to 'origin' only
 * @return 0 on success, BH_GITERR_PUSH_ORIGIN, BH_GITERR_PUSH_REMOTE or other
 * error code
 */
int bh_git_push(bh_git_repository_manager *manager, git_reference *current_ref,
                int origin_only);

/**
 * Create a new branch for this daemon and check it out.
 *
 * @param manager Repository manager
 * @param branch_name Name of the branch
 * @return 0 or error code
 */
int bh_git_checkout_branch(bh_git_repository_manager *manager,
                           const char *branch_name);

/**
 * Check if Repository is dirty.
 *
 * @param is_dirty The destination of the status
 * @param manager Repository manager
 * return 0 or error code
 */
int bh_git_is_dirty(int *is_dirty, bh_git_repository_manager *manager);

/**
 * Get the current commit in the repository.
 *
 * @param current_commit The current commit
 * @param manager Repository manager
 * @return 0 or error code
 */
int bh_git__current_commit_lookup(git_commit **current_commit,
                                  bh_git_repository_manager *manager);

/**
 * Update the reference with the name 'name' to point to a commit.
 *
 * @param manager Repository manager
 * @param reference_name Name of the reference e.g. HEAD, refs/heads/master,
 * etc.
 * @param commit_id OID of the commit
 * @param reflog messaged to be used in reflog
 * @return 0 or error code
 */
int bh_git__update_ref(bh_git_repository_manager *manager, const char *ref_name,
                       git_oid *commit_id, const char *reflog);

/**
 * Update the master reference to point to a commit.
 *
 * @param manager Repository manager
 * @param commit_id OID of the commit
 * @param reflog message to be used in the reflog
 * @return 1 or error code
 */
int bh_git__update_master_ref(bh_git_repository_manager *manager,
                              git_oid *commit_id, const char *reflog);

#endif
