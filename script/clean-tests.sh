#!/usr/bin/env bash
# cleaning up test artefacts

rm -rf \
       "${ROOT_DIR}"/test/data/**/*.tmp.{counter,txt} \
       "${ROOT_DIR}"/test/data/**/.git \
       "${ROOT_DIR}"/test/data/**/*.out \
       $("${ROOT_DIR}"/script/which-test-headers.sh "${ROOT_DIR}"/test/*.test.c) \
       "${ROOT_DIR}"/test/data/bh-git/.gitignore
