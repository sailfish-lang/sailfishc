/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/ast/Ast.h"
#include "../src/lexar/Lexar.h"
#include "../src/parser/Parser.h"
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
        "ExportDefinition",
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
        "IntegerLiteral"
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
        "IntegerLiteral",
        "Division",
        "ExpressionStatement",
        "Identifier",
        "ReturnStatement",
        "Identifier",
        "AttributeAccess",
        "ExpressionStatement",
        "IntegerLiteral",
        "ArrayExpression",
        "ExpressionStatement",
        "Identifier",
        "ReturnStatement",
        "Identifier",
        "Identifier",
        "FunctionCall",
        "MethodAccess",
        "ExpressionStatement",
        "IfStatement",
        "Identifier"
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

    for (auto const& a : v->getInOrderTraversal(root))
    {
    }
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}