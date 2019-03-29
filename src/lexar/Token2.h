/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <iostream>
#include <string>

namespace Tokenn
{
enum Kind
{
    // Keywords
    START,
    OWN_ACCESSOR,
    TREE,
    BRANCH,
    UAT,
    UFA,
    FUN,
    DEC,
    EXP,
    IMPORT,
    RETURN,
    NEW,

    // Basic Literals
    IDENTIFIER,
    INTEGER,
    FLOAT,
    BOOL,
    STRING,

    // Compairsons, Arithmetic, etc.
    MODULO,
    ADDITION,
    ADDTO,
    SUBTRACTION,
    SUBFROM,
    MULTIPLICATION,
    EXPONENTIATION,
    MULTTO,
    DIVISION,
    DIVFROM,
    GREATER_THAN,
    GREATER_THAN_OR_EQUALS,
    LESS_THAN,
    LESS_THAN_OR_EQUALS,
    AND,
    OR,
    EQUIVALENCE,
    NONEQUIVALENCE,
    ASSIGNMENT,
    NEGATION,

    // Structure
    COMMENT,

    // Punctuation
    UNDERSCORE,
    COMMA,
    LCURLEY,
    RCURLEY,
    LPAREN,
    RPAREN,
    DOT,
    TRIPLE_DOT,
    PIPE,
    COLON,

    // Else
    ERROR,
    EOF_,
};

std::string displayKind(const Kind&);
}

struct Token2
{
    Tokenn::Kind kind;
    std::string value;
    int col;
    int line;
};

std::string prettifyFormatToken(const Token2&);
