/**
 * #!gochomugo
 * Meetings with Git
 */
#include "main.h"

int bh_git_repository_manager_new(bh_git_repository_manager **manager,
                                  bh_config *config) {
  int ret_code = 0;
  int i = 0;
  bh_git_repository_manager *mgr = NULL;
  git_strarray remote_names = {0};
  git_remote **remotes = NULL;

  /* NOTE: For some reason, this must come before the malloc() call */
  git_libgit2_init();

  mgr = malloc(sizeof(bh_git_repository_manager));
  if (NULL == mgr) {
    return_err_now(BH_GITERR_NOMEM);
  }

  mgr->repository = NULL;
  mgr->remotes = NULL;
  mgr->origin = NULL;
  mgr->num_of_remotes = 0;

  contra_assert_not_null(config->path);
  return_err(git_repository_open(&(mgr->repository), config->path));

  return_err(git_remote_list(&remote_names, mgr->repository));

  mgr->num_of_remotes = remote_names.count;

  remotes = malloc(sizeof(git_remote *) * mgr->num_of_remotes);
  if (NULL == remotes) {
    return_err(1); // TODO
  }

  mgr->remotes = remotes;

  for (i = 0; i < remote_names.count; i++) {
    char *remote_name = remote_names.strings[i];

    return_err(
        git_remote_lookup(&(mgr->remotes[i]), mgr->repository, remote_name));

    if (0 == strcmp(remote_name, "origin")) {
      mgr->origin = mgr->remotes[i];
    }
  }

  *manager = mgr;

on_error:
  /** free(remotes) - bh_git_repository_manager_free() does it for us */
  if (NULL != mgr)
    bh_git_repository_manager_free(&mgr);
  *manager = NULL;
cleanup:
  /** free(remotes) - bh_git_repository_manager_free() does it for us, once
   * user frees the manager after use */
  git_strarray_free(&remote_names);
  return ret_code;
}

void bh_git_repository_manager_free(bh_git_repository_manager **manager) {
  bh_git_repository_manager *mgr = *manager;
  if (NULL == mgr)
    return;

  if (NULL != mgr->remotes) {
    int i;
    for (i = 0; i < mgr->num_of_remotes; i++) {
      git_remote_free(mgr->remotes[i]);
    }
    free(mgr->remotes);
  }

  /* free(mgr->origin) - this is already done by the loop
   * above */
  if (NULL != mgr->repository)
    git_repository_free(mgr->repository);
  if (NULL != mgr)
    free(mgr);
  *manager = NULL;
  git_libgit2_shutdown();
}

int credentials_cb(git_cred **out, const char *url,
                   const char *username_from_url, unsigned int allowed_types,
                   void *payload) {
  return git_cred_ssh_key_from_agent(out, username_from_url);
}

int bh_git_fetch_origin(bh_git_repository_manager *manager) {
  int ret_code = 0;
  git_fetch_options options = GIT_FETCH_OPTIONS_INIT;
  const git_error *err = NULL;

  options.callbacks.credentials = credentials_cb;

  ret_code = git_remote_fetch(manager->origin, NULL, &options, NULL);
  err = giterr_last();
  if (NULL != err) {
    if (NULL != strstr(err->message, "Connection refused") ||
        NULL != strstr(err->message, "Name or service not known") ||
        NULL != strstr(err->message, "Network is unreachable") ||
        NULL != strstr(err->message, "Failed getting banner")) {
      return_err_now(BH_GITERR_PULL_ORIGIN);
    }
  }
  return_err(ret_code);

on_error:
cleanup:
  return ret_code;
}

int bh_git_merge_origin(git_commit **out, bh_git_repository_manager *manager) {
  int ret_code;
  git_oid current_commit_id, new_commit_id;
  git_commit *current_commit = NULL, *new_commit = NULL;
  git_index *current_index = NULL, *new_index = NULL;
  git_oid new_tree_id;
  git_tree *new_tree = NULL;
  git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

  /** Get the OID of the current commit */
  return_err(git_reference_name_to_id(&current_commit_id, manager->repository,
                                      "HEAD"));

  /** Get the OID of the new commit */
  return_err(git_reference_name_to_id(&new_commit_id, manager->repository,
                                      "refs/remotes/origin/master"));

  /** Exit early, if the commit OIDs are the same */
  if (git_oid_equal(&current_commit_id, &new_commit_id)) {
    *out = NULL;
    return_ok(0);
  }

  /* Get the current commit */
  return_err(git_commit_lookup(&current_commit, manager->repository,
                               &current_commit_id));

  /** Get the new commit */
  return_err(
      git_commit_lookup(&new_commit, manager->repository, &new_commit_id));

  /** Merge the two commit, producing a new index */
  return_err(git_merge_commits(&new_index, manager->repository, current_commit,
                               new_commit, NULL));

  /** Exit with an error, if we had conflicts */
  if (1 == git_index_has_conflicts(new_index)) {
    return_err_now(BH_GITERR_MERGE_CONFLICTS);
  }

  /** Writing the new tree pointed to, by the new index, producing the OID of
   * the new tree */
  return_err(
      git_index_write_tree_to(&new_tree_id, new_index, manager->repository));

  /** Read the new tree into memory */
  return_err(git_tree_lookup(&new_tree, manager->repository, &new_tree_id));

  /** Get the index backed by the repository */
  return_err(git_repository_index(&current_index, manager->repository));

  /** Read the new tree into the repository's index */
  return_err(git_index_read_tree(current_index, new_tree));

  /** Checkout the index */
  checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
  ret_code =
      git_checkout_index(manager->repository, current_index, &checkout_opts);
  if (0 != ret_code && NULL != giterr_last() &&
      NULL != strstr(giterr_last()->message, "conflict prevents checkout")) {
    return_err_now(BH_GITERR_MERGE_CONFLICTS);
  }
  return_err(ret_code);

  /** Update the master reference to point to the new commit */
  ret_code = bh_git__update_master_ref(manager, &new_commit_id, "merge-update");
  return_err(ret_code);

  /** Write the updated index to disc */
  ret_code = git_index_write(current_index);
  return_err(ret_code);

  /** Leave the commit to the caller */
  *out = new_commit;

on_error:
  *out = NULL;
cleanup:
  if (NULL != current_index)
    git_index_free(current_index);
  if (NULL != new_index)
    git_index_free(new_index);
  if (NULL != current_commit)
    git_commit_free(current_commit);
  /** git_commit_free(new_commit) - this is the 'out' */
  if (NULL != new_tree)
    git_tree_free(new_tree);
  return ret_code;
}

int bh_git_commit_changes(git_commit **new_commit,
                          bh_git_repository_manager *manager,
                          const git_commit *base_commit,
                          git_signature *daemon_signature) {
  int ret_code = 0;
  git_oid new_commit_id;
  git_commit *current_commit = NULL;
  git_oid new_tree_id;
  git_tree *current_tree = NULL, *new_tree = NULL;
  git_index *current_index = NULL;
  git_diff *diffs = NULL;
  git_diff_stats *diff_stats = NULL;
  git_signature *host_signature = NULL;

  /** If a NULL base commit was provided, we use the commit pointed to by HEAD
   */
  if (NULL == base_commit) {
    ret_code = bh_git__current_commit_lookup(&current_commit, manager);
    base_commit = (const git_commit *)current_commit;
  }

  /** Get the current index */
  return_err(git_repository_index(&current_index, manager->repository));

  /** Add all the changes */
  return_err(git_index_add_all(current_index, NULL, GIT_INDEX_ADD_DEFAULT, NULL,
                               NULL));

  /** Get the tree pointed to by the base commit */
  return_err(git_commit_tree(&current_tree, base_commit));

  /** Check for diffs between the current index and the above tree */
  return_err(git_diff_tree_to_index(&diffs, manager->repository, current_tree,
                                    NULL, NULL));

  /** Get the diff stats */
  return_err(git_diff_get_stats(&diff_stats, diffs));

  /** Exit early, if there are no changes */
  if (0 == git_diff_stats_files_changed(diff_stats)) {
    return_err_now(BH_GITERR_NO_CHANGES);
  }

  /** Write tree, using the index with changes, to disc */
  return_err(git_index_write_tree(&new_tree_id, current_index));

  /** Get the newly-created tree, into memory */
  return_err(git_tree_lookup(&new_tree, manager->repository, &new_tree_id));

  /** Read the new tree into the repository's index */
  return_err(git_index_read_tree(current_index, new_tree));

  /** Get the default signature, to use as the host signature */
  return_err(git_signature_default(&host_signature, manager->repository));

  /** Write the updated index to disc */
  return_err(git_index_write(current_index));

  /** Create a new commit */
  return_err(git_commit_create_v(&new_commit_id, manager->repository, "HEAD",
                                 host_signature, daemon_signature, NULL,
                                 "backup", new_tree, 1, current_commit));

  /** Update the master ref */
  return_err(
      bh_git__update_master_ref(manager, &new_commit_id, "commit-changes"));

  /** Return the new commit */
  return_err(
      git_commit_lookup(new_commit, manager->repository, &new_commit_id));

on_error:
  *new_commit = NULL;
cleanup:
  if (NULL != current_commit)
    git_commit_free(current_commit);
  if (NULL != current_tree)
    git_tree_free(current_tree);
  if (NULL != current_index)
    git_index_free(current_index);
  if (NULL != diffs)
    git_diff_free(diffs);
  if (NULL != diff_stats)
    git_diff_stats_free(diff_stats);
  if (NULL != host_signature)
    git_signature_free(host_signature);
  return ret_code;
}

int bh_git_push(bh_git_repository_manager *manager, git_reference *current_ref,
                int origin_only) {
  int ret_code = 0;
  int index;
  git_strarray refspecs = {0};
  git_push_options options = GIT_PUSH_OPTIONS_INIT;

  if (NULL == current_ref) {
    return_err(git_repository_head(&current_ref, manager->repository));
  }

  options.callbacks.credentials = credentials_cb;

  /* Init the refspecs */
  refspecs.strings = malloc(sizeof(char *) * 1);
  if (NULL == refspecs.strings) {
    return_err_now(BH_GITERR_NOMEM);
  }

  /** Add the refspecs */
  return_err(
      asprintf(&refspecs.strings[0], "%s", git_reference_name(current_ref)));
  refspecs.count = 1;

  /** First, push to 'origin', as it's the main branch */
  return_err_ext(git_remote_push(manager->origin, &refspecs, &options),
                 BH_GITERR_PUSH_ORIGIN);

  if (false == origin_only) {
    for (index = 0; index < manager->num_of_remotes; index++) {
      /* Do not push to 'origin' again */
      if (manager->remotes[index] == manager->origin)
        continue;

      return_err_ext(
          git_remote_push(manager->remotes[index], &refspecs, &options),
          BH_GITERR_PUSH_REMOTE);
    }
  }

on_error:
cleanup:
  if (0 != refspecs.count)
    git_strarray_free(&refspecs);
  return ret_code;
}

int bh_git_checkout_branch(bh_git_repository_manager *manager,
                           const char *branch_name) {
  int ret_code = 0;
  git_commit *current_commit = NULL;
  git_reference *machine_branch_ref = NULL;
  char *ref_name = NULL;

  /** Get the current commit */
  return_err(bh_git__current_commit_lookup(&current_commit, manager));

  /** Create the local machine branch */
  return_err(git_branch_create(&machine_branch_ref, manager->repository,
                               branch_name, current_commit, true));

  /** Resolve the reference name */
  return_err(asprintf(&ref_name, "refs/heads/%s", branch_name));

  /** Update HEAD to point to the new branch */
  return_err(git_repository_set_head(manager->repository, ref_name));

  /** Checkout the branch */
  /** Set upstream */

on_error:
cleanup:
  if (NULL != current_commit)
    git_commit_free(current_commit);
  if (NULL != machine_branch_ref)
    git_reference_free(machine_branch_ref);
  if (NULL != ref_name)
    free(ref_name);
  return ret_code;
}

int bh_git_is_dirty(int *out, bh_git_repository_manager *manager) {
  int ret_code = 0;
  git_diff *diff = NULL;
  git_diff_options diff_options = GIT_DIFF_OPTIONS_INIT;
  git_diff_stats *diff_stats = NULL;
  int is_dirty = false;

  diff_options.flags |= GIT_DIFF_INCLUDE_UNTRACKED;

  /** Diff between HEAD and workdir */
  return_err(git_diff_index_to_workdir(&diff, manager->repository, NULL,
                                       &diff_options));

  /** Get the diff stats */
  return_err(git_diff_get_stats(&diff_stats, diff));

  /** If there are no changes */
  if (0 < git_diff_stats_files_changed(diff_stats)) {
    is_dirty = true;
  }

  *out = is_dirty;

on_error:
cleanup:
  if (NULL != diff_stats)
    git_diff_stats_free(diff_stats);
  if (NULL != diff)
    git_diff_free(diff);
  return ret_code;
}

int bh_git__current_commit_lookup(git_commit **current_commit,
                                  bh_git_repository_manager *manager) {
  int ret_code = 0;
  git_oid current_commit_id;

  return_err(git_reference_name_to_id(&current_commit_id, manager->repository,
                                      "HEAD"));

  return_err(git_commit_lookup(current_commit, manager->repository,
                               &current_commit_id));

on_error:
cleanup:
  return ret_code;
}

int bh_git__update_ref(bh_git_repository_manager *manager, const char *ref_name,
                       git_oid *commit_id, const char *reflog) {
  int ret_code = 0;
  git_reference *current_master_ref = NULL;
  git_reference *real_master_ref = NULL;
  git_reference *new_master_ref = NULL;

  /** Get the reference to the 'master' branch */
  return_err(
      git_reference_lookup(&current_master_ref, manager->repository, ref_name));

  /** Resolve the reference. It might be symbolic */
  return_err(git_reference_resolve(&real_master_ref, current_master_ref));

  /** Update this reference to point to the commit */
  return_err(git_reference_set_target(&new_master_ref, real_master_ref,
                                      commit_id, reflog));

on_error:
cleanup:
  if (NULL != current_master_ref)
    git_reference_free(current_master_ref);
  if (NULL != real_master_ref)
    git_reference_free(real_master_ref);
  if (NULL != new_master_ref)
    git_reference_free(new_master_ref);
  return ret_code;
}

int bh_git__update_master_ref(bh_git_repository_manager *manager,
                              git_oid *commit_id, const char *reflog) {
  return bh_git__update_ref(manager, "HEAD", commit_id, reflog);
}
