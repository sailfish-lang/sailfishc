/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "IntegerLiteral.h"
#include <vector>

namespace ast
{
class ArrayExpression : public Expression
{
  private:
    std::vector<Expression*> expressionList;
    // flag for determining whether this is a valid index access, i.e.
    // if it can be used as an index
    bool canBeIndexAccess;

  public:
    // constructor
    ArrayExpression(std::vector<Expression*> e, bool c)
    {
        expressionList = e;
        canBeIndexAccess = c;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::ArrayExpression;
    }
    // get method
    std::vector<Expression*>
    getExpressionList()
    {
        return expressionList;
    }
    bool
    getCanBeIndexAccess()
    {
        return canBeIndexAccess;
    }
};
}