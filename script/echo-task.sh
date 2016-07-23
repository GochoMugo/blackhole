#!/usr/bin/env bash
# echo task being run in the Makefile

task=${1}
for path in ${@:2}
do
    real_path=$(readlink -f "${path}")
    rootdir=$(dirname "${BASH_SOURCE[0]}")
    rootdir=$(dirname "${rootdir}")
    rootdir=$(readlink -f "${rootdir}")
    echo -e "    \033[0;32m${task} \033[1;37m${real_path#${rootdir}/}\e[0m"
done
