#!/usr/bin/env bash
# print out filename of any file without 'test!skip-gen' in the first 10 lines

for f in ${*}
do
    if [[ -z "$(head -n 10 "${f}" | grep 'test!skip-gen')" ]]
    then
        echo "${f}" | sed -e s/\.c$/\.h\ / | tr -d '\n'
    fi
done
