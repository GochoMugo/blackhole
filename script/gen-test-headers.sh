#!/usr/bin/env bash

each_file() {

src_file="${1}"
basedir="$(dirname "${1}")"
c_filename="$(basename "${1}")"
header_file="${basedir}/$(echo "${c_filename}" | sed -e s/\.c/\.h/ )"
module_name="$(echo "${c_filename}" | sed -e s/\.c// )"
funcs="$(cat "${src_file}" | grep 'void tests_' | sed -e s/\ {/\;/ )"

{
    echo "/* Automatically generated by script/gen-test-headers.sh */"
    echo "#ifndef _BH_tests_${module_name}_h_"
    echo "#define _BH_tests_${module_name}_h_ 1"
    echo
    echo "#define setup_each    tests_bh_${module_name}_setup_each"
    echo "#define teardown_each tests_bh_${module_name}_teardown_each"
    echo
    echo "int tests_bh_${module_name}_setup_each(void **state);"
    echo "int tests_bh_${module_name}_teardown_each(void **state);"
    echo
    echo "${funcs}"
    echo
    echo
    echo "static const struct CMUnitTest tests_bh_${module_name}[] = {"
    while read func
    do
        echo "    cmocka_unit_test_setup_teardown($(echo "${func}" | sed -e 's/void\ //g' -e 's/(.*);//'), setup_each, teardown_each),"
    done <<< "${funcs}"
    echo "};"
    echo
    echo
    echo "#undef setup_each"
    echo "#undef teardown_each"
    echo
    echo "#endif"
} > "${header_file}"

}

for f in ${*}
do
    # skip 'main.c'; it does not contain test cases
    [[ "$(basename "${f}")" == "main.c" ]] && continue
    each_file "${f}"
done
