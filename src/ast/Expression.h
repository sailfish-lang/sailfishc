/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class Expression
{
  public:
    enum ExpressionType
    {
        GroupingExpression,
        NewExpression,
        PrimaryExpression,
        UnaryExpression,
    };
    virtual ExpressionType getExpressionType() = 0;
};
}