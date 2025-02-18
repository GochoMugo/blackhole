/**
 * #!gochomugo
 * What the Fuck's happening
 */
#include "main.h"

int bh_status_print(bh_daemon *daemon) {
  int ret_code = 0;
  const char *branch_name = NULL;
  int pull_errors = -1;
  int push_origin_errors = -1;
  int push_remote_errors = -1;
  int is_dirty = false;
  git_reference *current_ref = NULL;

  return_err(git_repository_head(&current_ref, daemon->manager->repository));

  return_err(git_branch_name(&branch_name, current_ref));

  return_err(bh_counter_get(&pull_errors, daemon->paths.counters,
                            BH_EVENT_PULLERRORS));

  return_err(bh_counter_get(&push_origin_errors, daemon->paths.counters,
                            BH_EVENT_PUSHORIGINERRORS));

  return_err(bh_counter_get(&push_remote_errors, daemon->paths.counters,
                            BH_EVENT_PUSHREMOTEERRORS));

  return_err(bh_git_is_dirty(&is_dirty, daemon->manager));

  puts("");
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", "name");
  cc_fprintf(CC_FG_WHITE, stdout, "%s\n", daemon->config->name);
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", "rootdir");
  cc_fprintf(CC_FG_WHITE, stdout, "%s\n", daemon->config->path);
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", "branch");
  cc_fprintf(CC_FG_WHITE, stdout, "%s\n", branch_name);
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", "dirty");
  cc_fprintf(CC_FG_WHITE, stdout, "%s\n", true == is_dirty ? "yes" : "no");
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", BH_EVENT_PULLERRORS);
  cc_fprintf(CC_FG_WHITE, stdout, "%d\n", pull_errors);
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", BH_EVENT_PUSHORIGINERRORS);
  cc_fprintf(CC_FG_WHITE, stdout, "%d\n", push_origin_errors);
  cc_fprintf(CC_FG_CYAN, stdout, "%20s : ", BH_EVENT_PUSHREMOTEERRORS);
  cc_fprintf(CC_FG_WHITE, stdout, "%d\n", push_remote_errors);
  puts("");

on_error:
cleanup:
  if (NULL != current_ref)
    git_reference_free(current_ref);
  /** if (NULL != branch_name) free(branch_name); - git_reference_free()
   * above does it for us */
  return ret_code;
}
