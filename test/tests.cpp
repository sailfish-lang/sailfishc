/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/sailfish/Lexar.h"
#include <gtest/gtest.h>

TEST(LexarTest, HelloWorld)
{
    static const Kind expected[] = {Kind::COMMENT_TOKEN, Kind::IDENTIFIER_TOKEN,
                                    Kind::LCURLEY_TOKEN, Kind::IDENTIFIER_TOKEN,
                                    Kind::LPAREN_TOKEN,  Kind::STRING_TOKEN,
                                    Kind::RPAREN_TOKEN,  Kind::RCURLEY_TOKEN};

    Lexar* lexar = new Lexar("../sailfish_examples/helloworld.fish");

    Token* t;
    int i = 0;

    t = lexar->getToken();

    while (!t->isEOF())
    {
        ASSERT_EQ(expected[i], t->getKind());
        t = lexar->getToken();
        ++i;
    }
}

TEST(LexarTest, FizzBuzz)
{
    static const Kind expected[] = {
        Kind::KEYWORD_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::ARROW_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::IDENTIFIER_TOKEN, Kind::ARROW_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::LCURLEY_TOKEN,    Kind::KEYWORD_TOKEN,
        Kind::LPAREN_TOKEN,     Kind::IDENTIFIER_TOKEN, Kind::OPERATION_TOKEN,
        Kind::INTEGER_TOKEN,    Kind::LOGIC_TOKEN,      Kind::INTEGER_TOKEN,
        Kind::RPAREN_TOKEN,     Kind::LCURLEY_TOKEN,    Kind::IDENTIFIER_TOKEN,
        Kind::LPAREN_TOKEN,     Kind::STRING_TOKEN,     Kind::RPAREN_TOKEN,
        Kind::RCURLEY_TOKEN,    Kind::KEYWORD_TOKEN,    Kind::LPAREN_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::OPERATION_TOKEN,  Kind::INTEGER_TOKEN,
        Kind::LOGIC_TOKEN,      Kind::INTEGER_TOKEN,    Kind::RPAREN_TOKEN,
        Kind::LCURLEY_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::LPAREN_TOKEN,
        Kind::STRING_TOKEN,     Kind::RPAREN_TOKEN,     Kind::RCURLEY_TOKEN,
        Kind::KEYWORD_TOKEN,    Kind::LPAREN_TOKEN,     Kind::IDENTIFIER_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::INTEGER_TOKEN,    Kind::LOGIC_TOKEN,
        Kind::INTEGER_TOKEN,    Kind::RPAREN_TOKEN,     Kind::LCURLEY_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::LPAREN_TOKEN,     Kind::STRING_TOKEN,
        Kind::RPAREN_TOKEN,     Kind::RCURLEY_TOKEN,    Kind::KEYWORD_TOKEN,
        Kind::LCURLEY_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::LPAREN_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::RPAREN_TOKEN,     Kind::RCURLEY_TOKEN,
        Kind::RCURLEY_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::LCURLEY_TOKEN,
        Kind::COMMENT_TOKEN,    Kind::KEYWORD_TOKEN,    Kind::IDENTIFIER_TOKEN,
        Kind::OPERATION_TOKEN,  Kind::LBRACKET_TOKEN,   Kind::INTEGER_TOKEN,
        Kind::COMMA_TOKEN,      Kind::INTEGER_TOKEN,    Kind::RBRACKET_TOKEN,
        Kind::LCURLEY_TOKEN,    Kind::IDENTIFIER_TOKEN, Kind::LPAREN_TOKEN,
        Kind::IDENTIFIER_TOKEN, Kind::RPAREN_TOKEN,     Kind::RCURLEY_TOKEN,
        Kind::RCURLEY_TOKEN,
    };

    Lexar* lexar = new Lexar("../sailfish_examples/fizzbuzz.fish");

    Token* t;
    int i = 0;

    t = lexar->getToken();

    while (!t->isEOF())
    {
        ASSERT_EQ(expected[i], t->getKind());
        t = lexar->getToken();
        ++i;
    }
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}