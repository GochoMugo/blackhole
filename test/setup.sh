#!/usr/bin/env bash

git_init=(
    data/bh-hook
    data/bh-daemon
)


# initialing repositories
for repo in ${git_init[@]}
do
    [[ -d "${repo}/.git" ]] && continue
    git init "${repo}"
done


# removing read permissions
chmod -r data/bh-counter/.blackhole/counters/unreadable.counter




# remove counters incremented in previous test runs
rm -f data/bh-counter/.blackhole/counters/*.tmp.counter
exes=(
    "data/noop"
    "data/bh-status/run"
)
for exe in ${exes[@]}
do
    [[ -e "${exe}.out" ]] && [[ "${exe}.out" -nt "${exe}.c" ]] && continue
    ${CC} -o "${exe}.out" "${exe}.c" ${CFLAGS} ${LIBS}
done
