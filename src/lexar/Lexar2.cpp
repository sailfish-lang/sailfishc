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
std::unique_ptr<Token2>
Lexar2::makeToken(const TokenKind& k, const std::string& v)
{
    auto kd = k; // since kd is constant we copy here

    // instead of a symbol table or a keyword table/data structure, we have this
    // nice if-else tree
    if (kd == TokenKind::IDENTIFIER)
    {
        if (v == "start")
            kd = TokenKind::START;
        else if (v == "own")
            kd = TokenKind::OWN_ACCESSOR;
        else if (v == "Tree")
            kd = TokenKind::TREE;
        else if (v == "Uat")
            kd = TokenKind::UAT;
        else if (v == "Ufn")
            kd = TokenKind::UFN;
        else if (v == "Ife")
            kd = TokenKind::IFE;
        else if (v == "fun")
            kd = TokenKind::FUN;
        else if (v == "empty")
            kd = TokenKind::EMPTY;
        else if (v == "dec")
            kd = TokenKind::DEC;
        else if (v == "import")
            kd = TokenKind::IMPORT;
        else if (v == "return")
            kd = TokenKind::RETURN;
        else if (v == "new")
            kd = TokenKind::NEW;
        else if (v == "and")
            kd = TokenKind::AND;
        else if (v == "or")
            kd = TokenKind::OR;
        else if (v == "true" || v == "false")
            kd = TokenKind::BOOL;
    }
    return std::make_unique<Token2>(kd, v, col, line);
}

std::unique_ptr<Token2>
Lexar2::makeTokenPutback(const TokenKind& k, std::string& v, char& c)
{
    // readjust the col and line counters
    col = prevCol;
    --prevCol;
    if (c == '\n')
        --line;

    // put the char back in the filebuffer
    file.putback(c);

    // edge case where if the last char was a space, even though we want to
    // putback, the buffer itself won't be off by one
    if (!isspace(c))
        v.pop_back();

    return makeToken(k, v);
}

char
Lexar2::getNextChar()
{
    if (isFile)
    {
        if (file.peek() == EOF)
            return -1;

        char c;
        file.get(c);
        return c;
    }

    if (rawString.size() == 0)
        return -1;

    char c = rawString.at(0);
    rawString.erase(0, 1);
    return c;
}

Lexar2::Lexar2(const std::string& fileString, bool isAFile)
{
    if (isAFile)
    {
        file.open(fileString, std::fstream::in);
        if (!file.good())
            throw "File: " + fileString + " not found.\n";
    }
    else
    {
        rawString = fileString;
    }

    isFile = isAFile;
    line = 1;
    col = 1;
    prevCol = 0;
}

std::unique_ptr<Token2>
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
        if (state == State::STRING || state == State::LIST ||
            (state == State::COMMENT && c != '\n') || !isspace(c))
            buffer += c;

        // catch EOF
        if (c == EOF)
            return makeToken(TokenKind::EOF_, buffer);

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
                    return makeToken(TokenKind::UNDERSCORE, buffer);
                case ',':
                    return makeToken(TokenKind::COMMA, buffer);
                case '{':
                    return makeToken(TokenKind::LCURLEY, buffer);
                case '}':
                    return makeToken(TokenKind::RCURLEY, buffer);
                case '(':
                    return makeToken(TokenKind::LPAREN, buffer);
                case ')':
                    return makeToken(TokenKind::RPAREN, buffer);
                case '%':
                    return makeToken(TokenKind::MODULO, buffer);
                case ':':
                    return makeToken(TokenKind::COLON, buffer);
                case '|':
                    return makeToken(TokenKind::PIPE, buffer);
                case ';':
                    return makeToken(TokenKind::ERROR,
                                     "No semi-colons in Sailfish.");

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
                case '[':
                    state = State::LIST;
                    break;
                default:
                    return makeToken(TokenKind::ERROR,
                                     "Unrecognized character.");
                }
            }
            break;

        case State::IDENTIFIER:
            if (!isalnum(c) && c != '_')
                return makeTokenPutback(TokenKind::IDENTIFIER, buffer, c);
            break;

        case State::INTEGER:
            if (c == '.')
                state = State::FLOAT;
            else if (!isdigit(c))
                return makeTokenPutback(TokenKind::INTEGER, buffer, c);
            break;

        case State::FLOAT:
            if (!isdigit(c))
                return makeTokenPutback(TokenKind::FLOAT, buffer, c);
            break;

        case State::COMMENT:
            if (c == '\n')
                return makeTokenPutback(TokenKind::COMMENT, buffer, c);
            break;

        case State::DIVISION:
            return c == '=' ? makeToken(TokenKind::DIVFROM, buffer)
                            : makeTokenPutback(TokenKind::DIVISION, buffer, c);

        case State::STRING:
            if (c == '\\')
                state = State::STRING_ESCAPE;
            else if (c == '"')
                return makeToken(TokenKind::STRING, buffer);
            break;

        case State::STRING_ESCAPE:
            state = State::STRING;
            break;

        case State::SUBTRACTION:
            if (c == '=')
                return makeToken(TokenKind::SUBFROM, buffer);
            else if (c == '-')
                return makeToken(TokenKind::UNARYMINUS, buffer);
            else
                return makeTokenPutback(TokenKind::SUBTRACTION, buffer, c);

        case State::ADDITION:
            if (c == '=')
                return makeToken(TokenKind::ADDTO, buffer);
            else if (c == '+')
                return makeToken(TokenKind::UNARYADD, buffer);
            else
                return makeTokenPutback(TokenKind::ADDITION, buffer, c);

        case State::ASSIGNMENT:
            return c == '='
                       ? makeToken(TokenKind::EQUIVALENCE, buffer)
                       : makeTokenPutback(TokenKind::ASSIGNMENT, buffer, c);

        case State::NEGATION:
            return c == '=' ? makeToken(TokenKind::NONEQUIVALENCE, buffer)
                            : makeTokenPutback(TokenKind::NEGATION, buffer, c);

        case State::MULTIPLICATION:
            if (c == '=')
                return makeToken(TokenKind::MULTTO, buffer);
            if (c == '*')
                return makeToken(TokenKind::EXPONENTIATION, buffer);
            return makeTokenPutback(TokenKind::MULTIPLICATION, buffer, c);

        case State::LESS_THAN:
            return c == '=' ? makeToken(TokenKind::LESS_THAN_OR_EQUALS, buffer)
                            : makeTokenPutback(TokenKind::LESS_THAN, buffer, c);

        case State::GREATER_THAN:
            return c == '='
                       ? makeToken(TokenKind::GREATER_THAN_OR_EQUALS, buffer)
                       : makeTokenPutback(TokenKind::GREATER_THAN, buffer, c);

        case State::DOUBLE_DOT:
            if (c != '.')
                return makeTokenPutback(TokenKind::DOT, buffer, c);
            state = State::TRIPLE_DOT;
            break;

        case State::TRIPLE_DOT:
            return c == '.' ? makeToken(TokenKind::TRIPLE_DOT, buffer)
                            : makeToken(TokenKind::ERROR, "Expected a dot.");

        case State::LIST:
            if (c == ']')
            {
                if (buffer == "[int]" || buffer == "[bool]" ||
                    buffer == "[str]" || buffer == "[flt]")
                    return makeToken(TokenKind::LISTTYPE, buffer);

                else
                    return makeToken(TokenKind::LIST, buffer);
            }
            break;

        default:
            return makeToken(TokenKind::ERROR, "Unexpected state.");
        }
    }
}