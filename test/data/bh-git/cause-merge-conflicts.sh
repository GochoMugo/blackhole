#!/usr/bin/env bash

set -e

# This script cause merge-conflicts to occur when trying to merge the
# origin branch and the local branch.
#
# How? It writes to a file and the pushes to origin. Then, resets back to
# the previous commit. Writes totally different content to the same file.

source "${TEST_DIR_DATA_GIT}/_variables.sh"


./push-new-and-reset.sh
echo "
XXX
YYY
" > new.txt
