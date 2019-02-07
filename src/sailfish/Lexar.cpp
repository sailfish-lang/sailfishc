/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lexar.h"
#include "DFA.h"
#include "Token.h"
#include <iostream>
#include <string>

Lexar::Lexar(std::string filename)
{
    scanner = new Scanner(filename);
    dfa = new DFA();
    currentState = State::START;
}

Token*
Lexar::getToken()
{
    char c;
    while ((c = scanner->getChar()))
    {
        if (c == EOF)
        {
            return new Token(Kind::EOF_TOKEN, "EOF");
        }
        else
        {
            int nextState = dfa->getNextState(currentState, c);

            if (currentState == State::START)
            {
                if (!isspace(c))
                {
                    buffer += c;
                }
            }
            else if (nextState == State::DONE)
            {
                auto tempBuffer = buffer;
                buffer = "";
                auto tempState = currentState;
                if (currentState != State::STRING &&
                    currentState != State::MULTIPLE_LINE_COMMENT)
                {
                    scanner->putBackChar(c);
                }
                currentState = State::START;

                switch (tempState)
                {
                case State::IDENTIFIER:
                    return new Token(Kind::IDENTIFIER_TOKEN, tempBuffer);
                case State::INTEGER:
                    return new Token(Kind::INTEGER_TOKEN, tempBuffer);
                case State::FLOAT:
                    return new Token(Kind::FLOAT_TOKEN, tempBuffer);
                case State::DIVISION_OR_COMMENT:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::SINGLE_LINE_COMMENT:
                    return new Token(Kind::COMMENT_TOKEN, tempBuffer);
                case State::MULTIPLE_LINE_COMMENT:
                    return new Token(Kind::COMMENT_TOKEN, tempBuffer);
                case State::BYTE:
                    return new Token(Kind::BYTE_TOKEN, tempBuffer);
                case State::STRING:
                    return new Token(Kind::STRING_TOKEN, tempBuffer);
                case State::ADDITION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::UNARY_ADDITION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::SUBTRACTION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::UNARY_SUBTRACTION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::ASSIGNMENT:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::EQUIVALENCE:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::CHAR:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::NEGATION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::NON_EQUIVALENCE:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::MULTIPLICATION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::EXPONENTIATION:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::AND:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::OR:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::LESS_THAN:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::LESS_THAN_EQUAL:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::LARROW:
                    return new Token(Kind::ARROW_TOKEN, tempBuffer);
                case State::GREATER_THAN:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::GREATER_THAN_EQUAL:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                case State::RARROW:
                    return new Token(Kind::ARROW_TOKEN, tempBuffer);
                case State::UNDERSCORE:
                    return new Token(Kind::UNDERSCORE_TOKEN, tempBuffer);
                case State::COMMA:
                    return new Token(Kind::COMMA_TOKEN, tempBuffer);
                case State::LBRACKET:
                    return new Token(Kind::LBRACKET_TOKEN, tempBuffer);
                case State::RBRACKET:
                    return new Token(Kind::RBRACKET_TOKEN, tempBuffer);
                case State::LCURLEY:
                    return new Token(Kind::LCURLEY_TOKEN, tempBuffer);
                case State::LPAREN:
                    return new Token(Kind::LPAREN_TOKEN, tempBuffer);
                case State::RPAREN:
                    return new Token(Kind::RPAREN_TOKEN, tempBuffer);
                case State::MODULO:
                    return new Token(Kind::OPERATION_TOKEN, tempBuffer);
                }
            }
            else if (nextState == State::ERROR)
            {
                auto tempBuffer = buffer;
                return new Token(Kind::ERROR_TOKEN, tempBuffer);
            }
            else
            {
                if (!isspace(c))
                {
                    buffer += c;
                }
            }

            currentState = nextState;
        }
    }
}