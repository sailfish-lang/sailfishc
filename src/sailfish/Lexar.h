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
    ADDITION,
    SUBTRACTION,
    ASSIGNMENT,
    NEGATION,
    MULTIPLICATION,
    AND_PRESTATE,
    OR_PRESTATE,
    RANGE_PRESTATE,
    LESS_THAN,
    GREATER_THAN,
    ERROR
};

class Lexar
{
  private:
    Scanner* scanner;

  public:
    // constructor
    Lexar(std::string filename);
    // will return EOF_TOKEN indefinitely once done parsing
    Token* getNextToken();
};
