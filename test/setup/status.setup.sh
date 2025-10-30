#!/usr/bin/env bash

set -e

data_dir="${TEST_DIR}/data/bh-status"

# Initialize git repo.
rm -rf "${data_dir}/.git"
git init --quiet "${data_dir}"
rm -rf "${data_dir}/.git/hooks"

# Create root commit.
pushd "${data_dir}" > /dev/null
touch new.tmp.txt
git add new.tmp.txt
git commit --message "root commit" > /dev/null
popd > /dev/null

# run the binary
TEST_DATA_DIR="${data_dir}" "${BIN_DIR}/status.test.out" > "${data_dir}/stdout.tmp.txt" 2> "${data_dir}/stderr.tmp.txt"
