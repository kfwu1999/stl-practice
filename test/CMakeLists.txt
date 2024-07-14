#-------------------------------------------------------------------------------
#  - Google Test Setup
#-------------------------------------------------------------------------------
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 
file(GLOB TEST_SOURCES "*.cpp")

#
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} GTest::GTest GTest::Main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
