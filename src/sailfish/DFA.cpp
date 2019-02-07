/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "DFA.h"
#include <iostream>
#include <map>
#include <tuple>

DFA::DFA()
{
    for (int state = State::START; state < State::ERROR; state++)
    {
        switch (state)
        {
        case State::START:
            for (char c = 0; c < 126; c++)
            {
                if (isalpha(c))
                {
                    dfa.insert({{state, c}, State::IDENTIFIER});
                }
                else if (isdigit(c))
                {
                    dfa.insert({{state, c}, State::INTEGER});
                }
                else
                {
                    switch (c)
                    {
                    case '/':
                        dfa.insert({{state, c}, State::DIVISION_OR_COMMENT});
                        break;
                    case '\'':
                        dfa.insert({{state, c}, State::BYTE});
                        break;
                    case '\"':
                        dfa.insert({{state, c}, State::STRING});
                        break;
                    case '+':
                        dfa.insert({{state, c}, State::ADDITION});
                        break;
                    case '-':
                        dfa.insert({{state, c}, State::SUBTRACTION});
                        break;
                    case '=':
                        dfa.insert({{state, c}, State::ASSIGNMENT});
                        break;
                    case '\\':
                        dfa.insert({{state, c}, State::CHAR});
                        break;
                    case '!':
                        dfa.insert({{state, c}, State::NEGATION});
                        break;
                    case '*':
                        dfa.insert({{state, c}, State::MULTIPLICATION});
                        break;
                    case '&':
                        dfa.insert({{state, c}, State::AND_PRESTATE});
                        break;
                    case '|':
                        dfa.insert({{state, c}, State::OR_PRESTATE});
                        break;
                    case '<':
                        dfa.insert({{state, c}, State::LESS_THAN});
                        break;
                    case '>':
                        dfa.insert({{state, c}, State::GREATER_THAN});
                        break;
                    case '_':
                        dfa.insert({{state, c}, State::UNDERSCORE});
                        break;
                    case ',':
                        dfa.insert({{state, c}, State::COMMA});
                        break;
                    case '[':
                        dfa.insert({{state, c}, State::LBRACKET});
                        break;
                    case ']':
                        dfa.insert({{state, c}, State::RBRACKET});
                        break;
                    case '{':
                        dfa.insert({{state, c}, State::LCURLEY});
                        break;
                    case '}':
                        dfa.insert({{state, c}, State::RCURLEY});
                        break;
                    case '(':
                        dfa.insert({{state, c}, State::LPAREN});
                        break;
                    case ')':
                        dfa.insert({{state, c}, State::RPAREN});
                        break;
                    case '%':
                        dfa.insert({{state, c}, State::MODULO});
                        break;
                    default:
                        dfa.insert({{state, c}, state});
                    }
                }
            }
            break;
        case State::IDENTIFIER:
            for (char c = 0; c < 126; c++)
            {
                if (isalnum(c) || c == '_')
                {
                    dfa.insert({{state, c}, state});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::INTEGER:
            for (char c = 0; c < 126; c++)
            {
                if (isdigit(c))
                {
                    dfa.insert({{state, c}, state});
                }
                else if (c == '.')
                {
                    dfa.insert({{state, c}, State::FLOAT});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::FLOAT:
            for (char c = 0; c < 126; c++)
            {
                if (isdigit(c))
                {
                    dfa.insert({{state, c}, state});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::DIVISION_OR_COMMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '/')
                {
                    dfa.insert({{state, c}, State::SINGLE_LINE_COMMENT});
                }
                else if (c == '*')
                {
                    dfa.insert(
                        {{state, c}, State::MULTIPLE_LINE_COMMENT_PRESTATE});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::SINGLE_LINE_COMMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '\n')
                {
                    dfa.insert({{state, c}, State::DONE});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case State::MULTIPLE_LINE_COMMENT_PRESTATE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '*')
                {
                    dfa.insert({{state, c}, State::MULTIPLE_LINE_COMMENT});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case State::MULTIPLE_LINE_COMMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '/')
                {
                    dfa.insert({{state, c}, State::DONE});
                }
                else
                {
                    dfa.insert(
                        {{state, c}, State::MULTIPLE_LINE_COMMENT_PRESTATE});
                }
            }
            break;
        case State::BYTE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '\'')
                {
                    dfa.insert({{state, c}, State::DONE});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case State::STRING:
            for (char c = 0; c < 126; c++)
            {
                if (c == '\\')
                {
                    dfa.insert({{state, c}, State::STRING_ESCAPE});
                }
                else if (c == '"')
                {
                    dfa.insert({{state, c}, State::DONE});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case State::STRING_ESCAPE:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, State::STRING});
            }
            break;
        case State::ADDITION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '+')
                {
                    dfa.insert({{state, c}, State::UNARY_ADDITION});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::SUBTRACTION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '-')
                {
                    dfa.insert({{state, c}, State::UNARY_SUBTRACTION});
                }
                else if (c == '>')
                {
                    dfa.insert({{state, c}, State::RARROW});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::ASSIGNMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, State::EQUIVALENCE});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::NEGATION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, State::EQUIVALENCE});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::MULTIPLICATION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '*')
                {
                    dfa.insert({{state, c}, State::EXPONENTIATION});
                }
                else
                {
                    dfa.insert({{state, c}, State::DONE});
                }
            }
            break;
        case State::AND_PRESTATE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '&')
                {
                    dfa.insert({{state, c}, State::AND});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, State::ERROR});
                }
            }
            break;
        case State::OR_PRESTATE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '|')
                {
                    dfa.insert({{state, c}, State::OR});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, State::ERROR});
                }
            }
            break;
        case State::LESS_THAN:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, State::LESS_THAN_EQUAL});
                }
                else if (c == '-')
                {
                    dfa.insert({{state, c}, State::LARROW});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, State::ERROR});
                }
            }
            break;
        case State::GREATER_THAN:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, State::GREATER_THAN_EQUAL});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, State::ERROR});
                }
            }
            break;
        case State::UNARY_ADDITION:
        case State::UNARY_SUBTRACTION:
        case State::CHAR:
        case State::NON_EQUIVALENCE:
        case State::EQUIVALENCE:
        case State::EXPONENTIATION:
        case State::OR:
        case State::LESS_THAN_EQUAL:
        case State::LARROW:
        case State::GREATER_THAN_EQUAL:
        case State::RARROW:
        case State::UNDERSCORE:
        case State::COMMA:
        case State::LBRACKET:
        case State::RBRACKET:
        case State::LCURLEY:
        case State::RCURLEY:
        case State::LPAREN:
        case State::RPAREN:
        case State::MODULO:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, State::DONE});
            }
            break;
        case State::ERROR:
            // not quite sure what to do here yet as the state reset may
            // happen in the lexar.
            // TODO: figure this out
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, state});
            }
            break;
        }
    }
}

int
DFA::getNextState(int state, char c)
{
    std::map<std::tuple<int, char>, int>::iterator it;

    it = dfa.find({state, c});
    if (it != dfa.end())
    {
        return dfa.find({state, c})->second;
    }
    else
    {
        return State::START;
    }
}