/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class Assignment : public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Assignment(Expression* e)
    {
        expr = e;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Assignment;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}