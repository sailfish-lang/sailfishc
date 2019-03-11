/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"

namespace ast
{
class BinaryExpression
{
  private:
    Expression* leftExpr;
    BinaryExpression* rightExpr;

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
        ExpressionOnlyStatement,
    };
    virtual BinaryExpressionType getBinaryExpressionType() = 0;

    // constructor
    BinaryExpression(Expression* l, BinaryExpression* r)
    {
        leftExpr = l;
        rightExpr = r;
    };

    // destructor
    ~BinaryExpression(){};

    // get methods
    virtual ast::Expression*
    getLeftExpr()
    {
        return leftExpr;
    }
    virtual BinaryExpression*
    getRightExpr()
    {
        return rightExpr;
    }
};
}