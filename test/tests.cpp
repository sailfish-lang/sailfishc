/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "../src/sailfish/Lexar.h"
#include <gtest/gtest.h>

TEST(LexarTest, HelloWorld.fish)
{
    static const Kind expected[] = {Kind::COMMENT_TOKEN, Kind::KEYWORD_TOKEN,
                                    Kind::LCURLEY_TOKEN, Kind::KEYWORD_TOKEN,
                                    Kind::LPAREN_TOKEN,  Kind::STRING_TOKEN,
                                    Kind::RPAREN_TOKEN,  Kind::RCURLEY_TOKEN};

    Lexar* lexar = new Lexar("../sailfish_examples/helloworld.fish");

    Token* t;
    int i = 0;

    t = lexar->getToken();

    while (!t->isEOF())
    {
        EXPECT_EQ(expected[i], t->getKind());
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