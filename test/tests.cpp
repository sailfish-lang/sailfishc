/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/ast/Ast.h"
#include "../src/lexar/Lexar.h"
#include "../src/parser/Parser.h"
#include "../src/semant/symbol_table/DictionarySymbol.h"
#include "../src/semant/symbol_table/FunctionSymbol.h"
#include "../src/semant/symbol_table/ListSymbol.h"
#include "../src/semant/symbol_table/PrimitiveSymbol.h"
#include "../src/semant/symbol_table/ScopeStack.h"
#include "../src/semant/symbol_table/Symbol.h"
#include "../src/semant/symbol_table/UDTSymbol.h"
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
        Kind::RPAREN_TOKEN,     Kind::ERROR_TOKEN,      Kind::DOT_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::COMMENT_TOKEN,    Kind::PIPE_TOKEN,
        Kind::PIPE_TOKEN,       Kind::TRIPLE_DOT_TOKEN, Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::BOOL_TOKEN,       Kind::BOOL_TOKEN,
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
        "Identifier",
        "Identifier",
        "Identifier",
        "DictionaryItem",
        "DictionaryLiteral",
        "NewExpression",
        "Assignment",
        "DictionaryDefinition",
        "ExportDefintion",
        "Identifier",
        "Identifier",
        "ListItem",
        "Identifier",
        "ListItem",
        "ListLiteral",
        "NewExpression",
        "Assignment",
        "ListDefinition",
        "Identifier",
        "Identifier",
        "Typename",
        "Variable",
        "Input",
        "Typename",
        "Output",
        "FunctionDefinition",
        "Identifier",
        "Identifier",
        "Typename",
        "Variable",
        "UserDefinedTypeAttributes",
        "Identifier",
        "Identifier",
        "Identifier",
        "Typename",
        "Variable",
        "Input",
        "Typename",
        "Output",
        "FunctionDefinition",
        "UserDefinedTypeMethods",
        "UserDefinedTypeDefinition",
        "Identifier",
        "Typename",
        "Variable",
        "FloatLiteral",
        "Assignment",
        "NewVariableDefinition",
        "IntegerLiteral",
        "IntegerLiteral",
        "EquivalenceComparison",
        "BooleanLiteral",
        "AndComparison",
        "OrComparison",
        "StringLiteral",
        "BinaryGreaterThan",
        "IntegerLiteral",
        "BinaryLessThan",
        "IntegerLiteral",
        "BinaryGreaterThanOrEqual",
        "IntegerLiteral",
        "BinaryLessThanOrEqual",
        "IntegerLiteral",
        "NonEquivalenceComparison",
        "Identifier",
        "ExpressionStatement",
        "IntegerLiteral",
        "Assignment",
        "ExpressionStatement",
        "IntegerLiteral",
        "Addition",
        "ExpressionStatement",
        "IntegerLiteral",
        "Subtraction",
        "ExpressionStatement",
        "IntegerLiteral",
        "Modulo",
        "ExpressionStatement",
        "IntegerLiteral",
        "Multiplication",
        "ExpressionStatement",
        "Identifier",
        "Division",
        "ExpressionStatement",
        "IntegerLiteral",
        "ArrayExpression",
        "ExpressionStatement",
        "Identifier",
        "ReturnStatement",
        "Identifier",
        "AttributeAccess",
        "ExpressionStatement",
        "Identifier",
        "ReturnStatement",
        "Identifier",
        "Identifier",
        "FunctionCall",
        "MethodAccess",
        "ExpressionStatement",
        "IfStatement",
        "Identifier",
        "Typename",
        "Variable",
        "Identifier",
        "Identifier",
        "Identifier",
        "DictionaryItem",
        "UserDefinedType",
        "NewExpression",
        "Assignment",
        "NewVariableDefinition",
        "InitialExecutionBody",
    };

    Parser* parser = new Parser();
    ast::Start* root = parser->parse("../sailfish_examples/parser.fish");

    InOrderTraversal* v = new InOrderTraversal();

    int i = 0;
    for (auto const& a : v->getInOrderTraversal(root))
    {
        std::cout << a << " : " << expected[i] << '\n';
        ASSERT_EQ(expected[i], a);
        ++i;
    }
}

TEST(ScopeStackDataStructureTest, ScopeStackNode)
{
    ScopeStackNode* a = new ScopeStackNode(1, "int");
    ASSERT_EQ(a->getLevel(), 1);
    ASSERT_EQ(a->getType(), "int");

    ScopeStackNode* b = new ScopeStackNode(2, "flt");
    a->setPrev(b);
    ASSERT_EQ(a->getPrev()->getLevel(), 2);
    ASSERT_EQ(a->getPrev()->getType(), "flt");
    ASSERT_EQ(a->hasPrev(), true);
    ASSERT_EQ(a->getPrev()->hasPrev(), false);
}

TEST(ScopeStackDataStructureTest, ScopeStack)
{
    ScopeStack* stack = new ScopeStack();

    // test start size
    ASSERT_EQ(stack->peek(), nullptr);

    // test simple push
    stack->push(1, "int");
    ASSERT_EQ(stack->peek()->getLevel(), 1);
    ASSERT_EQ(stack->peek()->getType(), "int");

    // test simple pop
    stack->pop();
    ASSERT_EQ(stack->peek(), nullptr);

    // test more complex case
    stack->push(1, "int");
    stack->push(2, "flt");
    stack->push(3, "str");
    stack->push(4, "bool");
    stack->push(5, "void");
    stack->pop();
    stack->pop();
    stack->pop();
    ASSERT_EQ(stack->peek()->getLevel(), 2);
    ASSERT_EQ(stack->peek()->getType(), "flt");

    // test popping of an empty stack
    stack->clear();
    stack->pop();
    ASSERT_EQ(stack->peek(), nullptr);
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
int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}