#!/usr/bin/env bash

set -e


# remove counters incremented in previous test runs
rm -f data/bh-counter/.blackhole/counters/*.tmp.counter


git_init=(
    "data/bh-hook"
    "data/bh-daemon"
    "data/bh-status"
)
for repo in ${git_init[@]}
do
    [[ -d "${repo}/.git" ]] && continue
    git init "${repo}"
done


git_commit=(
    "data/bh-status"
)
for repo in ${git_commit[@]}
do
    pushd "${repo}"
    git rev-parse HEAD > /dev/null 2>&1 && { popd; continue; }
    touch .gitkeep
    git add .gitkeep
    git commit -m "root commit (for testing)"
    popd
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
