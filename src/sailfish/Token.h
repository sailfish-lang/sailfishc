/*
 * Robert Durst 2019
 * Marmot Programming Language
 */
#pragma once
#include <string>

enum Kind
{
    START_TOKEN,
    IDENTIFIER_TOKEN,
    INTEGER_TOKEN,
    FLOAT_TOKEN,
    OPERATION_TOKEN,
    LOGIC_TOKEN,
    COMMENT_TOKEN,
    BYTE_TOKEN,
    STRING_TOKEN,
    ASSIGNMENT_TOKEN,
    CHAR_TOKEN,
    ARROW_TOKEN,
    UNDERSCORE_TOKEN,
    COMMA_TOKEN,
    LBRACKET_TOKEN,
    RBRACKET_TOKEN,
    LCURLEY_TOKEN,
    RCURLEY_TOKEN,
    LPAREN_TOKEN,
    RPAREN_TOKEN,
    KEYWORD_TOKEN,
    ERROR_TOKEN,
    EOF_TOKEN,
};

class Token
{
  private:
    std::string value;
    Kind kind;

  public:
    // constructor
    Token(Kind, std::string);
    // debugging
    void display();
    // for looping through
    bool isEOF();
};