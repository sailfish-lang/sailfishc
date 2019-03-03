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
        ArrayExpression,
        GroupingExpression,
        MemberAccess,
        FunctionCallExpression,
        NewExpression,
        UnaryExpression,
        PrimaryExpression,
    };
    virtual ExpressionType getExpressionType() = 0;
};
}