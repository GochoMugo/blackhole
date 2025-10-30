#!/usr/bin/env bash

set -e

data_dir="${TEST_DIR}/data/bh-print"

# run the binary
"${BIN_DIR}/print.test.out" > "${data_dir}/stdout.tmp.txt" 2> "${data_dir}/stderr.tmp.txt"
