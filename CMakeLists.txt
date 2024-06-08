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
add_executable(test_forward_list test/test_forward_list.cpp)
add_executable(test_list test/test_list.cpp)
add_executable(test_stack test/test_stack.cpp)
add_executable(test_queue test/test_queue.cpp)

#
target_link_libraries(test_vector GTest::GTest GTest::Main)
target_link_libraries(test_array GTest::GTest GTest::Main)
target_link_libraries(test_forward_list GTest::GTest GTest::Main)
target_link_libraries(test_list GTest::GTest GTest::Main)
target_link_libraries(test_stack GTest::GTest GTest::Main)
target_link_libraries(test_queue GTest::GTest GTest::Main)

#
add_test(NAME test_vector COMMAND test_vector)
add_test(NAME test_array COMMAND test_array)
add_test(NAME test_forward_list COMMAND test_forward_list)
add_test(NAME test_list COMMAND test_list)
add_test(NAME test_stack COMMAND test_stack)
add_test(NAME test_queue COMMAND test_queue)
