/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lexar.h"
#include "Token.h"
#include <string>

/*
 * There are two types of token creations, one where the lexar stops at
 * the completion of a token and another where the lexar stops one past,
 * having found the end of the token. In the second case, I need the buffer to
 * pop the last char and to return this char to the scanner. Thus, I have
 * created a function for this second case.
 */
Token*
createTokenPutback(Kind k, char c, std::string buffer, Scanner* scanner)
{
    if (!isspace(c))
    {
        buffer.pop_back();
    }
    scanner->putBackChar(c);
    return new Token(k, buffer);
}

Lexar::Lexar(std::string filename)
{
    scanner = new Scanner(filename);
}

Token*
Lexar::getNextToken()
{
    char c;
    int state = State::START;
    std::string buffer = "";

    while ((c = scanner->getNextChar()))
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
                    state = State::IDENTIFIER;
                }
                else if (isdigit(c))
                {
                    state = State::INTEGER;
                }
                else
                {
                    switch (c)
                    {
                    // these are all completed on the first char
                    case '+':
                        return new Token(Kind::OPERATION_TOKEN, buffer);
                    case '_':
                        return new Token(Kind::UNDERSCORE_TOKEN, buffer);
                    case ',':
                        return new Token(Kind::COMMA_TOKEN, buffer);
                    case '.':
                        return new Token(Kind::DOT_TOKEN, buffer);
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
                    case ':':
                        return new Token(Kind::COLON_TOKEN, buffer);
                    case ';':
                        return new Token(Kind::ERROR_TOKEN,
                                         "No semi-colons in sailfish");

                    // these require multiple other states
                    case '/':
                        state = State::DIVISION_OR_COMMENT;
                        break;
                    case '\'':
                        state = State::BYTE;
                        break;
                    case '\"':
                        state = State::STRING;
                        break;
                    case '-':
                        state = State::SUBTRACTION;
                        break;
                    case '=':
                        state = State::ASSIGNMENT;
                        break;
                    case '!':
                        state = State::NEGATION;
                        break;
                    case '*':
                        state = State::MULTIPLICATION;
                        break;
                    case '&':
                        state = State::AND_PRESTATE;
                        break;
                    case '|':
                        state = State::OR_PRESTATE;
                        break;
                    case '<':
                        state = State::LESS_THAN;
                        break;
                    case '>':
                        state = State::GREATER_THAN;
                        break;
                    }
                }
                break;

            case State::IDENTIFIER:
                if (!isalnum(c) && c != '_')
                    return createTokenPutback(Kind::IDENTIFIER_TOKEN, c, buffer,
                                              scanner);
                break;

            case State::INTEGER:
                if (c == '.')
                    state = State::FLOAT;
                else if (!isdigit(c))
                    return createTokenPutback(Kind::INTEGER_TOKEN, c, buffer,
                                              scanner);
                break;

            case State::FLOAT:
                if (!isdigit(c))
                    return createTokenPutback(Kind::FLOAT_TOKEN, c, buffer,
                                              scanner);
                break;

            case State::DIVISION_OR_COMMENT:
                if (c == '/')
                    state = State::SINGLE_LINE_COMMENT;
                else if (c == '*')
                    state = State::MULTIPLE_LINE_COMMENT_PRESTATE;
                else
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);
                break;

            case State::SINGLE_LINE_COMMENT:
                if (c == '\n')
                    return new Token(Kind::COMMENT_TOKEN, buffer);
                break;

            case State::MULTIPLE_LINE_COMMENT_PRESTATE:
                if (c == '*')
                    state = State::MULTIPLE_LINE_COMMENT;
                break;

            case State::MULTIPLE_LINE_COMMENT:
                if (c == '/')
                    return new Token(Kind::COMMENT_TOKEN, buffer);
                else if (c != '*')
                    state = State::MULTIPLE_LINE_COMMENT_PRESTATE;
                break;

            case State::BYTE:
                if (c == '\'')
                    return new Token(Kind::BYTE_TOKEN, buffer);

                break;
            case State::STRING:
                if (c == '\\')
                    state = State::STRING_ESCAPE;
                else if (c == '"')
                    return new Token(Kind::STRING_TOKEN, buffer);

                break;
            case State::STRING_ESCAPE:
                state = State::STRING;
                break;

            case State::SUBTRACTION:
                if (c == '>')
                    return new Token(Kind::ARROW_TOKEN, buffer);
                else
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);
            case State::ASSIGNMENT:
                if (c == '=')
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                else
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);

            case State::NEGATION:
                if (c == '=')
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                else
                    return createTokenPutback(Kind::LOGIC_TOKEN, c, buffer,
                                              scanner);

            case State::MULTIPLICATION:
                if (c == '*')
                    return new Token(Kind::OPERATION_TOKEN, buffer);
                else
                    return createTokenPutback(Kind::OPERATION_TOKEN, c, buffer,
                                              scanner);

            case State::AND_PRESTATE:
                if (c != '&')
                    return createTokenPutback(Kind::ERROR_TOKEN, c,
                                              "& is not a valid token",
                                              scanner);
                else
                    return new Token(Kind::LOGIC_TOKEN, buffer);

            case State::OR_PRESTATE:
                if (c != '|')
                    return createTokenPutback(Kind::ERROR_TOKEN, c,
                                              "| is not a valid token",
                                              scanner);
                else
                    return new Token(Kind::LOGIC_TOKEN, buffer);

            case State::LESS_THAN:
                if (c == '=')
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                else if (c == '-')
                    return new Token(Kind::ARROW_TOKEN, buffer);
                else
                    return createTokenPutback(Kind::LOGIC_TOKEN, c, buffer,
                                              scanner);

            case State::GREATER_THAN:
                if (c == '=')
                    return new Token(Kind::LOGIC_TOKEN, buffer);
                else
                    return createTokenPutback(Kind::LOGIC_TOKEN, c, buffer,
                                              scanner);
            }
        }
    }
}
