cmake_minimum_required(VERSION 3.28)
project(dsun)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++20")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_library(dsun STATIC src/dsun.h)

set_target_properties(dsun PROPERTIES LINKER_LANGUAGE CXX)
include_directories(src)
add_subdirectory(examples)
# Tests
add_subdirectory(tests)

include(CTest)
add_test(NAME dsun_tests COMMAND $<TARGET_FILE:dsun_tests> --gtest_filter=* --gtest_color=no)