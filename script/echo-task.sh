#!/usr/bin/env bash
# echo task being run in the Makefile

task=${1}
files=(${@:2})
len=${#files[@]}

for (( i=0, first=0, last=1; i<$len; i++, first=1, last=($len-$i-2) ))
do
    real_path=$(readlink -f "${files[$i]}")
    rootdir=$(dirname "${BASH_SOURCE[0]}")
    rootdir=$(dirname "${rootdir}")
    rootdir=$(readlink -f "${rootdir}")
    endlabel="+"
    if [[ "${len}" -eq 1 ]] || [[ "${last}" -eq 0 ]] ; then endlabel=""; fi
    echo -e "    \033[0;32m${task} \033[1;37m${real_path#${rootdir}/}\e[0m ${endlabel}"
done
