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
    for (int state = START; state < ERROR; state++)
    {
        switch (state)
        {
        case START:
            for (char c = 0; c < 126; c++)
            {
                if (isalpha(c))
                {
                    dfa.insert({{state, c}, IDENTIFIER});
                }
                else if (isdigit(c))
                {
                    dfa.insert({{state, c}, INTEGER});
                }
                else
                {
                    switch (c)
                    {
                    case '/':
                        dfa.insert({{state, c}, DIVISION_OR_COMMENT});
                        break;
                    case '\'':
                        dfa.insert({{state, c}, BYTE});
                        break;
                    case '\"':
                        dfa.insert({{state, c}, STRING});
                        break;
                    case '+':
                        dfa.insert({{state, c}, ADDITION});
                        break;
                    case '-':
                        dfa.insert({{state, c}, SUBTRACTION});
                        break;
                    case '=':
                        dfa.insert({{state, c}, ASSIGNMENT});
                        break;
                    case '\\':
                        dfa.insert({{state, c}, CHAR});
                        break;
                    case '!':
                        dfa.insert({{state, c}, NEGATION});
                        break;
                    case '*':
                        dfa.insert({{state, c}, MULTIPLICATION});
                        break;
                    case '&':
                        dfa.insert({{state, c}, AND_PRESTATE});
                        break;
                    case '|':
                        dfa.insert({{state, c}, OR_PRESTATE});
                        break;
                    case '<':
                        dfa.insert({{state, c}, LESS_THAN});
                        break;
                    case '>':
                        dfa.insert({{state, c}, GREATER_THAN});
                        break;
                    case '_':
                        dfa.insert({{state, c}, UNDERSCORE});
                        break;
                    case ',':
                        dfa.insert({{state, c}, COMMA});
                        break;
                    case '[':
                        dfa.insert({{state, c}, LBRACKET});
                        break;
                    case ']':
                        dfa.insert({{state, c}, RBRACKET});
                        break;
                    case '{':
                        dfa.insert({{state, c}, LCURLEY});
                        break;
                    case '}':
                        dfa.insert({{state, c}, RCURLEY});
                        break;
                    case '(':
                        dfa.insert({{state, c}, LPAREN});
                        break;
                    case ')':
                        dfa.insert({{state, c}, RPAREN});
                        break;
                    case '%':
                        dfa.insert({{state, c}, MODULO});
                        break;
                    default:
                        dfa.insert({{state, c}, state});
                    }
                }
            }
            break;
        case IDENTIFIER:
            for (char c = 0; c < 126; c++)
            {
                if (isalnum(c) || c == '_')
                {
                    dfa.insert({{state, c}, state});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case INTEGER:
            for (char c = 0; c < 126; c++)
            {
                if (isdigit(c))
                {
                    dfa.insert({{state, c}, state});
                }
                else if (c == '.')
                {
                    dfa.insert({{state, c}, FLOAT});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case FLOAT:
            for (char c = 0; c < 126; c++)
            {
                if (isdigit(c))
                {
                    dfa.insert({{state, c}, state});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case DIVISION_OR_COMMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '/')
                {
                    dfa.insert({{state, c}, SINGLE_LINE_COMMENT});
                }
                else if (c == '*')
                {
                    dfa.insert({{state, c}, MULTIPLE_LINE_COMMENT_PRESTATE});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case SINGLE_LINE_COMMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '\n')
                {
                    dfa.insert({{state, c}, DONE});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case MULTIPLE_LINE_COMMENT_PRESTATE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '*')
                {
                    dfa.insert({{state, c}, MULTIPLE_LINE_COMMENT});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case MULTIPLE_LINE_COMMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '/')
                {
                    dfa.insert({{state, c}, DONE});
                }
                else
                {
                    dfa.insert({{state, c}, MULTIPLE_LINE_COMMENT_PRESTATE});
                }
            }
            break;
        case BYTE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '\'')
                {
                    dfa.insert({{state, c}, DONE});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case STRING:
            for (char c = 0; c < 126; c++)
            {
                if (c == '\\')
                {
                    dfa.insert({{state, c}, STRING_ESCAPE});
                }
                else if (c == '"')
                {
                    dfa.insert({{state, c}, DONE});
                }
                else
                {
                    dfa.insert({{state, c}, state});
                }
            }
            break;
        case STRING_ESCAPE:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, STRING});
            }
            break;
        case ADDITION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '+')
                {
                    dfa.insert({{state, c}, UNARY_ADDITION});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case UNARY_ADDITION:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case SUBTRACTION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '-')
                {
                    dfa.insert({{state, c}, UNARY_SUBTRACTION});
                }
                else if (c == '>')
                {
                    dfa.insert({{state, c}, RARROW});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case UNARY_SUBTRACTION:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case ASSIGNMENT:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, EQUIVALENCE});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case EQUIVALENCE:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case CHAR:
            // TODO: catch errors
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case NEGATION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, EQUIVALENCE});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case NON_EQUIVALENCE:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case MULTIPLICATION:
            for (char c = 0; c < 126; c++)
            {
                if (c == '*')
                {
                    dfa.insert({{state, c}, EXPONENTIATION});
                }
                else
                {
                    dfa.insert({{state, c}, DONE});
                }
            }
            break;
        case EXPONENTIATION:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case AND_PRESTATE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '&')
                {
                    dfa.insert({{state, c}, AND});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, ERROR});
                }
            }
            break;
        case AND:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case OR_PRESTATE:
            for (char c = 0; c < 126; c++)
            {
                if (c == '|')
                {
                    dfa.insert({{state, c}, OR});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, ERROR});
                }
            }
            break;
        case OR:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case LESS_THAN:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, LESS_THAN_EQUAL});
                }
                else if (c == '-')
                {
                    dfa.insert({{state, c}, LARROW});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, ERROR});
                }
            }
            break;
        case GREATER_THAN:
            for (char c = 0; c < 126; c++)
            {
                if (c == '=')
                {
                    dfa.insert({{state, c}, GREATER_THAN_EQUAL});
                }
                else
                {
                    // in real life this is a binary op, but I don't need
                    // binary ops right now
                    dfa.insert({{state, c}, ERROR});
                }
            }
            break;
        case LESS_THAN_EQUAL:
        case LARROW:
        case GREATER_THAN_EQUAL:
        case RARROW:
        case UNDERSCORE:
        case COMMA:
        case LBRACKET:
        case RBRACKET:
        case LCURLEY:
        case RCURLEY:
        case LPAREN:
        case RPAREN:
        case MODULO:
            for (char c = 0; c < 126; c++)
            {
                dfa.insert({{state, c}, DONE});
            }
            break;
        case ERROR:
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
        return START;
    }
}