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

// OP represents the AST node type for a given nonterminal
enum class OP
{
    ADDITION,
    ADDTO,
    AND,
    ASSIGNMENT,
    ATTRIBUTE_ACCESS,
    ATTRIBUTE,
    BLOCK,
    BRANCH,
    DECLARATION,
    DIVFROM,
    DIVISION,
    E0,
    END,
    EQUIVALENCE,
    EXPONENT,
    FUNCTION_CALL,
    FUNCTION_IN_OUT,
    FUNCTION_INFO,
    FUNCTION_INPUT,
    FUNCTION,
    GREATER_THAN_OR_EQUALS,
    GREATER_THAN,
    IMPORT,
    INPUT,
    LESS_THAN_OR_EQUALS,
    LESS_THAN,
    MEMBER,
    METHOD_ACCESS,
    METHOD,
    MODULO,
    MULTIPLICATION,
    MULTTO,
    NEGATION,
    NONEQUIVALENCE,
    NULL_VAL,
    OR,
    PRIMARY,
    RETURN,
    SCRIPT,
    SOURCE_PART,
    SOURCE,
    START,
    STATEMENT,
    SUBFROM,
    SUBTRACTION,
    TREE,
    UDA,
    UDT,
    UDTDEC,
    UDTDECITEM,
    UNARYADD,
    UNARYMINUS,
    VARIABLE,
    LISTITEM,
    LIST,
};
std::string disp(OP o);

// LIT represents the AST node type for a given terminal
enum class LIT
{
    BOOLEAN,
    FLOAT,
    IDENTIFIER,
    INTEGER,
    LISTTYPE,
    STRING,
    TYPE,
    OWN,
    EMPTY,
};
std::string disp(LIT l);

// node = nonterminal and leaf = terminal
struct LeafLexeme;
struct NodeLexeme;

// type alias for more legible code
using LeafPtr = std::unique_ptr<LeafLexeme>;
using NodePtr = std::unique_ptr<NodeLexeme>;

// Lexeme is short for the variant that may either be a node or a leaf,
// convenient for nonterminals that have one or two children
// using Lexeme =
//     std::variant<std::shared_ptr<NodeLexeme>, std::shared_ptr<LeafLexeme>>;
using Lexeme = std::variant<LeafPtr, NodePtr>;

struct LeafLexeme
{
    LIT lit;
    std::string value;
    LeafLexeme(LIT l, std::string v) : lit(l), value(v){};
};
LeafPtr makeLeaf(LIT l, std::string v);

struct NodeLexeme
{
    OP op;
    Lexeme left;
    Lexeme right;
    NodeLexeme(OP o, Lexeme l, Lexeme r)
        : op(o), left(std::move(l)), right(std::move(r)){};
};

// overloaded since nonterminal may have only a single child, in that case we
// utilize the null node constructor
NodePtr makeNode(OP o, Lexeme, Lexeme);
NodePtr makeNode(OP o, const std::string&, Lexeme);
NodePtr makeNode(OP o, Lexeme);
NodePtr makeNullNode();
