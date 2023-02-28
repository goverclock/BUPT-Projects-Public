cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
project(compilation_principle)

file(GLOB_RECURSE SRC_FILES src/*.cpp )
add_executable(main ${SRC_FILES})

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()


# the following two lines are just for command 'ctest'
# include(GoogleTest)
# gtest_discover_tests(hello_test)
