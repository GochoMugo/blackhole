#!/usr/bin/env bash

set -e

# This script checks to ensure we pushed to the origin and remote okay.
#
# How? It ensures we committed and a change and compares
# 'refs/heads/master', 'refs/remotes/origin/master' and
# 'refs/remotes/origin2/master'

source "${TEST_DIR_DATA_GIT}/_variables.sh"

[[ "$(cat "${root_dir}/.git/refs/heads/master")" == "$(cat "${root_dir}/.git/refs/remotes/origin/master")" ]] && true || false
[[ "$(./origins-unsynced.sh && echo true || echo false)" == "false" ]] && true || false
