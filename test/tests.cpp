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

TEST(SeamanticTest, TypeChecker)
{
    static const std::string expected[] = {
        "Functions input type of:  nonExistentType for function: "
        "inputDoesNotExist does not exist.",
        "Declared function input named: flt illegally shares its name with a "
        "type or a keyword/reserved word.",
        "Functions output type of:  nonExistentType for function: "
        "outputDoesNotExist does not exist.",
        "Declared function named: flt illegally shares its name with a type or "
        "a keyword/reserved word.",
        "Declared list named: dec illegally shares its name with a type or a "
        "keyword/reserved word.",
        "Declared type: flt of primitive for variable named: function_foo does "
        "not match assigned expression type of: int.",
        "Defined dictionary's key type of: Foo for dictionary: "
        "dictionary_error_a does not exist.",
        "Declared type of dictionary keys: Foo for variable named: "
        "dictionary_error_a does not match assigned expression's dictionary "
        "keys of type: int.",
        "Defined dictionary's value type of: Foo for dictionary: "
        "dictionary_error_b does not exist.",
        "Declared type of dictionary values: Foo for variable named: "
        "dictionary_error_b does not match assigned expression's dictionary "
        "values of type: flt.",
        "Declared definition named: str illegally shares its name with a type "
        "or a keyword/reserved word.",
        "Declared type of dictionary for variable named: dictionary_error_c "
        "does not match assigned expression type of: list.",
        "Dictionary is not homogenous. Received key types: flt and int which "
        "do not match.",
        "Declared type of dictionary keys: int for variable named: "
        "dictionary_error_e does not match assigned expression's dictionary "
        "keys of type: flt.",
        "Declared type of dictionary values: flt for variable named: "
        "dictionary_error_f does not match assigned expression's dictionary "
        "values of type: int.",
        "Defined lists's type of: Foo for list: list_error_a does not exist.",
        "Declared type of list: Foo for variable named: list_error_a does not "
        "match assigned expression's list of type: int.",
        "Declared type of list for variable named: list_error_b does not match "
        "assigned expression type of: dictionary.",
        "List is not homogenous. Received types: flt and int which do not "
        "match.",
        "Declared type of list: int for variable named: list_error_d does not "
        "match assigned expression's list of type: flt.",
        "Declared udt named: bool illegally shares its name with a type or a "
        "keyword/reserved word.",
        "Declared udt attribute named: flt illegally shares its name with a "
        "type or a keyword/reserved word.",
        "Declared udt attribute named: void illegally shares its name with a "
        "type or a keyword/reserved word.",
        "Udt attribute type of:  void for attribute: void and for udt: bool "
        "does not exist.",
        "Udt attribute type of:  udt for attribute: u and for udt: bool does "
        "not exist.",
        "Left hand assignment for: assignment_a expected: Foo and received: "
        "int.",
        "Left hand assignment for: assignment_c expected: flt and received: "
        "Foo.",
        "Undefined assignment left hand expression: f4.",
        "Undefined assignment right hand expression: f4 for assignment to: "
        "assignment_b.",
        "Left hand assignment for: assignment_b expected: Foo and received: "
        "f4.",
        "Cannot assign variable to function.",
        "Expected either float or integer type on left side of addition. "
        "Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of addition. "
        "Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of addition. "
        "Instead received: bool.",
        "Expected the same types on each side of addition. Instead recevied: "
        "int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of addition. Instead recevied: "
        "flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of subtraction. "
        "Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of subtraction. "
        "Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of subtraction. "
        "Instead received: bool.",
        "Expected the same types on each side of subtraction. Instead "
        "recevied: int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of subtraction. Instead "
        "recevied: flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of multiplication. "
        "Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of multiplication. "
        "Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of multiplication. "
        "Instead received: bool.",
        "Expected the same types on each side of multiplication. Instead "
        "recevied: int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of multiplication. Instead "
        "recevied: flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of division. "
        "Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of division. "
        "Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of division. "
        "Instead received: bool.",
        "Expected the same types on each side of division. Instead recevied: "
        "int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of division. Instead recevied: "
        "flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Integer type on left side of exponentiation. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Integer type on left side of exponentiation. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Integer type on left side of exponentiation. Instead received: bool.",
        "Integer type on left side of modulo. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Integer type on left side of modulo. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Integer type on left side of modulo. Instead received: bool.",
        "Expected either float or integer type on left side of greater than "
        "comparison. Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of greater than "
        "comparison. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of greater than "
        "comparison. Instead received: bool.",
        "Expected the same types on each side of greater than comparison. "
        "Instead recevied: int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of greater than comparison. "
        "Instead recevied: flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of greater than or "
        "equal to comparison. Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of greater than or "
        "equal to comparison. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of greater than or "
        "equal to comparison. Instead received: bool.",
        "Expected the same types on each side of greater than or equal to "
        "comparison. Instead recevied: int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of greater than or equal to "
        "comparison. Instead recevied: flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of less than "
        "comparison. Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of less than "
        "comparison. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of less than "
        "comparison. Instead received: bool.",
        "Expected the same types on each side of less than comparison. Instead "
        "recevied: int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of less than comparison. Instead "
        "recevied: flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of less than or "
        "equal to comparison. Instead received: flt.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected either float or integer type on left side of less than or "
        "equal to comparison. Instead received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected either float or integer type on left side of less than or "
        "equal to comparison. Instead received: bool.",
        "Expected the same types on each side of less than or equal to "
        "comparison. Instead recevied: int and flt.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of less than or equal to "
        "comparison. Instead recevied: flt and int.",
        "Left hand assignment for: f expected: bool and received: flt.",
        "Expected the same types on each side of equivalence comparison. "
        "Instead recevied: bool and int.",
        "Expected the same types on each side of equivalence comparison. "
        "Instead recevied: int and bool.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected the same types on each side of nonequivalence comparison. "
        "Instead recevied: bool and int.",
        "Expected the same types on each side of nonequivalence comparison. "
        "Instead recevied: int and bool.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected boolean type on left side of and comparison. Instead "
        "received: bool.",
        "Expected boolean type on left side of and comparison. Instead "
        "received: bool.",
        "Expected boolean type on left side of and comparison. Instead "
        "received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected boolean type on left side of or comparison. Instead "
        "received: bool.",
        "Expected boolean type on left side of or comparison. Instead "
        "received: bool.",
        "Expected boolean type on left side of or comparison. Instead "
        "received: int.",
        "Left hand assignment for: f expected: bool and received: int.",
        "Expected boolean type following negation. Instead received: bool.",
        "Expected boolean type following negation. Instead received: int.",
    };

    Parser* p = new Parser();
    ast::Start* root = p->parse("../sailfish_examples/semantics.fish");

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