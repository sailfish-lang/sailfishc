/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/ast/Ast.h"
#include "../src/common/ReservedWords.h"
#include "../src/lexar/Lexar.h"
#include "../src/parser/Parser.h"
#include "../src/semant/SymbolTable.h"
#include "../src/visitor/InOrderTraversal.h"
#include <gtest/gtest.h>

TEST(LexarTest, HelloWorld)
{
    static const Kind expected[] = {Kind::COMMENT_TOKEN, Kind::START_TOKEN,
                                    Kind::LCURLEY_TOKEN, Kind::IDENTIFIER_TOKEN,
                                    Kind::LPAREN_TOKEN,  Kind::STRING_TOKEN,
                                    Kind::RPAREN_TOKEN,  Kind::RCURLEY_TOKEN};

    Lexar* lexar = new Lexar("../sailfish_examples/helloworld.fish");

    Token* t;
    int i = 0;

    t = lexar->getNextToken();

    while (!t->isEOF())
    {
        ASSERT_EQ(expected[i], t->getKind());
        t = lexar->getNextToken();
        ++i;
    }
}

TEST(LexarTest, Nonsense)
{
    static const Kind expected[] = {
        Kind::START_TOKEN,      Kind::LCURLEY_TOKEN,    Kind::KEYWORD_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::INTEGER_TOKEN,    Kind::FLOAT_TOKEN,
        Kind::COMMENT_TOKEN,    Kind::STRING_TOKEN,     Kind::STRING_TOKEN,
        Kind::BYTE_TOKEN,       Kind::BYTE_TOKEN,       Kind::KEYWORD_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::OPERATION_TOKEN,  Kind::INTEGER_TOKEN,
        Kind::LOGIC_TOKEN,      Kind::LOGIC_TOKEN,      Kind::ARROW_TOKEN,
        Kind::ARROW_TOKEN,      Kind::UNDERSCORE_TOKEN, Kind::COMMA_TOKEN,
        Kind::LBRACKET_TOKEN,   Kind::RBRACKET_TOKEN,   Kind::LPAREN_TOKEN,
        Kind::RPAREN_TOKEN,     Kind::ERROR_TOKEN,      Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::COMMENT_TOKEN,    Kind::PIPE_TOKEN,       Kind::PIPE_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::BOOL_TOKEN,
        Kind::BOOL_TOKEN,       Kind::LFISH_TAIL_TOKEN, Kind::RFISH_TAIL_TOKEN,
        Kind::RCURLEY_TOKEN,
    };

    Lexar* lexar = new Lexar("../sailfish_examples/nonsense_lexar.fish");

    Token* t;
    int i = 0;

    t = lexar->getNextToken();

    while (!t->isEOF())
    {
        ASSERT_EQ(expected[i], t->getKind());
        t = lexar->getNextToken();
        ++i;
    }
}

TEST(ParserTest, AllTokens)
{
    static const std::string expected[] = {
        "Identifier: someDict",
        "DictionaryItem",
        "DictionaryLiteral",
        "NewExpression",
        "DictionaryDefinition",
        "ExportDefintion",
        "Identifier: someList",
        "ListItem",
        "ListLiteral",
        "NewExpression",
        "ListDefinition",
        "Identifier: foo",
        "Identifier: void",
        "Typename: void",
        "Variable",
        "Input",
        "Typename: void",
        "Output",
        "Identifier: i",
        "FloatLiteral: 10.0",
        "ExpressionStatement",
        "Addition",
        "ReturnStatement",
        "FunctionDefinition",
        "Identifier: foo",
        "IntegerLiteral: 10",
        "IntegerLiteral: 1",
        "IntegerLiteral: 2",
        "IntegerLiteral: 4",
        "IntegerLiteral: 7",
        "ExpressionStatement",
        "Modulo",
        "ExpressionStatement",
        "Negation",
        "ExpressionStatement",
        "OrComparison",
        "ExpressionStatement",
        "AndComparison",
        "ExpressionStatement",
        "EquivalenceComparison",
        "ExpressionStatement",
        "Assignment",
        "ExpressionStatement",
        "IntegerLiteral: 10",
        "IntegerLiteral: 11",
        "IntegerLiteral: 2",
        "ExpressionStatement",
        "NonEquivalenceComparison",
        "ExpressionStatement",
        "Subtraction",
        "ExpressionStatement",
        "IfStatement",
        "InitialExecutionBody",
    };

    Parser* parser = new Parser();
    ast::Start* root = parser->parse("../sailfish_examples/parser.fish");

    InOrderTraversal* v = new InOrderTraversal();

    int i = 0;
    for (auto const& a : v->getInOrderTraversal(root))
    {
        ASSERT_EQ(expected[i], a);
        ++i;
    }
}

TEST(SymbolTableDataStructure, SymbolTable)
{
    SymbolTable* st = new SymbolTable();

    // -- start in the global scope ---------------------- SCOPE: 0
    // add a new primitive
    st->addSymbol("primitive", "int");
    ASSERT_EQ(st->hasVariable("primitive"), true);
    ASSERT_EQ(st->getSymbolScope("primitive"), 0);

    // add a new function
    st->addSymbol("somefunction", "function");
    ASSERT_EQ(st->hasVariable("somefunction"), true);
    ASSERT_EQ(st->getSymbolScope("somefunction"), 0);

    // // -- start in a nested scope inside the function  --------- SCOPE: 1
    st->enterScope();
    ASSERT_EQ(st->isGlobalScope(), false);
    // // add a new list
    st->addSymbol("someArr", "int[]");
    ASSERT_EQ(st->hasVariable("someArr"), true);
    ASSERT_EQ(st->getSymbolScope("someArr"), 1);

    // // -- exit nested scope  ---------------------- SCOPE: 0
    st->exitScope();
    ASSERT_EQ(st->isGlobalScope(), true);
    ASSERT_EQ(st->hasVariable("someArr"), false);

    // // add a new dictionary
    st->addSymbol("someDict", "dictionary");
    ASSERT_EQ(st->hasVariable("someDict"), true);
    ASSERT_EQ(st->getSymbolScope("someDict"), 0);

    // add a UDT
    st->addSymbol("FOOUDT", "FOO");
    ASSERT_EQ(st->hasVariable("FOOUDT"), true);
    ASSERT_EQ(st->getSymbolScope("FOOUDT"), 0);

    // -- enter a scope  ---------------------- SCOPE: 1
    st->enterScope();

    // add a previously deleted list
    st->addSymbol("someArr", "int[]");
    ASSERT_EQ(st->hasVariable("someArr"), true);
    ASSERT_EQ(st->getSymbolScope("someArr"), 1);

    // -- enter a scope  ---------------------- SCOPE: 2
    st->enterScope();

    // add a previously added list for a new scope and a different type
    st->addSymbol("someArr", "int");
    ASSERT_EQ(st->hasVariable("someArr"), true);
    ASSERT_EQ(st->getSymbolScope("someArr"), 2);
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}