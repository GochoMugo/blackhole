#!/usr/bin/env bash

set -e

# This script creates a new file

source "${TEST_DIR_DATA_GIT}/_variables.sh"


echo "
AAA
BBB
" > "${file_new}"
