/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class BinaryExpression
{
  public:
    enum BinaryExpressionType
    {
        Exponentiation,
        Multiplication,
        Division,
        Modulo,
        Addition,
        Subtraction,
        BinaryGreaterThan,
        BinaryLessThan,
        BinaryGreaterThanOrEqual,
        BinaryLessThanOrEqual,
        EquivalenceComparison,
        NonEquivalenceComparison,
        AndComparison,
        OrComparison,
        Assignment,
        FunctionCallExpression,
        MemberAccess,
        ExpressionOnlyStatement,
    };
    virtual BinaryExpressionType getBinaryExpressionType() = 0;
};
}