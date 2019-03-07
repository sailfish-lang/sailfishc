/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/ast/Ast.h"
#include "../src/lexar/Lexar.h"
#include "../src/parser/Parser.h"
#include "../src/semant/DictionarySymbol.h"
#include "../src/semant/FunctionSymbol.h"
#include "../src/semant/ListSymbol.h"
#include "../src/semant/PrimitiveSymbol.h"
#include "../src/semant/Symbol.h"
#include "../src/semant/SymbolTable.h"
#include "../src/semant/UDTSymbol.h"
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
        "Identifier: 1",
        "Identifier: \"hello\"",
        "DictionaryItem",
        "DictionaryLiteral",
        "NewExpression",
        "DictionaryDefinition",
        "ExportDefintion",
        "Identifier: someList",
        "Identifier: 1.0",
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

TEST(SymbolDataStructure, Symbol)
{
    // test primitive and defaults of Symbol abstract class
    Symbol* p = new PrimitiveSymbol("int");
    ASSERT_EQ(p->getType(), "int");
    ASSERT_EQ(p->getSymbolType(), Symbol::Primitive);
    ASSERT_EQ(p->getMethods(), nullptr);
    ASSERT_EQ(p->getAttributes(), nullptr);
    ASSERT_EQ(p->getKeyType(), "");
    ASSERT_EQ(p->getValueType(), "");
    ASSERT_EQ(p->getInputTypes().size(), 0);
    ASSERT_EQ(p->getOutputTypes().size(), 0);

    // test list
    Symbol* l = new ListSymbol("int[]");
    ASSERT_EQ(l->getType(), "int[]");
    ASSERT_EQ(l->getSymbolType(), Symbol::List);

    // test dictionary
    Symbol* d = new DictionarySymbol("dictionary", "str", "int");
    ASSERT_EQ(d->getType(), "dictionary");
    ASSERT_EQ(d->getSymbolType(), Symbol::Dictionary);
    ASSERT_EQ(d->getKeyType(), "str");
    ASSERT_EQ(d->getValueType(), "int");

    // test function
    std::vector<std::string> inputs{"int", "bool"};
    std::vector<std::string> outputs{"void"};
    Symbol* f = new FunctionSymbol("function", inputs, outputs);
    ASSERT_EQ(f->getType(), "function");
    ASSERT_EQ(f->getSymbolType(), Symbol::Function);
    ASSERT_EQ(f->getInputTypes().size(), 2);
    ASSERT_EQ(f->getOutputTypes().size(), 1);

    // test UDT -- SymbolTable will change thus just going nullptr for now
    Symbol* u = new UDTSymbol("udt", nullptr, nullptr);
    ASSERT_EQ(u->getType(), "udt");
    ASSERT_EQ(u->getSymbolType(), Symbol::UDT);
    ASSERT_EQ(u->getAttributes(), nullptr);
    ASSERT_EQ(u->getMethods(), nullptr);
}

TEST(SymbolTableDataStructure, SymbolTable)
{
    SymbolTable* st = new SymbolTable();

    // -- start in the global scope
    // add a new primitive
    st->addSymbol("primitive", "int", Symbol::SymbolType::Primitive);
    ASSERT_EQ(st->hasVariable("primitive"), true);

    // add a new function
    std::vector<std::string> inputs{"int", "bool"};
    std::vector<std::string> outputs{"void"};
    st->addSymbol("somefunction", "function", inputs, outputs,
                  Symbol::SymbolType::Function);

    ASSERT_EQ(st->hasVariable("somefunction"), true);

    // // -- start in a nested scope inside the function
    st->enterScope();
    // // add a new list
    st->addSymbol("someArr", "int[]", Symbol::SymbolType::List);
    ASSERT_EQ(st->hasVariable("someArr"), true);

    // // -- exit nested scope
    st->exitScope();
    ASSERT_EQ(st->hasVariable("someArr"), false);

    // // add a new dictionary
    st->addSymbol("someDict", "dictionary", "int", "str",
                  Symbol::SymbolType::Dictionary);
    ASSERT_EQ(st->hasVariable("someDict"), true);

    // -- enter a scope
    st->enterScope();

    // add a previously deleted list
    st->addSymbol("someArr", "int[]", Symbol::SymbolType::List);
    ASSERT_EQ(st->hasVariable("someArr"), true);

    // construct a symbol table for UDT attributes
    SymbolTable* st_atts = new SymbolTable();
    st_atts->addSymbol("foo", "flt", Symbol::SymbolType::Primitive);
    // construct a symbol table for UDT methods
    SymbolTable* st_meths = new SymbolTable();
    st_meths->addSymbol("somefunction", "function", inputs, outputs,
                        Symbol::SymbolType::Function);
    // add a UDT
    st->addSymbol("FOOUDT", "FOO", st_atts, st_meths, Symbol::SymbolType::UDT);
    ASSERT_EQ(st->hasVariable("FOOUDT"), true);
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}