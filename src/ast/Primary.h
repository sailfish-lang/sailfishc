/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class Primary
{
  public:
    enum PrimaryType
    {
        Identifier,
        StringLiteral,
        BooleanLiteral,
        IntegerLiteral,
        FloatLiteral,
        ListLiteral,
        DictionaryLiteral,
    };
    virtual PrimaryType getType() = 0;
};
}