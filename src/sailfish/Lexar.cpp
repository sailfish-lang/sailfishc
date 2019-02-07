/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lexar.h"
#include "DFA.h"
#include <iostream>
#include <string>

Lexar::Lexar(std::string filename)
{
    scanner = new Scanner(filename);
    dfa = new DFA();
    currentState = dfa->START;
}

void
Lexar::getToken()
{
    char c;
    while ((c = scanner->getChar()))
    {
        if (c == EOF)
        {
            std::cout << "EOF" << '\n';
            break;
        }
        else
        {
            int nextState = dfa->getNextState(currentState, c);

            if (currentState == dfa->START)
            {
                if (!isspace(c))
                {
                    buffer += c;
                }
            }
            else if (nextState == dfa->DONE)
            {
                if (currentState == dfa->IDENTIFIER)
                {
                    std::cout << "IDENTIFIER" << buffer << '\n';
                }
                else if (currentState == dfa->INTEGER)
                {
                    std::cout << "INTEGER" << buffer << '\n';
                }
                else if (currentState == dfa->FLOAT)
                {
                    std::cout << "FLOAT" << buffer << '\n';
                }
                else if (currentState == dfa->DIVISION_OR_COMMENT)
                {
                    std::cout << "DIVISION" << buffer << '\n';
                }
                else if (currentState == dfa->SINGLE_LINE_COMMENT)
                {
                    std::cout << "SINGLE LINE COMMENT" << buffer << '\n';
                }
                else if (currentState == dfa->MULTIPLE_LINE_COMMENT)
                {
                    std::cout << "MULTIPLE LINE COMMENT" << buffer << '\n';
                }
                else if (currentState == dfa->BYTE)
                {
                    std::cout << "BYTE" << buffer << '\n';
                }
                else if (currentState == dfa->STRING)
                {
                    std::cout << "STRING" << buffer << '\n';
                }
                else if (currentState == dfa->ADDITION)
                {
                    std::cout << "ADDITION" << buffer << '\n';
                }
                else if (currentState == dfa->UNARY_ADDITION)
                {
                    std::cout << "UNARY ADDITION" << buffer << '\n';
                }
                else if (currentState == dfa->SUBTRACTION)
                {
                    std::cout << "SUBTRACTION" << buffer << '\n';
                }
                else if (currentState == dfa->UNARY_SUBTRACTION)
                {
                    std::cout << "UNARY SUBTRACTION" << buffer << '\n';
                }
                else if (currentState == dfa->ASSIGNMENT)
                {
                    std::cout << "ASSIGNMENT" << buffer << '\n';
                }
                else if (currentState == dfa->EQUIVALENCE)
                {
                    std::cout << "EQUIVALENCE" << buffer << '\n';
                }
                else if (currentState == dfa->CHAR)
                {
                    std::cout << "CHAR" << buffer << '\n';
                }
                else if (currentState == dfa->NEGATION)
                {
                    std::cout << "NEGATION" << buffer << '\n';
                }
                else if (currentState == dfa->NON_EQUIVALENCE)
                {
                    std::cout << "NON EQUIVALENCE" << buffer << '\n';
                }
                else if (currentState == dfa->MULTIPLICATION)
                {
                    std::cout << "MULTIPLICATION" << buffer << '\n';
                }
                else if (currentState == dfa->EXPONENTIATION)
                {
                    std::cout << "EXPONENTIATION" << buffer << '\n';
                }
                else if (currentState == dfa->AND)
                {
                    std::cout << "AND" << buffer << '\n';
                }
                else if (currentState == dfa->OR)
                {
                    std::cout << "OR" << buffer << '\n';
                }
                else if (currentState == dfa->LESS_THAN)
                {
                    std::cout << "LESS THAN" << buffer << '\n';
                }

                else if (currentState == dfa->LESS_THAN_EQUAL)
                {
                    std::cout << "LESS THAN EQUAL" << buffer << '\n';
                }

                else if (currentState == dfa->LARROW)
                {
                    std::cout << "LEFT ARROW" << buffer << '\n';
                }

                else if (currentState == dfa->GREATER_THAN)
                {
                    std::cout << "GREATER THAN" << buffer << '\n';
                }

                else if (currentState == dfa->GREATER_THAN_EQUAL)
                {
                    std::cout << "GREATER THAN EQUAL" << buffer << '\n';
                }

                else if (currentState == dfa->RARROW)
                {
                    std::cout << "RARROW" << buffer << '\n';
                }

                else if (currentState == dfa->UNDERSCORE)
                {
                    std::cout << "UNDERSCORE" << buffer << '\n';
                }

                else if (currentState == dfa->COMMA)
                {
                    std::cout << "COMMA" << buffer << '\n';
                }

                else if (currentState == dfa->LBRACKET)
                {
                    std::cout << "LBRACKET" << buffer << '\n';
                }

                else if (currentState == dfa->RBRACKET)
                {
                    std::cout << "RBRACKET" << buffer << '\n';
                }

                else if (currentState == dfa->LCURLEY)
                {
                    std::cout << "LCURLEY" << buffer << '\n';
                }

                else if (currentState == dfa->LPAREN)
                {
                    std::cout << "LPAREN" << buffer << '\n';
                }

                else if (currentState == dfa->RPAREN)
                {
                    std::cout << "RPAREN" << buffer << '\n';
                }

                else if (currentState == dfa->MODULO)
                {
                    std::cout << "MODULO" << buffer << '\n';
                }

                buffer = "";
                nextState = dfa->START;

                if (currentState != dfa->STRING &&
                    currentState != dfa->MULTIPLE_LINE_COMMENT)
                {
                    scanner->putBackChar(c);
                }
            }
            else if (nextState == dfa->ERROR)
            {
                std::cout << "ERROR" << '\n';
                currentState = dfa->START;
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