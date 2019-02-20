/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class BinaryLessThan : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryLessThan(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryLessThan;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}