#!/usr/bin/env bash

set -e

# This script pushes changes to the origin and resets back.

source "${TEST_DIR_DATA_GIT}/_variables.sh"

commit_hash="$(git log --format='%H')"
./push-new.sh
git reset "${commit_hash}" > /dev/null
