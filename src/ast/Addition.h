/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Addition : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Addition(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~Addition()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Addition;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}