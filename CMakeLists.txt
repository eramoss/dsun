cmake_minimum_required(VERSION 3.28)
project(dsun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_library(dsun STATIC src/dsun.h)

set_target_properties(dsun PROPERTIES LINKER_LANGUAGE CXX)
include_directories(src)
add_subdirectory(examples)
# Tests
add_subdirectory(tests)

include(CTest)
add_test(NAME dsun_tests COMMAND $<TARGET_FILE:dsun_tests> --gtest_filter=* --gtest_color=no)