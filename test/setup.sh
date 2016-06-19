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


noop="data/noop"
if [[ ! -e "${noop}" ]]
then
    ${CC} -o "${noop}" "${noop}.c"
fi


# remove counters incremented in previous test runs
rm -f data/bh-counter/.blackhole/counters/*.tmp.counter
