include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(dsun_tests ${TEST_SOURCES})

target_include_directories(dsun_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(dsun_tests PRIVATE dsun gtest gtest_main)
add_dependencies(dsun_tests dsun)
enable_testing()
