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
        IndexAccess,
        MemberAccess,
        FunctionCall,
        PrimaryExpression,
        Negation,
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
    };
    virtual ExpressionType getExpressionType() = 0;
};
}