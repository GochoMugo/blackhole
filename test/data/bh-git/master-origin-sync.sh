#!/usr/bin/env bash

set -e

# This script checks if the local master and the origin are in sync.
#
# How? It compares 'refs/remotes/origin/master' and 'refs/heads/master'
# to see if they contain the same OID.

source "${TEST_DIR_DATA_GIT}/_variables.sh"

[[ "$(cat "${root_dir}/.git/refs/remotes/origin/master")" == "$(cat "${root_dir}/.git/refs/heads/master")" ]]
