/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/lexar/Lexar2.h"
#include "../src/parser/Parser2.h"
#include "../src/semantics/SymbolMetaData.h"
#include "../src/semantics/SymbolTable.h"
#include "../src/semantics/UDTTable.h"
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

TEST(ParserTest, Parser2UDT)
{
    static const std::string expected[] = {"Source",
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

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}