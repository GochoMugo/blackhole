#!/usr/bin/env bash

set -e

# This script resets the repository in preparation for the
# next unit test case. It returns the repository to an expected state
# before each test case.
# This allows us to re-use a single repository
# rather than create multiple, separate repos for each test case.

source "${TEST_DIR_DATA_GIT}/_variables.sh"

rm -rf .git/ *.txt
git init . > /dev/null

rm -rf .git/hooks

git remote add origin "${origin_url}"
git remote add test "${remote_url}"

cp gitignore .gitignore
git add .gitignore
git commit -m "Root commit" > /dev/null

if [[ -z "${TEST_NO_NETWORK}" ]]
then
    retry git push origin master --force > /dev/null 2>&1 || {
        echo " [X] pushing to origin failed (${BASH_SOURCE[0]})"
        false
    }
    retry git push test master --force > /dev/null 2>&1 || {
        echo " [X] pushing to remote failed (${BASH_SOURCE[0]})"
        false
    }
fi
