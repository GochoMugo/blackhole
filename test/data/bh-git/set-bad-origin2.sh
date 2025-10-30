#!/usr/bin/env bash

set -e

# This script sets a bad remote

source "${TEST_DIR_DATA_GIT}/_variables.sh"

git remote set-url origin2 "${bad_url}"
