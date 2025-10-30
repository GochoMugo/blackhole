#!/usr/bin/env bash

set -e


repo_dir="${TEST_DIR}/data/bh-daemon"

# Initialize git repo.
rm -rf "${repo_dir}/.git"
git init --quiet "${repo_dir}"
