/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <iostream>
#include <string>

enum class TokenKind
{
    // Keywords
    START,
    OWN_ACCESSOR,
    TREE,
    UAT,
    UFN,
    IFE,
    FUN,
    DEC,
    IMPORT,
    RETURN,
    NEW,

    // Basic Literals
    IDENTIFIER,
    INTEGER,
    FLOAT,
    BOOL,
    STRING,
    LIST,

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

    // unaries
    NEGATION,
    UNARYADD,
    UNARYMINUS,

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
    LISTTYPE,
    ERROR,
    EOF_,
};

std::string displayKind(const TokenKind&);

struct Token2
{
    TokenKind kind;
    std::string value;
    int col;
    int line;
    Token2(TokenKind k, const std::string& v, int c, int l)
        : kind(k), value(v), col(c), line(l){};
    std::string prettifyFormatToken();
};
