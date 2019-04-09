/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/ast/Ast.h"
#include "../src/common/ReservedWords.h"
#include "../src/errorhandler/Error.h"
#include "../src/lexar/Lexar.h"
#include "../src/lexar/Lexar2.h"
#include "../src/parser/Parser.h"
#include "../src/parser/Parser2.h"
#include "../src/semantics/SemanticAnalyzer.h"
#include "../src/semantics/SymbolMetaData.h"
#include "../src/semantics/SymbolTable.h"
#include "../src/semantics/TypeChecker.h"
#include "../src/semantics/UDTTable.h"
#include "visitors/InOrderTraversal.h"
#include <gtest/gtest.h>
#include <memory>
#include <variant>

std::unique_ptr<Token2>
makeToken(TokenKind kind, std::string value, int col, int line)
{
    return std::make_unique<Token2>(kind, value, col, line);
}

TEST(LexarTest, Lex2)
{
    std::unique_ptr<Token2> expected[] = {
        makeToken(TokenKind::START, "start", 1, 1),
        makeToken(TokenKind::LCURLEY, "{", 1, 7),
        makeToken(TokenKind::OWN_ACCESSOR, "own", 2, 5),
        makeToken(TokenKind::TREE, "Tree", 2, 9),
        makeToken(TokenKind::DEC, "dec", 2, 14),
        makeToken(TokenKind::LPAREN, "(", 2, 18),
        makeToken(TokenKind::UAT, "Ufa", 3, 9),
        makeToken(TokenKind::UFN, "Ufn", 3, 13),
        makeToken(TokenKind::COMMENT, "# hello", 4, 9),
        makeToken(TokenKind::IMPORT, "import", 5, 9),
        makeToken(TokenKind::RETURN, "return", 6, 9),
        makeToken(TokenKind::NEW, "new", 6, 16),
        makeToken(TokenKind::IDENTIFIER, "something", 7, 9),
        makeToken(TokenKind::INTEGER, "1", 7, 19),
        makeToken(TokenKind::FLOAT, "1.0", 7, 21),
        makeToken(TokenKind::BOOL, "true", 7, 25),
        makeToken(TokenKind::BOOL, "false", 7, 30),
        makeToken(TokenKind::STRING, "\"hello\"", 8, 9),
        makeToken(TokenKind::MODULO, "%", 9, 9),
        makeToken(TokenKind::ADDITION, "+", 9, 10),
        makeToken(TokenKind::ADDTO, "+=", 9, 11),
        makeToken(TokenKind::SUBTRACTION, "-", 9, 13),
        makeToken(TokenKind::SUBFROM, "-=", 9, 14),
        makeToken(TokenKind::EXPONENTIATION, "**", 9, 16),
        makeToken(TokenKind::ASSIGNMENT, "=", 9, 18),
        makeToken(TokenKind::MULTTO, "*=", 9, 19),
        makeToken(TokenKind::MULTIPLICATION, "*", 9, 21),
        makeToken(TokenKind::DIVISION, "/", 9, 22),
        makeToken(TokenKind::DIVFROM, "/=", 9, 23),
        makeToken(TokenKind::GREATER_THAN_OR_EQUALS, ">=", 9, 25),
        makeToken(TokenKind::GREATER_THAN, ">", 9, 27),
        makeToken(TokenKind::LESS_THAN, "<", 9, 28),
        makeToken(TokenKind::LESS_THAN_OR_EQUALS, "<=", 9, 29),
        makeToken(TokenKind::EQUIVALENCE, "==", 9, 32),
        makeToken(TokenKind::NONEQUIVALENCE, "!=", 9, 33),
        makeToken(TokenKind::NEGATION, "!", 9, 35),
        makeToken(TokenKind::AND, "and", 9, 36),
        makeToken(TokenKind::OR, "or", 9, 40),
        makeToken(TokenKind::UNDERSCORE, "_", 10, 9),
        makeToken(TokenKind::COLON, ":", 10, 10),
        makeToken(TokenKind::COMMA, ",", 10, 11),
        makeToken(TokenKind::TRIPLE_DOT, "...", 10, 12),
        makeToken(TokenKind::DOT, ".", 10, 15),
        makeToken(TokenKind::PIPE, "|", 10, 16),
        makeToken(TokenKind::STRING, "\"\"trickystring\"", 10, 17),
        makeToken(TokenKind::RPAREN, ")", 11, 5),
        makeToken(TokenKind::RCURLEY, "}", 12, 1),
    };

    Lexar2* lexar2 = new Lexar2("./examples/Lexar2.fish");

    int i = 0;

    auto t = lexar2->getNextToken();

    while (t->kind != TokenKind::EOF_)
    {
        auto flag = false;
        if (t->kind == expected[i]->kind && t->value == expected[i]->value &&
            t->col == expected[i]->col && t->line == expected[i]->line)
            flag = true;
        ASSERT_EQ(flag, true);
        t = lexar2->getNextToken();
        ++i;
    }
}

TEST(LexarTest, HelloWorld)
{
    static const Kind expected[] = {Kind::COMMENT_TOKEN, Kind::START_TOKEN,
                                    Kind::LCURLEY_TOKEN, Kind::IDENTIFIER_TOKEN,
                                    Kind::LPAREN_TOKEN,  Kind::STRING_TOKEN,
                                    Kind::RPAREN_TOKEN,  Kind::RCURLEY_TOKEN};

    Lexar* lexar = new Lexar("./examples/helloworld.fish");

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
        Kind::KEYWORD_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::OPERATION_TOKEN,
        Kind::INTEGER_TOKEN,    Kind::LOGIC_TOKEN,      Kind::LOGIC_TOKEN,
        Kind::ARROW_TOKEN,      Kind::ARROW_TOKEN,      Kind::UNDERSCORE_TOKEN,
        Kind::COMMA_TOKEN,      Kind::LPAREN_TOKEN,     Kind::RPAREN_TOKEN,
        Kind::ERROR_TOKEN,      Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::COMMENT_TOKEN,
        Kind::PIPE_TOKEN,       Kind::PIPE_TOKEN,       Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::BOOL_TOKEN,       Kind::BOOL_TOKEN,
        Kind::RCURLEY_TOKEN,
    };

    Lexar* lexar = new Lexar("./examples/nonsense_lexar.fish");

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
        "Identifier: foo",
        "Identifier: ",
        "Typename: void",
        "Variable",
        "Input",
        "Typename: void",
        "Output",
        "Identifier: i",
        "FloatLiteral: 10.0",
        "BinaryCompOrArith",
        "ReturnStatement",
        "FunctionDefinition",
        "Identifier: foo",
        "IntegerLiteral: 10",
        "IntegerLiteral: 1",
        "IntegerLiteral: 2",
        "IntegerLiteral: 4",
        "IntegerLiteral: 7",
        "BinaryCompOrArith",
        "Negation",
        "BinaryCompOrArith",
        "BinaryCompOrArith",
        "BinaryCompOrArith",
        "Assignment",
        "IntegerLiteral: 10",
        "IntegerLiteral: 11",
        "IntegerLiteral: 2",
        "BinaryCompOrArith",
        "BinaryCompOrArith",
        "IfStatement",
        "InitialExecutionBody",
    };

    Parser* parser = new Parser();
    ast::Start* root = parser->parse("./examples/parser.fish");

    InOrderTraversal* v = new InOrderTraversal();

    int i = 0;
    for (auto const& a : v->getInOrderTraversal(root))
    {
        ASSERT_EQ(expected[i], a);
        ++i;
    }
}

TEST(ParserTest, Parser2UDT)
{
    static const std::string expected[] = {"Source",
                                           "Import",
                                           "Import",
                                           "foo",
                                           "\"foo.fish\"",
                                           "Import",
                                           "Import",
                                           "bar",
                                           "\"bar.fish\"",
                                           "Script",
                                           "Function",
                                           "Function",
                                           "foo",
                                           "Function Info",
                                           "Function In Out",
                                           "Function Input",
                                           "Variable",
                                           "int",
                                           "i",
                                           "Function Input",
                                           "Variable",
                                           "flt",
                                           "f",
                                           "void",
                                           "Block",
                                           "Statement",
                                           "Return",
                                           "Primary",
                                           "i",
                                           "Start",
                                           "start",
                                           "Block",
                                           "Statement",
                                           "Tree",
                                           "Branch",
                                           "Branch",
                                           "Equivalence",
                                           "Primary",
                                           "1",
                                           "And",
                                           "Primary",
                                           "2",
                                           "Less than",
                                           "Primary",
                                           "2",
                                           "Primary",
                                           "3",
                                           "Block",
                                           "Branch",
                                           "Branch",
                                           "Negation",
                                           "Primary",
                                           "true",
                                           "Block",
                                           "Statement",
                                           "Declaration",
                                           "Variable",
                                           "flt",
                                           "f",
                                           "Primary",
                                           "12.0",
                                           "Statement",
                                           "Declaration",
                                           "Variable",
                                           "[int]",
                                           "is",
                                           "Primary",
                                           "[1,2,3]",
                                           "Statement",
                                           "Declaration",
                                           "Variable",
                                           "Foo",
                                           "f",
                                           "Member",
                                           "UDT dec",
                                           "Foo",
                                           "UDT dec item",
                                           "UDT dec item",
                                           "f",
                                           "i",
                                           "Statement",
                                           "Member",
                                           "Attribute Access",
                                           "f",
                                           "Add to",
                                           "Member",
                                           "Method Access",
                                           "Function Call",
                                           "f",
                                           "Addition",
                                           "Unary add",
                                           "Primary",
                                           "1",
                                           "Member",
                                           "Attribute Access",
                                           "f"};

    Parser2* p = new Parser2("./examples/parser2_script.fish");
    auto n = p->parse();

    std::vector<std::string> out;

    const auto& func = [&out](std::string s) mutable { out.push_back(s); };

    p->postorder(std::move(n), func);

    int i = 0;
    for (auto const& a : out)
    {
        ASSERT_EQ(expected[i], a);
        ++i;
    }
}

TEST(ParserTest, Parser2Script)
{
    static const std::string expected[] = {"Source",
                                           "Udt",
                                           "Attribute",
                                           "Attribute",
                                           "Variable",
                                           "flt",
                                           "f",
                                           "Method",
                                           "Method",
                                           "Function",
                                           "foo",
                                           "Function Info",
                                           "Function In Out",
                                           "Function Input",
                                           "Variable",
                                           "int",
                                           "i",
                                           "void",
                                           "Block"};

    Parser2* p = new Parser2("./examples/parser2_udt.fish");
    auto n = p->parse();

    std::vector<std::string> out;

    const auto& func = [&out](std::string s) mutable { out.push_back(s); };

    p->postorder(std::move(n), func);

    int i = 0;
    for (auto const& a : out)
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

TEST(SemanticTest, TypeCheckerFunctions)
{
    static const std::string expected[] = {
        "Declared type: nonExistentType for variable named: foo is not a "
        "legal "
        "or known type.",
        "Declared function input named: flt illegally shares its name with "
        "a "
        "type or a keyword/reserved word.",
        "Declared type: nonExistentType for variable named: "
        "nonExistentType is "
        "not a legal or known type.",
        "Actual return type of: int does not match expected return type "
        "of: "
        "nonExistentType.",
        "Declared variable named: flt illegally shares its name with a "
        "type or "
        "a keyword/reserved word.",
        "Function: foo is not defined.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and unknown.",
        "Supplied argument type of: int does not match expected type of: "
        "str.",
        "Not enough args supplied to function: good.",
        "Too many args supplied to function: good.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_functions.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerPrimitiveDeclerations)
{
    static const std::string expected[] = {
        "Declared variable named: dec illegally shares its name with a "
        "type or "
        "a "
        "keyword/reserved word.",
        "Declared type: flt of primitive for variable named: function_foo "
        "does "
        "not match assigned expression type of: int.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_primitives.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

// NOTE: all inherently test assignment
TEST(SemanticTest, TypeCheckerBinariesAndUnaries)
{
    static const std::string expected[] = {
        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead "
        "received:"
        " int and flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",

        "Expected int type on left side of operation. Instead "
        "received: flt.",
        "Expected int type on left side of operation. Instead received: "
        "bool.",

        "Expected int type on left side of operation. Instead "
        "received: flt.",
        "Expected int type on left side of operation. Instead received: "
        "bool.",

        "Expected the same types on each side of operation. Instead "
        "received: "
        "bool and int.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and bool.",

        "Expected the same types on each side of operation. Instead "
        "received: "
        "bool and int.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and bool.",

        "Expected boolean type on right side of operation. Instead "
        "received: "
        "int.",
        "Expected boolean type on left side of operation. Instead "
        "received: "
        "int.",

        "Expected boolean type on right side of operation. Instead "
        "received: "
        "int.",
        "Expected boolean type on left side of operation. Instead "
        "received: "
        "int.",

        "Expected boolean type following negation. Instead received: int."};

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_binaries_unaries.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerUDTDec)
{
    static const std::string expected[] = {
        "Declared variable named: bool illegally shares its name with a "
        "type "
        "or a keyword/reserved word.",
        "Declared variable named: flt illegally shares its name with a "
        "type or "
        "a keyword/reserved word.",
        "Udt attribute type of:  void for attribute:  and for udt: bool "
        "does "
        "not exist.",
        "Udt attribute type of:  udt for attribute: u and for udt: bool "
        "does "
        "not exist.",
        "Too few arguments in udt initialization.",
        "Received unknown attribute name of: a in constructor of udt of "
        "type: "
        "Foo.",
        "Too few arguments in udt initialization.",
        "Received intiializer variable of type: flt in constructor of udt "
        "of "
        "type: Foo when a variable for type: int was expected for "
        "attribute "
        "named: i.",
        "Too few arguments in udt initialization.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_udt.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

// NOTE: should have no errors!
TEST(SemanticTest, TypeCheckerAssignments)
{
    static const std::string expected[] = {

    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_assignment.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerIfElse)
{
    static const std::string expected[] = {
        "Expected grouping to result in a boolean.",
        "Expected grouping to result in a boolean.",
        "Expected grouping to result in a boolean.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_if_else.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        // std::cout << err->getErrorMessage() << "\n";
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerReturnStatements)
{
    static const std::string expected[] = {
        "Actual return type of: flt does not match expected return type "
        "of: "
        "int.",
        "Unexpected return in function returning void.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_function_return.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerMethodAccess)
{
    static const std::string expected[] = {
        "Supplied argument type of: int does not match expected type of: "
        "flt.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "int and flt.",
        "Method: bar does not exist for udt type: Foo.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and unknown.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_methodaccess.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerAttributeAccess)
{
    static const std::string expected[] = {
        "Too few arguments in udt initialization.",
        "Attribute: f does not exists for udt type: Foo.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and unknown.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and int.",
        "Attribute: i called on undeclared variable: goo.",
        "Expected the same types on each side of operation. Instead "
        "received: "
        "flt and unknown.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("./examples/semantics_attributeaccess.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}