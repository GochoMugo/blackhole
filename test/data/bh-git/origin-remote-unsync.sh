#!/usr/bin/env bash

set -e

# This script checks if the remote and origin are out of sync.
#
# How? It compares 'ref/remotes/origin/master' and 'ref/remotes/test.master'
# to see if they contain the same OID.

source "${TEST_DIR_DATA_GIT}/_variables.sh"

[[ "$(cat "${root_dir}/.git/refs/remotes/origin/master")" != "$(cat "${root_dir}/.git/refs/remotes/test/master")" ]]
