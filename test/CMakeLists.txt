cmake_minimum_required(VERSION 2.6)

# Add sailfishc libs. TODO: write this more eloquently
ADD_LIBRARY(SailfishcLibs 
    ../src/lexar/Lexar.cpp
    ../src/lexar/Scanner.cpp
    ../src/lexar/Token.cpp
    ../src/parser/Parser.cpp
    ../src/visitor/InOrderTraversal.cpp
    ../src/visitor/Visitor.cpp
)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(sailfishc_tests tests.cpp ${DirSOURCES})
target_link_libraries(sailfishc_tests ${GTEST_LIBRARIES} SailfishcLibs pthread)