/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lexar2.h"
#include <iostream>

/*
 * There are two types of token creations, one where the lexar stops at
 * the completion of a token and another where the lexar stops one past,
 * having found the end of the token. In the second case, I need the buffer to
 * pop the last char and to return this char to the scanner.
 */
Token2
Lexar2::makeToken(const Tokenn::Kind& k, const std::string& v)
{
    auto kd = k; // since kd is constant we copy here
    if (kd == Tokenn::IDENTIFIER)
    {
        if (v == "start")
            kd = Tokenn::Kind::START;
        else if (v == "own")
            kd = Tokenn::Kind::OWN_ACCESSOR;
        else if (v == "Tree")
            kd = Tokenn::Kind::TREE;
        else if (v == "Uat")
            kd = Tokenn::Kind::UAT;
        else if (v == "Ufn")
            kd = Tokenn::Kind::UFN;
        else if (v == "fun")
            kd = Tokenn::Kind::FUN;
        else if (v == "dec")
            kd = Tokenn::Kind::DEC;
        else if (v == "import")
            kd = Tokenn::Kind::IMPORT;
        else if (v == "return")
            kd = Tokenn::Kind::RETURN;
        else if (v == "new")
            kd = Tokenn::Kind::NEW;
        else if (v == "and")
            kd = Tokenn::Kind::AND;
        else if (v == "or")
            kd = Tokenn::Kind::OR;
        else if (v == "true" || v == "false")
            kd = Tokenn::Kind::BOOL;
    }
    return {.kind = kd, .value = v, .col = int(col - v.size()), .line = line};
}

Token2
Lexar2::makeTokenPutback(const Tokenn::Kind& k, std::string& v, char& c)
{
    // readjust the col and line counters
    col = prevCol;
    --prevCol;
    if (c == '\n')
        --line;

    // put the char back in the filebuffer
    file.putback(c);

    if (!isspace(c))
        v.pop_back();

    return makeToken(k, v);
}

char
Lexar2::getNextChar()
{
    if (file.peek() == EOF)
    {
        return -1;
    }

    char c;
    file.get(c);
    return c;
}

Lexar2::Lexar2(const std::string& filename)
{
    file.open(filename, std::fstream::in);
    line = 1;
    col = 1;
    prevCol = 0;
}

Token2
Lexar2::getNextToken()
{
    char c;
    int state = State::START;
    std::string buffer = "";

    while ((c = getNextChar()))
    {
        // update col and line for error handling
        if (c == '\n')
        {
            ++line;
            prevCol = col;
            col = 1;
        }
        else
        {
            prevCol = col;
            ++col;
        }

        // ignore whitespaces if not in a string, adding current char to the
        // buffer
        if (state == State::STRING || (state == State::COMMENT && c != '\n') ||
            !isspace(c))
            buffer += c;

        // catch EOF
        if (c == EOF)
            return makeToken(Tokenn::Kind::EOF_, buffer);

        switch (state)
        {
        case State::START:
            if (isalpha(c))
                state = State::IDENTIFIER;

            else if (isdigit(c))
                state = State::INTEGER;

            else if (isspace(c))
                state = State::START;

            else
            {
                switch (c)
                {
                // completed on the first char
                case '_':
                    return makeToken(Tokenn::Kind::UNDERSCORE, buffer);
                case ',':
                    return makeToken(Tokenn::Kind::COMMA, buffer);
                case '{':
                    return makeToken(Tokenn::Kind::LCURLEY, buffer);
                case '}':
                    return makeToken(Tokenn::Kind::RCURLEY, buffer);
                case '(':
                    return makeToken(Tokenn::Kind::LPAREN, buffer);
                case ')':
                    return makeToken(Tokenn::Kind::RPAREN, buffer);
                case '%':
                    return makeToken(Tokenn::Kind::MODULO, buffer);
                case ':':
                    return makeToken(Tokenn::Kind::COLON, buffer);
                case '|':
                    return makeToken(Tokenn::Kind::PIPE, buffer);
                case ';':
                case '[':
                case ']':
                    return makeToken(Tokenn::Kind::ERROR,
                                     "No semi-colons or brackets in Sailfish.");

                // multiple states for completion
                case '+':
                    state = State::ADDITION;
                    break;
                case '-':
                    state = State::SUBTRACTION;
                    break;
                case '*':
                    state = State::MULTIPLICATION;
                    break;
                case '/':
                    state = State::DIVISION;
                    break;
                case '\"':
                    state = State::STRING;
                    break;
                case '=':
                    state = State::ASSIGNMENT;
                    break;
                case '!':
                    state = State::NEGATION;
                    break;
                case '<':
                    state = State::LESS_THAN;
                    break;
                case '>':
                    state = State::GREATER_THAN;
                    break;
                case '#':
                    state = State::COMMENT;
                    break;
                case '.':
                    state = State::DOUBLE_DOT;
                    break;
                default:
                    return makeToken(Tokenn::Kind::ERROR,
                                     "Unrecognized character.");
                }
            }
            break;

        case State::IDENTIFIER:
            if (!isalnum(c) && c != '_')
                return makeTokenPutback(Tokenn::Kind::IDENTIFIER, buffer, c);
            break;

        case State::INTEGER:
            if (c == '.')
                state = State::FLOAT;
            else if (!isdigit(c))
                return makeTokenPutback(Tokenn::Kind::INTEGER, buffer, c);
            break;

        case State::FLOAT:
            if (!isdigit(c))
                return makeTokenPutback(Tokenn::Kind::FLOAT, buffer, c);
            break;

        case State::COMMENT:
            if (c == '\n')
                return makeTokenPutback(Tokenn::Kind::COMMENT, buffer, c);
            break;

        case State::DIVISION:
            return c == '='
                       ? makeToken(Tokenn::Kind::DIVFROM, buffer)
                       : makeTokenPutback(Tokenn::Kind::DIVISION, buffer, c);

        case State::STRING:
            if (c == '\\')
                state = State::STRING_ESCAPE;
            else if (c == '"')
                return makeToken(Tokenn::Kind::STRING, buffer);
            break;

        case State::STRING_ESCAPE:
            state = State::STRING;
            break;

        case State::SUBTRACTION:
            return c == '='
                       ? makeToken(Tokenn::Kind::SUBFROM, buffer)
                       : makeTokenPutback(Tokenn::Kind::SUBTRACTION, buffer, c);

        case State::ADDITION:
            return c == '='
                       ? makeToken(Tokenn::Kind::ADDTO, buffer)
                       : makeTokenPutback(Tokenn::Kind::ADDITION, buffer, c);

        case State::ASSIGNMENT:
            return c == '='
                       ? makeToken(Tokenn::Kind::EQUIVALENCE, buffer)
                       : makeTokenPutback(Tokenn::Kind::ASSIGNMENT, buffer, c);

        case State::NEGATION:
            return c == '='
                       ? makeToken(Tokenn::Kind::NONEQUIVALENCE, buffer)
                       : makeTokenPutback(Tokenn::Kind::NEGATION, buffer, c);

        case State::MULTIPLICATION:
            if (c == '=')
                return makeToken(Tokenn::Kind::MULTTO, buffer);
            if (c == '*')
                return makeToken(Tokenn::Kind::EXPONENTIATION, buffer);
            return makeTokenPutback(Tokenn::Kind::MULTIPLICATION, buffer, c);

        case State::LESS_THAN:
            return c == '='
                       ? makeToken(Tokenn::Kind::LESS_THAN_OR_EQUALS, buffer)
                       : makeTokenPutback(Tokenn::Kind::LESS_THAN, buffer, c);

        case State::GREATER_THAN:
            return c == '='
                       ? makeToken(Tokenn::Kind::GREATER_THAN_OR_EQUALS, buffer)
                       : makeTokenPutback(Tokenn::Kind::GREATER_THAN, buffer,
                                          c);

        case State::DOUBLE_DOT:
            if (c != '.')
                return makeTokenPutback(Tokenn::Kind::DOT, buffer, c);
            state = State::TRIPLE_DOT;
            break;

        case State::TRIPLE_DOT:
            return c == '.' ? makeToken(Tokenn::Kind::TRIPLE_DOT, buffer)
                            : makeToken(Tokenn::Kind::ERROR, "Expected a dot.");

        default:
            return makeToken(Tokenn::Kind::ERROR, "Unexpected state.");
        }
    }
}