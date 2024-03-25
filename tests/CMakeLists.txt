include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(dsun_tests ${TEST_SOURCES} ../src/dsun.h)

target_link_libraries(dsun_tests PRIVATE gtest gtest_main)
enable_testing()
