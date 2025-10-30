#!/usr/bin/env bash

set -e

# This script pushes a new file to the remote

source "${TEST_DIR_DATA_GIT}/_variables.sh"


./touch.sh
git add "${file_new}"
git commit --message "${file_new}" > /dev/null
