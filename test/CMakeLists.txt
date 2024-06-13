#-------------------------------------------------------------------------------
#  - Google Test Setup
#-------------------------------------------------------------------------------
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executables
add_executable(test_vector test_vector.cpp)
add_executable(test_array test_array.cpp)
add_executable(test_forward_list test_forward_list.cpp)
add_executable(test_list test_list.cpp)
add_executable(test_stack test_stack.cpp)
add_executable(test_queue test_queue.cpp)

# Link test executables to Google Test libraries
target_link_libraries(test_vector GTest::GTest GTest::Main)
target_link_libraries(test_array GTest::GTest GTest::Main)
target_link_libraries(test_forward_list GTest::GTest GTest::Main)
target_link_libraries(test_list GTest::GTest GTest::Main)
target_link_libraries(test_stack GTest::GTest GTest::Main)
target_link_libraries(test_queue GTest::GTest GTest::Main)

# Add tests to CMake's test suite
add_test(NAME test_vector COMMAND test_vector)
add_test(NAME test_array COMMAND test_array)
add_test(NAME test_forward_list COMMAND test_forward_list)
add_test(NAME test_list COMMAND test_list)
add_test(NAME test_stack COMMAND test_stack)
add_test(NAME test_queue COMMAND test_queue)
