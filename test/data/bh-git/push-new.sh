#!/usr/bin/env bash

set -e

# This script pushes a new file to the remote

source "${TEST_DIR_DATA_GIT}/_variables.sh"

./commit-new.sh
retry git push origin master > /dev/null 2>&1 || {
    echo " [X] pushing to origin failed (${BASH_SOURCE[0]})"
    false
}
