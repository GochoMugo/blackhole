#!/usr/bin/env bash

set -e

# This script ensures the merge occurred successful

source "${TEST_DIR_DATA_GIT}/_variables.sh"

git log --oneline | grep "commit#2" > /dev/null
cat "${file_new}" | grep "2" > /dev/null
