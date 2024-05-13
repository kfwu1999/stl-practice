#-------------------------------------------------------------------------------
#  - Project
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
project(stl-practice)

#-------------------------------------------------------------------------------
#  - Configuration
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd

#-------------------------------------------------------------------------------
#  - lib
#-------------------------------------------------------------------------------
include_directories(include)
add_executable(main main.cpp)


#-------------------------------------------------------------------------------
#  - Google Test Setup
#-------------------------------------------------------------------------------
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#
add_executable(test_vector test/test_vector.cpp)
add_executable(test_array test/test_array.cpp)

#
target_link_libraries(test_vector GTest::GTest GTest::Main)
target_link_libraries(test_array GTest::GTest GTest::Main)

#
add_test(NAME test_vector COMMAND test_vector)
add_test(NAME test_array COMMAND test_array)
