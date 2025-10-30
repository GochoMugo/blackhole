#!/usr/bin/env bash

set -e

counters_dir="${TEST_DIR}/data/bh-counter/.blackhole/counters"

# remove counters incremented in previous test runs
rm -f "${counters_dir}"/*.tmp.counter

# create an unreadable counter
counter_unreadable="${counters_dir}/unreadable.tmp.counter"
touch "${counter_unreadable}"
chmod a-r "${counter_unreadable}"

# create a binary file as counter
counter_binary="${counters_dir}/binary.tmp.counter"
gcc -o "${counter_binary}" "${TEST_DIR}/data/noop.c"
