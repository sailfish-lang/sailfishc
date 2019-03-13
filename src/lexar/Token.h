/*
 * Robert Durst 2019
 * Marmot Programming Language
 */
#pragma once
#include "../common/ReservedWords.h"
#include <string>

// Kind represents all the different types of tokens.
enum Kind
{
    START_TOKEN,
    OWN_ACCESSOR_TOKEN,
    IDENTIFIER_TOKEN,
    INTEGER_TOKEN,
    FLOAT_TOKEN,
    BOOL_TOKEN,
    OPERATION_TOKEN,
    LOGIC_TOKEN,
    COMMENT_TOKEN,
    STRING_TOKEN,
    ARROW_TOKEN,
    UNDERSCORE_TOKEN,
    COMMA_TOKEN,
    LBRACKET_TOKEN,
    RBRACKET_TOKEN,
    LCURLEY_TOKEN,
    RCURLEY_TOKEN,
    LPAREN_TOKEN,
    RPAREN_TOKEN,
    LFISH_TAIL_TOKEN,
    RFISH_TAIL_TOKEN,
    KEYWORD_TOKEN,
    ERROR_TOKEN,
    DOT_TOKEN,
    TRIPLE_DOT_TOKEN,
    PIPE_TOKEN,
    COLON_TOKEN,
    EOF_TOKEN,
};

class Token
{
  private:
    std::string value;
    Kind kind;
    int lineNum;
    int colNum;

  public:
    // constructor
    Token(Kind, std::string, int, int);
    // destructor
    ~Token(){};
    // debugging
    void display();
    // useful for looping through
    bool isEOF();

    // basic get functions
    Kind getKind();
    std::string getValue();
    int getLineNum();
    int getColNum();
};