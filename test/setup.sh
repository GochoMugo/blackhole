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
    git init --quiet "${repo}"
done


git_commit=(
    "data/bh-status"
)
for repo in ${git_commit[@]}
do
    pushd "${repo}" > /dev/null
    git rev-parse HEAD > /dev/null 2>&1 && { popd > /dev/null; continue; }
    touch .gitkeep
    git add .gitkeep
    git commit -m "root commit (for testing)" > /dev/null
    popd > /dev/null
done


counter_unreadable="data/bh-counter/.blackhole/counters/unreadable.tmp.counter"
touch "${counter_unreadable}"
chmod -r "${counter_unreadable}"


exes=(
    "data/noop"
    "data/bh-print/run"
    "data/bh-status/run"
)
for exe in ${exes[@]}
do
    [[ -e "${exe}.out" ]] && [[ "${exe}.out" -nt "${exe}.c" ]] && continue
    echo ${BIN_DIR}
    gcc -o "${exe}.out" "${exe}.c" -L"${BIN_DIR}" -lblackhole `pkg-config --libs libgit2 libiniparser`
done
