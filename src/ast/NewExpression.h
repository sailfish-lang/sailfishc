/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "New.h"
#include "Node.h"

namespace ast
{
class NewExpression : public Expression, public Node
{
  private:
    New* newVal;

  public:
    // constructor
    NewExpression(New* n, int lineNum) : Node(lineNum)
    {
        newVal = n;
    };
    // destructor
    ~NewExpression()
    {
        delete newVal;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::NewExpression;
    }
    // get methods
    New*
    getNewVal()
    {
        return newVal;
    }
};
}