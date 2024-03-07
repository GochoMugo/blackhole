#!/usr/bin/env bash
# cleaning up test artefacts

rm -rf bh-tests \
       "${ROOT_DIR}"/test/data/**/*.tmp.counter \
       "${ROOT_DIR}"/test/data/**/*.git \
       "${ROOT_DIR}"/test/data/**/*.out \
       "${ROOT_DIR}"/test/data/*.out \
       $("${ROOT_DIR}"/script/which-test-headers.sh "${ROOT_DIR}"/test/*.c) \
       "${ROOT_DIR}"/test/data/bh-git/.gitignore
