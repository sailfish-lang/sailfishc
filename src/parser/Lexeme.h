/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../lexar/Lexar2.h"
#include "../lexar/Token2.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

enum class OP
{
    SOURCE,
    SOURCE_PART,
    IMPORT,
    UDT,
    UDA,
    START,
    SCRIPT,
    VARIABLE,
    FUNCTION,
    FUNCTION_INFO,
    FUNCTION_IN_OUT,
    TREE,
    BRANCH,
    E0,
    EXPONENT,
    MULTIPLICATION,
    DIVISION,
    MODULO,
    ADDITION,
    SUBTRACTION,
    LESS_THAN,
    LESS_THAN_OR_EQUALS,
    GREATER_THAN,
    GREATER_THAN_OR_EQUALS,
    EQUIVALENCE,
    NONEQUIVALENCE,
    AND,
    OR,
    ASSIGNMENT,
    NEGATION,
    END,
    PRIMARY,
    DECLARATION,
    RETURN,
    ATTRIBUTE,
    METHOD,
    FUNCTION_INPUT,
    STATEMENT,
    BLOCK,
    NULL_VAL,
    MEMBER,
    ATTRIBUTE_ACCESS,
    METHOD_ACCESS,
    INPUT,
    FUNCTION_CALL,
    UDTDEC,
    UDTDECITEM,
    UNARYADD,
    UNARYMINUS,
    ADDTO,
    SUBFROM,
    DIVFROM,
    MULTTO,
};
std::string disp(OP o);

enum class LIT
{
    IDENTIFIER,
    STRING,
    BOOLEAN,
    INTEGER,
    FLOAT,
};
std::string disp(LIT l);

struct LeafLexeme;
struct NodeLexeme;
typedef std::variant<std::shared_ptr<NodeLexeme>, std::shared_ptr<LeafLexeme>>
    Lexeme;

struct LeafLexeme
{
    LIT lit;
    std::string value;
    LeafLexeme(LIT l, std::string v) : lit(l), value(v){};
};
std::shared_ptr<LeafLexeme> makeLeaf(LIT l, std::string v);

struct NodeLexeme
{
    OP op;
    Lexeme left;
    Lexeme right;
    NodeLexeme(OP o, Lexeme l, Lexeme r) : op(o), left(l), right(r){};
};
std::shared_ptr<NodeLexeme> makeNode(OP o, Lexeme, Lexeme);
std::shared_ptr<NodeLexeme> makeNode(OP o, Lexeme);
std::shared_ptr<NodeLexeme> makeNullNode();
