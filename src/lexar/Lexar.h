/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Scanner.h"
#include "Token.h"
#include <string>

// These states are utilized by the lexar as a sort of DFA.
enum State
{
    START,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    DIVISION_OR_COMMENT,
    SINGLE_LINE_COMMENT,
    MULTIPLE_LINE_COMMENT_PRESTATE,
    MULTIPLE_LINE_COMMENT,
    BYTE,
    STRING,
    STRING_ESCAPE,
    SUBTRACTION,
    ASSIGNMENT,
    NEGATION,
    MULTIPLICATION,
    AND_PRESTATE,
    OR_PRESTATE,
    LESS_THAN,
    GREATER_THAN,
    DOUBLE_DOT_PRESTATE,
    TRIPLE_DOT_PRESTATE,
    RBRACKET_PRESTATE,
    ERROR
};

class Lexar
{
  private:
    Scanner* scanner;
    int currentLineNum;
    int currentColNum;

  public:
    // constructor
    Lexar(std::string filename);
    // destructor
    ~Lexar()
    {
        delete scanner;
    };
    // will return EOF_TOKEN indefinitely once done parsing
    Token* getNextToken();
};
