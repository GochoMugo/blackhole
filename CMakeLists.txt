cmake_minimum_required(VERSION 3.6)
project(blackhole)

# general configurations
add_definitions(-DLIBINIPARSER_VERSION="?")
set(CMAKE_C_FLAGS "--std=c99")
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(deps iniparser git2)
set(test_deps blackhole ${deps} cmocka)

# library
include_directories(deps)
file(GLOB src_lib src/*.c deps/**/*.c)
list(REMOVE_ITEM src_lib bh.c)
add_library(blackhole ${src_lib})

# executable
set(src_exe ${src_lib} src/bh.c)
add_executable(bh ${src_exe})
target_link_libraries(bh ${deps})
add_dependencies(bh blackhole)

# tests
file(GLOB test_src_exe test/*.c)
add_custom_target(pre-bh-tests
    COMMAND ${root_dir}/script/gen-test-headers.sh ${test_src_exe}
    COMMENT "generating test header files")
add_executable(bh-tests ${test_src_exe})
target_link_libraries(bh-tests ${test_deps})
add_dependencies(bh-tests blackhole pre-bh-tests)

# task: running tests
add_custom_target(run-tests
    COMMAND BIN_DIR=${CMAKE_CURRENT_BINARY_DIR}
        ./setup.sh
    COMMAND TEST_DIR=${root_dir}/test
        ${CMAKE_CURRENT_BINARY_DIR}/bh-tests
    WORKING_DIRECTORY ${root_dir}/test)
add_dependencies(run-tests bh-tests)

# task: cleaning after tests
add_custom_target(clean-tests
    COMMAND SRC_DIR=${root_dir} ${root_dir}/script/clean-tests.sh)
