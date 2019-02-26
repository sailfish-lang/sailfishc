/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class BinaryLessThanOrEqual : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryLessThanOrEqual(Expression* e)
    {
        expr = e;
    };
    // destructor
    ~BinaryLessThanOrEqual()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryLessThanOrEqual;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}