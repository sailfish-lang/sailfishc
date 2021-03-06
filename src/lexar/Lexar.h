/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * Lexar encapsulates the logic for retreiving tokens one by one from a given
 * source code text file.
 */
#pragma once
#include "Token.h"
#include <fstream>
#include <memory>

class Lexar
{
  private:
    std::ifstream file;
    int line;
    int col;
    int prevCol; // for when we need to jump back a line and remember where the
                 // end of that line was
    bool isFile; // may have a raw string instead of a file
    std::string rawString;
    char getNextChar();
    std::unique_ptr<Token> makeToken(const TokenKind&, const std::string&);
    std::unique_ptr<Token> makeTokenPutback(const TokenKind&, std::string&,
                                            char&);
    // represents dfa states in our pseudo dfa/state machine implementation
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
        LIST,
        ERROR
    };

  public:
    Lexar(const std::string&, bool);
    std::unique_ptr<Token> getNextToken();
};