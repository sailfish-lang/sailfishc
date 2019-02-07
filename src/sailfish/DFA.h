/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <map>
#include <tuple>

class DFA
{
  private:
    // (current state, char) -> new state
    std::map<std::tuple<int, char>, int> dfa;

  public:
    // all the possible states
    enum state
    {
        START,
        IDENTIFIER,
        INTEGER,
        FLOAT,
        DIVISION_OR_COMMENT,
        SINGLE_LINE_COMMENT,
        MULTIPLE_LINE_COMMENT_PRESTATE,
        MULTIPLE_LINE_COMMENT,
        BYTE,
        STRING,
        STRING_ESCAPE,
        ADDITION,
        UNARY_ADDITION,
        SUBTRACTION,
        UNARY_SUBTRACTION,
        ASSIGNMENT,
        EQUIVALENCE,
        CHAR,
        NEGATION,
        NON_EQUIVALENCE,
        MULTIPLICATION,
        EXPONENTIATION,
        AND_PRESTATE,
        AND,
        OR_PRESTATE,
        OR,
        LESS_THAN,
        LESS_THAN_EQUAL,
        LARROW,
        GREATER_THAN,
        GREATER_THAN_EQUAL,
        RARROW,
        UNDERSCORE,
        COMMA,
        LBRACKET,
        RBRACKET,
        LCURLEY,
        RCURLEY,
        LPAREN,
        RPAREN,
        MODULO,
        DONE,
        ERROR
    };
    // constructor
    DFA();
    // since we want to keep the dfa immutable, we'll keep it encapsulated here
    // and only have a public method to get next state
    int getNextState(int, char);
};