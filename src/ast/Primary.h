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
        AttributeAccessLiteral,
        AttributeMethodAccessLiteral,
        BooleanLiteral,
        FloatLiteral,
        FunctionCallLiteral,
        IdentifierLiteral,
        IntegerLiteral,
        MethodAccessLiteral,
        StringLiteral,
    };
    virtual PrimaryType getPrimaryType() = 0;
};
}