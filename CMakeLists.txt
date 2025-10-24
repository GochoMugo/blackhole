cmake_minimum_required(VERSION 4.1)
project(blackhole)

# modules
include(FindPkgConfig)

# options
option(CROSS "cross-compiling" OFF)

# general configurations
set(CMAKE_C_FLAGS "--std=c99 -Wall -pedantic")
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})

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
        && cmake .. -DBUILD_SHARED_LIBS=OFF -DBUILD_CLAR=OFF -DCURL=OFF -DUSE_OPENSSL=OFF
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

# test headers
set(test_deps blackhole ${deps} cmocka)
file(GLOB test_src_exe test/*.c)
add_custom_target(gen-test-headers
    COMMAND ${root_dir}/script/gen-test-headers.sh ${test_src_exe}
    COMMENT "generating test header files")

# test binary
add_executable(bh-tests EXCLUDE_FROM_ALL ${test_src_exe})
target_link_libraries(bh-tests ${test_deps})
add_dependencies(bh-tests blackhole gen-test-headers)

# test setup
get_target_property(test_include_dirs bh-tests INCLUDE_DIRECTORIES)
    list(TRANSFORM test_include_dirs PREPEND "-I")
get_target_property(test_link_dirs bh-tests LINK_DIRECTORIES)
    list(TRANSFORM test_link_dirs PREPEND "-L")
get_target_property(test_link_libs bh-tests LINK_LIBRARIES)
    list(TRANSFORM test_link_libs PREPEND "-l")
    list(TRANSFORM test_link_libs REPLACE "-l-framework" "-framework")
    separate_arguments(test_link_libs)
add_custom_target(setup-tests
    COMMAND BIN_DIR=${CMAKE_CURRENT_BINARY_DIR}
    INCLUDE_DIRS="${test_include_dirs}"
    LIBS="${test_link_libs}"
    LINK_DIRS="${test_link_dirs}"
        ./setup.sh
    WORKING_DIRECTORY ${root_dir}/test)

# run tests
add_custom_target(run-tests
    COMMAND TEST_DIR=${root_dir}/test
        ${CMAKE_CURRENT_BINARY_DIR}/bh-tests
    WORKING_DIRECTORY ${root_dir}/test)
add_dependencies(run-tests bh-tests setup-tests)

# clean after tests
add_custom_target(clean-tests
    COMMAND ROOT_DIR=${root_dir} ${root_dir}/script/clean-tests.sh)
