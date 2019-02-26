/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class BinaryGreaterThan : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryGreaterThan(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~BinaryGreaterThan()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryGreaterThan;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}