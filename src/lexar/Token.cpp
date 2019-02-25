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
    // handle keywords and start tokens
    if (k == IDENTIFIER_TOKEN)
    {
        if (v == "fun" || v == "Cat" || v == "Cfn" || v == "dec" ||
            v == "exp" || v == "if" || v == "else" || v == "return" ||
            v == "include" || v == "import" || v == "over" || v == "new")
            kind = KEYWORD_TOKEN;
        else if (v == "start")
            kind = START_TOKEN;
        else if (v == "and" || v == "or")
            kind = OPERATION_TOKEN;
        else if (v == "true" || v == "false")
            kind = BOOL_TOKEN;
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
    case BYTE_TOKEN:
        return "Byte";
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

Kind
Token::getKind()
{
    return kind;
}

std::string
Token::getValue()
{
    return value;
}