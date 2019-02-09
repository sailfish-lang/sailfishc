/*
 * Robert Durst 2019
 * Marmot Programming Language
 */
#include "Token.h"
#include <iostream>
#include <string>
Token::Token(Kind k, std::string v)
{
    kind = k;
    // handle keywords
    if (k == IDENTIFIER_TOKEN)
    {
        if (v == "fun" || v == "Cat" || v == "Cfn" || v == "loop" ||
            v == "dec" || v == "exp" || v == "ig" || v == "else" || "return" ||
            "include" || "import" || "break" || "continue")
            kind = KEYWORD_TOKEN;
    }
    value = v;
}

std::string
kindToString(Kind k)
{
    switch (k)
    {
    case START_TOKEN:
        return "Start";
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
    case CHAR_TOKEN:
        return "Char";
    case LBRACKET_TOKEN:
        return "Left Bracket";
    case RBRACKET_TOKEN:
        return "Right Brakcet";
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
    case ERROR_TOKEN:
        return "Error";
    case EOF_TOKEN:
        return "EOF";
    default:
        return "Unknown";
    }
}

void
Token::display()
{
    std::cout << kindToString(kind) << ": " << value << '\n';
}

bool
Token::isEOF()
{
    return kind == EOF_TOKEN;
}