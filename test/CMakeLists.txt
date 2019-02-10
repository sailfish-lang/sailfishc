cmake_minimum_required(VERSION 2.6)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(sailfishc_tests tests.cpp)
target_link_libraries(sailfishc_tests ${GTEST_LIBRARIES} pthread)