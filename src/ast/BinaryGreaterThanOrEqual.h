/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class BinaryGreaterThanOrEqual : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryGreaterThanOrEqual(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryGreaterThanOrEqual;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}