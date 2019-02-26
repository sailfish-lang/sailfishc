/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class OrComparison : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    OrComparison(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~OrComparison()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::OrComparison;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}