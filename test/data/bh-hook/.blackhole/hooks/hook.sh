#!/usr/bin/env bash
# This script is run by the `bh_hook_exec()` function.
# It tests the script arguments and exit with return code 0 if all are valid.
# Otherwise, it exits with a return code of 1.


# ${1} is the message. We are using the string 'test message'.
actual_message="${1}"
expected_message="test message"
[[ "${actual_message}" == "${expected_message}" ]] || {
    echo "wrong message to hook:"
    echo "  expected: ${expected_message}"
    echo "  actual:   ${actual_message}"
    exit 1
}


# ${2} is the name of the daemon. We are using the name 'test'.
actual_name="${2}"
expected_name="test"
[[ "${actual_name}" == "${expected_name}" ]] || {
    echo "wrong daemon name to hook:"
    echo "  expected: ${expected_name}"
    echo "  actual:   ${actual_name}"
    exit 1
}


# ${3} is the path to the repository. It must be the parent directory.
actual_path="$(readlink -f "${3}")"
expected_path="$(readlink -f "`dirname ${0}`/../..")"
[[ "${actual_path}" == "${expected_path}" ]] || {
    echo "wrong path to hook:"
    echo "  expected: ${expected_path}"
    echo "  actual:   ${actual_path}"
    exit 1
}


exit 0
