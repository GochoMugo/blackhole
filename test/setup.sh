#!/usr/bin/env bash

git_init=(
    data/bh-hook
    data/bh-daemon
)

# remove counters incremented in previous test runs
rm -f data/bh-counter/.blackhole/counters/*.tmp.counter

# initialing repositories
for repo in ${git_init[@]}
do
    [[ -d "${repo}/.git" ]] && continue
    git init "${repo}"
done




counter_unreadable="data/bh-counter/.blackhole/counters/unreadable.tmp.counter"
touch "${counter_unreadable}"
chmod -r "${counter_unreadable}"


exes=(
    "data/noop"
    "data/bh-status/run"
)
for exe in ${exes[@]}
do
    [[ -e "${exe}.out" ]] && [[ "${exe}.out" -nt "${exe}.c" ]] && continue
    ${CC} -o "${exe}.out" "${exe}.c" ${CFLAGS} ${LIBS}
done
