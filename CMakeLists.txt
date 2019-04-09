cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)

# Add sailfishc libs. TODO: write this more eloquently
ADD_LIBRARY(SailfishcLibs 
    ./src/lexar/Lexar.cpp
    ./src/lexar/Lexar2.cpp
    ./src/lexar/Token2.cpp
    ./src/parser/Parser.cpp
    ./src/parser/Parser2.cpp
    ./src/parser/Lexeme.cpp
    ./src/visitor/Visitor.cpp
    ./src/semantics/SymbolTable.cpp
    ./src/semantics/TypeChecker.cpp
    ./src/errorhandler/SemanticErrorHandler.cpp 
    ./src/errorhandler/SymbolTableErrorHandler.cpp 
    ./src/errorhandler/ParserErrorHandler.cpp
     ./src/errorhandler/Parser2ErrorHandler.cpp
    ./src/common/ReservedWords.cpp
    ./src/main/CommandLine.cpp
    ./src/transpiler/Transpiler.cpp
)

add_executable(sailfishc ./src/main/main.cpp ${DirSOURCES})
target_link_libraries(sailfishc SailfishcLibs pthread)
