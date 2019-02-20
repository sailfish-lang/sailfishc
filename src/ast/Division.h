/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Division : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Division(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Division;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}