cmake_minimum_required(VERSION 4.1)
project(blackhole)

# modules
include(FindPkgConfig)

# options
option(CROSS "cross-compiling" OFF)

# general configurations
set(CMAKE_C_FLAGS "--std=c99 -Wall -pedantic")
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(bin_dir ${CMAKE_CURRENT_BINARY_DIR})
set(test_dir ${root_dir}/test)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
if (CROSS)
    link_directories(/lib/aarch64-linux-gnu)
endif()
list(APPEND deps contra curl)

# iniparser
include_directories(deps/iniparser/src)
link_directories(deps/iniparser/build)
add_custom_target(build-iniparser
    COMMAND mkdir -p build
        && cd build
        && cmake ..
        && make all
    WORKING_DIRECTORY ${root_dir}/deps/iniparser)
list(APPEND deps iniparser)

# libgit2
pkg_check_modules(LIBSSH2 REQUIRED libssh2)
include_directories(deps/libgit2/include)
link_directories(deps/libgit2/build ${LIBSSH2_LIBRARY_DIRS})
add_custom_target(build-libgit2
    COMMAND mkdir -p build
        && cd build
        && cmake .. -DUSE_SSH=ON
        && make
    WORKING_DIRECTORY ${root_dir}/deps/libgit2)
list(APPEND deps git2 ssh2 z)

# macos quirks.
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    list(APPEND deps "iconv -framework Security" "-framework CoreFoundation")
endif()
# cross-compilation quirks.
if (CROSS)
    list(APPEND deps crypto)
endif()

# library
include_directories(deps)
file(GLOB src_lib src/*.c deps/**/*.c)
list(FILTER src_lib EXCLUDE REGEX src/bh.c)
add_library(blackhole ${src_lib})

# executable
set(src_exe src/bh.c)
add_executable(bh ${src_exe})
target_link_libraries(bh blackhole ${deps})
add_dependencies(bh build-iniparser build-libgit2 blackhole)
install(TARGETS bh RUNTIME)

# test config
set(test_deps blackhole ${deps} cmocka)
file(GLOB test_src_exe test/*.c)

# task: clean-tests
add_custom_target(clean-tests
    COMMAND ROOT_DIR=${root_dir} ${root_dir}/script/clean-tests.sh
    COMMENT "Cleaning test artefacts"
)

# task: gen-test-headers
add_custom_target(gen-test-headers
    COMMAND ${root_dir}/script/gen-test-headers.sh ${test_src_exe}
    COMMENT "Generating test header files")

# task: test.config
add_executable(config.test EXCLUDE_FROM_ALL test/config.test.c)
target_link_libraries(config.test ${test_deps})
add_dependencies(config.test blackhole gen-test-headers)
add_custom_target(test.config COMMAND TEST_DIR=${test_dir} ${bin_dir}/config.test)
add_dependencies(test.config config.test)

# task: test.counter
add_executable(counter.test EXCLUDE_FROM_ALL test/counter.test.c)
target_link_libraries(counter.test ${test_deps})
add_dependencies(counter.test blackhole gen-test-headers)
add_custom_target(test.counter.pre COMMAND TEST_DIR=${test_dir} ${test_dir}/setup/counter.setup.sh)
add_custom_target(test.counter COMMAND TEST_DIR=${test_dir} ${bin_dir}/counter.test)
add_dependencies(test.counter counter.test test.counter.pre)

# task: test.daemon
add_executable(daemon.test EXCLUDE_FROM_ALL test/daemon.test.c)
target_link_libraries(daemon.test ${test_deps})
add_dependencies(daemon.test blackhole gen-test-headers)
add_custom_target(test.daemon.pre COMMAND TEST_DIR=${test_dir} ${test_dir}/setup/daemon.setup.sh)
add_custom_target(test.daemon COMMAND TEST_DIR=${test_dir} ${bin_dir}/daemon.test)
add_dependencies(test.daemon daemon.test test.daemon.pre)

# task: test.error
add_executable(error.test EXCLUDE_FROM_ALL test/error.test.c)
target_link_libraries(error.test ${test_deps})
add_dependencies(error.test blackhole gen-test-headers)
add_custom_target(test.error COMMAND TEST_DIR=${test_dir} ${bin_dir}/error.test)
add_dependencies(test.error error.test)

# task: test.git
add_executable(git.test EXCLUDE_FROM_ALL test/git.test.c)
target_link_libraries(git.test ${test_deps})
add_dependencies(git.test blackhole gen-test-headers)
add_custom_target(test.git COMMAND TEST_DIR=${test_dir} ${bin_dir}/git.test)
add_dependencies(test.git git.test)

# task: test.hook
add_executable(hook.test EXCLUDE_FROM_ALL test/hook.test.c)
target_link_libraries(hook.test ${test_deps})
add_dependencies(hook.test blackhole gen-test-headers)
add_custom_target(test.hook.pre COMMAND TEST_DIR=${test_dir} ${test_dir}/setup/hook.setup.sh)
add_custom_target(test.hook COMMAND TEST_DIR=${test_dir} ${bin_dir}/hook.test)
add_dependencies(test.hook hook.test test.hook.pre)

# task: test.print
add_executable(print.test EXCLUDE_FROM_ALL test/print.test.c)
target_link_libraries(print.test ${test_deps})
add_dependencies(print.test blackhole gen-test-headers)
# compile run.c
add_executable(print.test.out EXCLUDE_FROM_ALL test/data/bh-print/run.c)
target_link_libraries(print.test.out ${test_deps})
add_dependencies(print.test.out blackhole)
# run run.c
add_custom_target(test.print.pre COMMAND BIN_DIR=${bin_dir} TEST_DIR=${test_dir} ${test_dir}/setup/print.setup.sh)
add_dependencies(test.print.pre print.test.out)
add_custom_target(test.print COMMAND TEST_DIR=${test_dir} ${bin_dir}/print.test)
add_dependencies(test.print print.test test.print.pre)

# task: test.status
add_executable(status.test EXCLUDE_FROM_ALL test/status.test.c)
target_link_libraries(status.test ${test_deps})
add_dependencies(status.test blackhole gen-test-headers)
# compile run.c
add_executable(status.test.out EXCLUDE_FROM_ALL test/data/bh-status/run.c)
target_link_libraries(status.test.out ${test_deps})
add_dependencies(status.test.out blackhole)
# run run.c
add_custom_target(test.status.pre COMMAND BIN_DIR=${bin_dir} TEST_DIR=${test_dir} ${test_dir}/setup/status.setup.sh)
add_dependencies(test.status.pre status.test.out)
add_custom_target(test.status COMMAND TEST_DIR=${test_dir} ${bin_dir}/status.test)
add_dependencies(test.status status.test test.status.pre)

# task: tests
add_custom_target(test)
add_dependencies(test
    test.config
    test.counter
    test.daemon
    test.error
    test.git
    test.hook
    test.print
    test.status
)
