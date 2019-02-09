/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lexar.h"
#include "Token.h"
#include <iostream>
#include <string>

Token*
createTokenPutback(Kind k, char c, std::string buffer, Scanner* scanner)
{
    if (!isspace(c))
    {
        buffer.pop_back();
    }
    scanner->putBackChar(c);
    return new Token(Kind::OPERATION_TOKEN, buffer);
}

Lexar::Lexar(std::string filename)
{
    scanner = new Scanner(filename);
}

Token*
Lexar::getToken()
{
    // something to hold the current token
    char c;
    // something to hold the current state
    int state = State::START;
    // something to hold the current buffer
    std::string buffer = "";

    // loop until no more chars in file to read
    while ((c = scanner->getChar()))
    {
        if (c == EOF)
        {
            return new Token(Kind::EOF_TOKEN, buffer);
        }
        else
        {
            if (!isspace(c))
            {
                buffer += c;
            }
            switch (state)
            {
            case State::START:
                if (isalpha(c))
                {
                    // go to the identifier state
                    state = State::IDENTIFIER;
                }
                else if (isdigit(c))
                {
                    // go to the digit state
                    state = State::INTEGER;
                }
                else
                {
                    switch (c)
                    {
                    // these are all completed on the first char
                    case '_':
                        return new Token(Kind::UNDERSCORE_TOKEN, buffer);
                    case ',':
                        return new Token(Kind::COMMA_TOKEN, buffer);
                    case '[':
                        return new Token(Kind::LBRACKET_TOKEN, buffer);
                    case ']':
                        return new Token(Kind::RBRACKET_TOKEN, buffer);
                    case '{':
                        return new Token(Kind::LCURLEY_TOKEN, buffer);
                    case '}':
                        return new Token(Kind::RCURLEY_TOKEN, buffer);
                    case '(':
                        return new Token(Kind::LPAREN_TOKEN, buffer);
                    case ')':
                        return new Token(Kind::RPAREN_TOKEN, buffer);
                    case '%':
                        return new Token(Kind::OPERATION_TOKEN, buffer);
                    case ';':
                        return new Token(Kind::ERROR_TOKEN,
                                         "No semi-colons in sailfish");

                    // these require multiple other states
                    case '/':
                        // go to comment state
                        state = State::DIVISION_OR_COMMENT;
                        break;
                    case '\'':
                        // go to byte state
                        state = State::BYTE;
                        break;
                    case '\"':
                        // go to string state
                        state = State::STRING;
                        break;
                    case '+':
                        // go to addition state
                        state = State::ADDITION;
                        break;
                    case '-':
                        // go to subtraction state
                        state = State::SUBTRACTION;
                        break;
                    case '=':
                        // go to equals state
                        state = State::ASSIGNMENT;
                        break;
                    case '\\':
                        // go to char state
                        state = State::CHAR;
                        break;
                    case '!':
                        // go to negation state
                        state = State::NEGATION;
                        break;
                    case '*':
                        // go to asteriks state
                        state = State::MULTIPLICATION;
                        break;
                    case '&':
                        // go to and prestate
                        state = State::AND_PRESTATE;
                        break;
                    case '|':
                        // go to or prestate
                        state = State::OR_PRESTATE;
                        break;
                    case '<':
                        // go to less than state
                        state = State::LESS_THAN;
                        break;
                    case '>':
                        // go to greater than state
                        state = State::GREATER_THAN;
                        break;
                    }
                }
                break;
            case State::IDENTIFIER:
                if (!isalnum(c) && c != '_')
                {
                    return createTokenPutback(Kind::IDENTIFIER_TOKEN, c, buffer,
                                              scanner);
                }
                break;
            case State::INTEGER:
                if (c == '.')
                {
                    state = State::FLOAT;
                }
                else if (!isdigit(c))
                {
                    return createTokenPutback(Kind::INTEGER_TOKEN, c, buffer,
                                              scanner);
                }
                break;
            case State::FLOAT:
                if (!isdigit(c))
                {
                    return createTokenPutback(Kind::FLOAT_TOKEN, c, buffer,
                                              scanner);
                }
                break;
            case State::DIVISION_OR_COMMENT:
                if (c == '/')
                {
                    state = State::SINGLE_LINE_COMMENT;
                }
                else if (c == '*')
                {
                    state = State::MULTIPLE_LINE_COMMENT_PRESTATE;
                }
                else
                {
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);
                }
                break;
            case State::SINGLE_LINE_COMMENT:
                if (c == '\n')
                {
                    return new Token(Kind::COMMENT_TOKEN, buffer);
                }
                break;
            case State::MULTIPLE_LINE_COMMENT_PRESTATE:
                if (c == '*')
                {
                    state = State::MULTIPLE_LINE_COMMENT;
                }
                break;
            case State::MULTIPLE_LINE_COMMENT:
                if (c == '/')
                {
                    return new Token(Kind::COMMENT_TOKEN, buffer);
                }

                state = State::MULTIPLE_LINE_COMMENT_PRESTATE;
                break;
            case State::BYTE:
                if (c == '\'')
                {
                    return new Token(Kind::BYTE_TOKEN, buffer);
                }
                break;
            case State::STRING:
                if (c == '\\')
                {
                    state = State::STRING_ESCAPE;
                }
                else if (c == '"')
                {
                    return new Token(Kind::STRING_TOKEN, buffer);
                }
                break;
            case State::STRING_ESCAPE:
                state = State::STRING;
                break;
            case State::ADDITION:
                // this should not push back the char
                if (c == '+')
                {
                    return new Token(Kind::OPERATION_TOKEN, buffer);
                }
                else
                {
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);
                }
            case State::SUBTRACTION:
                // this should not push back the char
                if (c == '-')
                {
                    return new Token(Kind::OPERATION_TOKEN, buffer);
                }
                else if (c == '>')
                {
                    return new Token(Kind::ARROW_TOKEN, buffer);
                }
                else
                {
                    return new Token(Kind::OPERATION_TOKEN, buffer);
                }
                break;
            case State::ASSIGNMENT:
                // this should not push back the char
                if (c == '=')
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
                else
                {
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);
                }
            case State::CHAR:
                return new Token(Kind::CHAR_TOKEN, buffer);
            case State::NEGATION:
                // this should not push back the char
                if (c == '=')
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
                // is ! a logic token or an operation token?
                else
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
            case State::MULTIPLICATION:
                // this should not push back the char
                if (c == '*')
                {
                    return new Token(Kind::OPERATION_TOKEN, buffer);
                }
                else
                {
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);
                }
            case State::AND_PRESTATE:
                if (c != '&')
                {
                    return createTokenPutback(Kind::ERROR_TOKEN, c,
                                              "& is not a valid token",
                                              scanner);
                }
                else
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
            case State::OR_PRESTATE:
                if (c != '|')
                {
                    return createTokenPutback(Kind::INTEGER_TOKEN, c,
                                              "| is not a valid token",
                                              scanner);
                }
                else
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
            case State::LESS_THAN:
                // no char putback here
                if (c == '=')
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
                // no char putback here
                else if (c == '-')
                {
                    return new Token(Kind::ARROW_TOKEN, buffer);
                }
                else
                {
                    return createTokenPutback(Kind::LOGIC_TOKEN, c, buffer,
                                              scanner);
                }
            case State::GREATER_THAN:
                // no char putback here
                if (c == '=')
                {
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                }
                else
                {
                    return createTokenPutback(Kind::LOGIC_TOKEN, c, buffer,
                                              scanner);
                }
            }
        }
    }
}