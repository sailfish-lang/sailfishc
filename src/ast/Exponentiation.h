/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Exponentiation : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Exponentiation(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Exponentiation;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}