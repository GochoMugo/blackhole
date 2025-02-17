/**
 * #!gochomugo
 * I have got a Type! /cc @RaeSremmurd
 */
#ifndef _BH_types_h_
#define _BH_types_h_ 1

/**
 * Repository manager. This acts as a handle that can be
 * passed to other git functions to work on the attached
 * repository.
 */
typedef struct {
  /** repository being managed */
  git_repository *repository;

  /** remote repositories to push changes to, including the origin */
  git_remote **remotes;

  /** number of remote repositories attached, above */
  int num_of_remotes;

  /** the 'origin' remote repository */
  git_remote *origin;
} bh_git_repository_manager;

/**
 * Configuration of the daemon.
 */
typedef struct {
  /** Path to the blackhole directory */
  const char *path;

  /** The name of the daemon */
  const char *name;

  /** The email address, to be used by daemon */
  const char *email;

  /** Path to the 'runstate' directory */
  const char *runstate_path;

  /** The original dictionary loaded from disk, by the iniparser */
  const dictionary *dict;
} bh_config;

/**
 * Paths used by the daemon
 */
typedef struct {
  /** counters */
  char *counters;

  /** hooks */
  char *hooks;
} bh_daemon_paths;

/**
 * Daemon information for this instance
 */
typedef struct {
  /** Configuration of the daemon */
  bh_config *config;

  /** Paths to files/directories used by the daemon */
  bh_daemon_paths paths;

  /** Repository manager */
  bh_git_repository_manager *manager;

  /** Git signature */
  git_signature *signature;
} bh_daemon;

#endif
