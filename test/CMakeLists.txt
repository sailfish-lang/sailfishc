cmake_minimum_required(VERSION 2.6)

# Add sailfishc libs. TODO: write this more eloquently
ADD_LIBRARY(SailfishcLibs 
    ../src/lexar/Lexar.cpp
    ../src/lexar/Lexar2.cpp
    ../src/lexar/Token2.cpp
    ../src/parser/Parser.cpp
    visitors/InOrderTraversal.cpp
    ../src/visitor/Visitor.cpp
    ../src/semantics/SymbolTable.cpp 
    ../src/semantics/TypeChecker.cpp 
    ../src/errorhandler/SemanticErrorHandler.cpp 
    ../src/errorhandler/SymbolTableErrorHandler.cpp 
    ../src/errorhandler/ParserErrorHandler.cpp
    ../src/common/ReservedWords.cpp
)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(sailfishc_tests tests.cpp ${DirSOURCES})
target_link_libraries(sailfishc_tests ${GTEST_LIBRARIES} SailfishcLibs pthread)