cmake_minimum_required(VERSION 3.14)
project(quicksand)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

file(GLOB SRC_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cc"
)

add_library(quicksand_lib ${SRC_SOURCES})

target_include_directories(quicksand_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Tests
enable_testing()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc"
)

add_executable(quicksand_tests ${TEST_SOURCES})

target_link_libraries(quicksand_tests PRIVATE
  quicksand_lib
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(quicksand_tests)
