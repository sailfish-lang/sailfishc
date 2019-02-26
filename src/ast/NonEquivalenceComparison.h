/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class NonEquivalenceComparison : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    NonEquivalenceComparison(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~NonEquivalenceComparison()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::NonEquivalenceComparison;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}