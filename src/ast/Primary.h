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
        IdentifierLiteral,
        StringLiteral,
        BooleanLiteral,
        IntegerLiteral,
        FloatLiteral,
        AttributeAccessLiteral,
        MethodAccessLiteral,
        FunctionCallLiteral,
    };
    virtual PrimaryType getPrimaryType() = 0;
};
}