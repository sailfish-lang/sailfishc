cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)
# set (CMAKE_CXX_FLAGS "-g")

# Add sailfishc libs. TODO: write this more eloquently
ADD_LIBRARY(SailfishcLibs 
    ./src/lexar/Lexar2.cpp
    ./src/lexar/Token2.cpp
    ./src/parser/Parser2.cpp
    ./src/parser/Lexeme.cpp
    ./src/errorhandler/Parser2ErrorHandler.cpp
    ./src/errorhandler/SemanticAnalyzerErrorHandler.cpp 
    ./src/main/CommandLine.cpp
    ./src/semantics/SymbolTable.cpp
)

add_executable(sailfishc ./src/main/main.cpp ${DirSOURCES})
target_link_libraries(sailfishc SailfishcLibs pthread)
