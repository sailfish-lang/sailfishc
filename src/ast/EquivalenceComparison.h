/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class EquivalenceComparison : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    EquivalenceComparison(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~EquivalenceComparison()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::EquivalenceComparison;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}