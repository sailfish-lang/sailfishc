/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Modulo : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Modulo(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Modulo;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}