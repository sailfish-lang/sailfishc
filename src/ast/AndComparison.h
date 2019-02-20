/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class AndComparison : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    AndComparison(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::AndComparison;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}