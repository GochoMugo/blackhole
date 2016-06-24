#!/usr/bin/env bash

set -e

# This script checks to ensure we are on the 'new' branch
#
# How? It uses `git branch` to list the branches and checks for '* new'

source "${TEST_DIR_DATA_GIT}/_variables.sh"

git branch --list | grep '* new' > /dev/null
