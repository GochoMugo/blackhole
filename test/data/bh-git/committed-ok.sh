#!/usr/bin/env bash

set -e

# This script checks to ensure a commit occurred.
#
# How? By default, we have only a single commit. If a commit did happen,
# we should have 2 commits instead

source "${TEST_DIR_DATA_GIT}/_variables.sh"

[[ "$(git log --oneline | wc -l)" -eq 2 ]] && true || false
