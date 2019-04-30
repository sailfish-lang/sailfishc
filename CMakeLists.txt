cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)
# set (CMAKE_CXX_FLAGS "-g")

# Add sailfishc libs. TODO: write this more eloquently
ADD_LIBRARY(SailfishcLibs 
    ./src/lexar/Lexar.cpp
    ./src/lexar/Token.cpp
    ./src/transpiler/transpiler.cpp
    ./src/sailfish/sailfishc.cpp
    ./src/errorhandler/ParserErrorHandler.cpp
    ./src/errorhandler/SemanticAnalyzerErrorHandler.cpp 
    ./src/main/CommandLine.cpp
    ./src/semantics/SymbolTable.cpp
    ./src/stdlib_c/stdlib_c.cpp
    ./src/stdlib_c/Lists.cpp
    ./src/tests/SemanticAnalysisTest.cpp
)

add_executable(sailfishc ./src/main/main.cpp ${DirSOURCES})
target_link_libraries(sailfishc SailfishcLibs pthread)
