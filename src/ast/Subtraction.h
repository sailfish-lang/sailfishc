/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Subtraction : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Subtraction(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~Subtraction()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Subtraction;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}