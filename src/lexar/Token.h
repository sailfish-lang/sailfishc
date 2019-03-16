/*
 * Robert Durst 2019
 * Marmot Programming Language
 */
#pragma once
#include "../common/ReservedWords.h"
#include <iostream>
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
    LCURLEY_TOKEN,
    RCURLEY_TOKEN,
    LPAREN_TOKEN,
    RPAREN_TOKEN,
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

    std::string
    kindToString(Kind k)
    {
        switch (k)
        {
        case START_TOKEN:
            return "Start";
        case OWN_ACCESSOR_TOKEN:
            return "own access";
        case IDENTIFIER_TOKEN:
            return "Identifier";
        case INTEGER_TOKEN:
            return "Integer";
        case FLOAT_TOKEN:
            return "Float";
        case OPERATION_TOKEN:
            return "Operation";
        case STRING_TOKEN:
            return "String";
        case LCURLEY_TOKEN:
            return "Left Curly";
        case RCURLEY_TOKEN:
            return "Right Curly";
        case LPAREN_TOKEN:
            return "Left Parenthesis";
        case RPAREN_TOKEN:
            return "Right Parenthesis";
        case KEYWORD_TOKEN:
            return "Keyword";
        case ARROW_TOKEN:
            return "Arrow";
        case UNDERSCORE_TOKEN:
            return "Underscore";
        case COMMA_TOKEN:
            return "Comma";
        case COMMENT_TOKEN:
            return "Comment";
        case LOGIC_TOKEN:
            return "Logic";
        case DOT_TOKEN:
            return "Dot";
        case TRIPLE_DOT_TOKEN:
            return "Triple Dot";
        case COLON_TOKEN:
            return "Colon";
        case PIPE_TOKEN:
            return "Pipe";
        case BOOL_TOKEN:
            return "Bool";
        case ERROR_TOKEN:
            return "Error";
        case EOF_TOKEN:
            return "EOF";
        default:
            return "Unknown";
        }
    }

  public:
    // constructor
    Token(Kind k, std::string v, int i, int c)
    {
        kind = k;
        // handle keywords and start tokens
        if (k == IDENTIFIER_TOKEN)
        {
            if (isKeyword(v))
                kind = KEYWORD_TOKEN;
            else if (v == "start")
                kind = START_TOKEN;
            else if (v == "own")
                kind = OWN_ACCESSOR_TOKEN;
            else if (v == "and" || v == "or")
                kind = OPERATION_TOKEN;
            else if (v == "true" || v == "false")
                kind = BOOL_TOKEN;
        }
        value = v;
        lineNum = i;
        colNum = c;
    }
    // destructor
    ~Token(){};
    // debugging
    void
    display()
    {
        std::cout << kindToString(kind) << ": " << value << '\n';
    }
    // useful for looping through
    bool
    isEOF()
    {
        return kind == EOF_TOKEN;
    }

    // basic get functions
    Kind
    getKind()
    {
        return kind;
    }
    std::string
    getValue()
    {
        return value;
    }
    int
    getLineNum()
    {
        return lineNum;
    }
    int
    getColNum()
    {
        return colNum;
    }
};