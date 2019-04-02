/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Token2.h"
#include <fstream>
#include <memory>

class Lexar2
{
  private:
    std::ifstream file;
    int line;
    int col;
    int prevCol; // for when we need to jump back a line and remember where the
                 // end of that line was
    char getNextChar();
    std::unique_ptr<Token2> makeToken(const Tokenn::Kind&, const std::string&);
    std::unique_ptr<Token2> makeTokenPutback(const Tokenn::Kind&, std::string&,
                                             char&);

    enum State
    {
        START,
        IDENTIFIER,
        INTEGER,
        FLOAT,
        DIVISION,
        ADDITION,
        SUBTRACTION,
        STRING,
        STRING_ESCAPE,
        ASSIGNMENT,
        NEGATION,
        MULTIPLICATION,
        LESS_THAN,
        GREATER_THAN,
        DOUBLE_DOT,
        TRIPLE_DOT,
        COMMENT,
        ERROR
    };

  public:
    Lexar2(const std::string& filename);
    std::unique_ptr<Token2> getNextToken();
};