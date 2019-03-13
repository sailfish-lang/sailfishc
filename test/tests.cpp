/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/ast/Ast.h"
#include "../src/common/ReservedWords.h"
#include "../src/errorhandler/Error.h"
#include "../src/lexar/Lexar.h"
#include "../src/parser/Parser.h"
#include "../src/semant/SemanticAnalyzer.h"
#include "../src/semant/SymbolTable.h"
#include "../src/semant/TypeChecker.h"
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
        Kind::KEYWORD_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::OPERATION_TOKEN,
        Kind::INTEGER_TOKEN,    Kind::LOGIC_TOKEN,      Kind::LOGIC_TOKEN,
        Kind::ARROW_TOKEN,      Kind::ARROW_TOKEN,      Kind::UNDERSCORE_TOKEN,
        Kind::COMMA_TOKEN,      Kind::LBRACKET_TOKEN,   Kind::RBRACKET_TOKEN,
        Kind::LPAREN_TOKEN,     Kind::RPAREN_TOKEN,     Kind::ERROR_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::COMMENT_TOKEN,    Kind::PIPE_TOKEN,
        Kind::PIPE_TOKEN,       Kind::OPERATION_TOKEN,  Kind::OPERATION_TOKEN,
        Kind::BOOL_TOKEN,       Kind::BOOL_TOKEN,       Kind::LFISH_TAIL_TOKEN,
        Kind::RFISH_TAIL_TOKEN, Kind::RCURLEY_TOKEN,
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

TEST(SemanticTest, TypeCheckerFunctions)
{
    static const std::string expected[] = {
        "Declared type: nonExistentType for variable named: foo is not a legal "
        "or known type.",
        "Declared function input named: flt illegally shares its name with a "
        "type or a keyword/reserved word.",
        "Declared type: nonExistentType for variable named: nonExistentType is "
        "not a legal or known type.",
        "Actual return type of: int does not match expected return type of: "
        "nonExistentType.",
        "Declared variable named: flt illegally shares its name with a type or "
        "a keyword/reserved word.",
        "Function: foo is not defined.",
        "Expected the same types on each side of operation. Instead received: "
        "int and unknown.",
        "Supplied argument type of: int does not match expected type of: str.",
        "Not enough args supplied to function: good.",
        "Too many args supplied to function: good.",
    };

    Parser* p = new Parser();
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_functions.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerListsDeclerations)
{
    static const std::string expected[] = {
        "Declared type: Foo for variable named: list_error_a is not a legal or "
        "known type.",
        "Declared type of list: Foo for variable named: list_error_a does not "
        "match assigned expression's list of type: int.",
        "Declared type of list for variable named: list_error_b does not match "
        "assigned expression type of: dictionary.",
        "List is not homogenous. Received types: flt and int which do not "
        "match.",
        "Declared type of list: int for variable named: list_error_d does not "
        "match assigned expression's list of type: flt.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("../sailfish_examples/semantics_lists.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerDictionaryDeclerations)
{
    static const std::string expected[] = {

        "Declared type: Foo for variable named: Foo is not a legal or known "
        "type.",
        "Declared type of dictionary keys: Foo for variable named: "
        "dictionary_error_a does not match assigned expression's dictionary "
        "keys of type: int.",
        "Declared type: Foo for variable named: Foo is not a legal or known "
        "type.",
        "Declared type of dictionary values: Foo for variable named: "
        "dictionary_error_b does not match assigned expression's dictionary "
        "values of type: flt.",
        "Declared variable named: str illegally shares its name with a type "
        "or a keyword/reserved word.",
        "Declared type of dictionary for variable named: dictionary_error_c "
        "does not match assigned expression type of: list.",
        "Inconsistent key types in dictionary",
        "Declared type of dictionary keys: int for variable named: "
        "dictionary_error_e does not match assigned expression's dictionary "
        "keys of type: flt.",
        "Declared type of dictionary values: flt for variable named: "
        "dictionary_error_f does not match assigned expression's dictionary "
        "values of type: int.",
    };

    Parser* p = new Parser();
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_dictionaries.fish");

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
        "Declared variable named: dec illegally shares its name with a type or "
        "a "
        "keyword/reserved word.",
        "Declared type: flt of primitive for variable named: function_foo does "
        "not match assigned expression type of: int.",
    };

    Parser* p = new Parser();
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_primitives.fish");

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
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected either float or integer type on left side of operation. "
        "Instead received: bool.",
        "Expected the same types on each side of operation. Instead received:"
        " int and flt.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",

        "Expected int type on left side of operation. Instead "
        "received: flt.",
        "Expected int type on left side of operation. Instead received: bool.",

        "Expected int type on left side of operation. Instead "
        "received: flt.",
        "Expected int type on left side of operation. Instead received: bool.",

        "Expected the same types on each side of operation. Instead received: "
        "bool and int.",
        "Expected the same types on each side of operation. Instead received: "
        "int and bool.",

        "Expected the same types on each side of operation. Instead received: "
        "bool and int.",
        "Expected the same types on each side of operation. Instead received: "
        "int and bool.",

        "Expected boolean type on right side of operation. Instead received: "
        "int.",
        "Expected boolean type on left side of operation. Instead received: "
        "int.",

        "Expected boolean type on right side of operation. Instead received: "
        "int.",
        "Expected boolean type on left side of operation. Instead received: "
        "int.",

        "Expected boolean type following negation. Instead received: int."};

    Parser* p = new Parser();
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_binaries_unaries.fish");

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
        "Declared variable named: bool illegally shares its name with a type "
        "or a "
        "keyword/reserved word.",
        "Declared variable named: flt illegally shares its name with "
        "a "
        "type or a keyword/reserved word.",
        "Declared variable named: void illegally shares its name "
        "with a "
        "type or a keyword/reserved word.",
        "Udt attribute type of:  void for attribute: void and for udt: bool "
        "does not exist.",
        "Udt attribute type of:  udt for attribute: u and for udt: bool does "
        "not exist.",
        "Variable: a is undefined.",
        "Attribute: int for declared udt of type: unknown does not match "
        "expected type: .",
        "Attribute: flt for declared udt of type: int does not match expected "
        "type: .",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("../sailfish_examples/semantics_udt.fish");

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
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_assignment.fish");

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
    ast::Start* root = p->parse("../sailfish_examples/semantics_if_else.fish");

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
        "Actual return type of: flt does not match expected return type of: "
        "int.",
        "Unexpected return in function returning void.",
    };

    Parser* p = new Parser();
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_function_return.fish");

    SemanticAnalyzer* s = new SemanticAnalyzer(root);

    int i = 0;
    for (Error* const& err : s->testAnalyze())
    {
        ASSERT_EQ(err->getErrorMessage(), expected[i]);
        ++i;
    }
}

TEST(SemanticTest, TypeCheckerDictionaryAndListAccessors)
{
    static const std::string expected[] = {
        "Expected the same types on each side of operation. Instead received: "
        "flt and int.",
        "Supplied key type: flt does not match expected key type: int for: "
        "someDict.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and unknown.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse(
        "../sailfish_examples/semantics_dictionary_list_accessors.fish");

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
        "Supplied argument type of: int does not match expected type of: flt.",
        "Expected the same types on each side of operation. Instead received: "
        "int and flt.",
        "Method: bar does not exist for udt type: Foo.",
        "Expected the same types on each side of operation. Instead received: "
        "flt and unknown.",
    };

    Parser* p = new Parser();
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_methodaccess.fish");

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
    ast::Start* root =
        p->parse("../sailfish_examples/semantics_attributeaccess.fish");

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