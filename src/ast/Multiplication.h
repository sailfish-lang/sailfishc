/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Multiplication : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Multiplication(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Multiplication;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}