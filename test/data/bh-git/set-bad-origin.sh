#!/usr/bin/env bash

set -e

# This script sets a bad origin

source "${TEST_DIR_DATA_GIT}/_variables.sh"

git remote set-url origin "${bad_url}"
