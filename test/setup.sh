#!/usr/bin/env bash

git_init=(
    data/bh-hook
)


for repo in ${git_init}
do
    [[ -d "${repo}/.git" ]] && continue
    git init "${repo}"
done
