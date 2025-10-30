#!/usr/bin/env bash

set -e

# This script pushes a new file to the remote

source "${TEST_DIR_DATA_GIT}/_variables.sh"

echo 1 > "${file_new}"
git add "${file_new}"
git commit -m "commit#1" > /dev/null
commit_hash="$(git log --format='%H' | head -n 1)"

echo 2 >> "${file_new}"
git add "${file_new}"
git commit -m "commit#2" > /dev/null

retry git push origin master > /dev/null 2>&1 || {
    echo " [X] pushing to origin failed (${BASH_SOURCE[0]})"
    false
}

git reset "${commit_hash}" > /dev/null
git checkout . --quiet
