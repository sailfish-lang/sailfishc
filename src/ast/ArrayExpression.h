/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "IntegerLiteral.h"
#include "Node.h"
#include <vector>

namespace ast
{
class ArrayExpression : public Expression, public Node
{
  private:
    std::vector<Expression*> expressionList;
    // flag for determining whether this is a valid index access, i.e.
    // if it can be used as an index
    bool canBeIndexAccess;

  public:
    // constructor
    ArrayExpression(std::vector<Expression*> e, bool c, int lineNum)
        : Node(lineNum)
    {
        expressionList = e;
        canBeIndexAccess = c;
    };
    // destructor
    ~ArrayExpression(){};
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