#!/usr/bin/env bash
# cleaning up test artefacts

rm -rf "${SRC_DIR}"/test/data/**/*.tmp.counter \
       "${SRC_DIR}"/test/data/**/*.git \
       "${SRC_DIR}"/test/data/**/*.out \
       "${SRC_DIR}"/test/data/*.out \
       `"${SRC_DIR}"/script/which-test-headers.sh "${SRC_DIR}"/test/*.c` \
       "${SRC_DIR}"/test/data/bh-git/.gitignore
