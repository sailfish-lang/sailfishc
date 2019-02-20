/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Negation : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Negation(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Negation;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}