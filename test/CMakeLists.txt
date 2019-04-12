cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)

# Add sailfishc libs. TODO: write this more eloquently
ADD_LIBRARY(SailfishcLibs 
    ../src/lexar/Lexar2.cpp
    ../src/lexar/Token2.cpp
    ../src/parser/Parser2.cpp
    ../src/parser/Lexeme.cpp
    ../src/semantics/SymbolTable.cpp 
    ../src/errorhandler/Parser2ErrorHandler.cpp 
    ../src/errorhandler/SemanticAnalyzerErrorHandler.cpp 
)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(sailfishc_tests tests.cpp ${DirSOURCES})
target_link_libraries(sailfishc_tests ${GTEST_LIBRARIES} SailfishcLibs pthread)