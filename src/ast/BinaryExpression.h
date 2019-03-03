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
class BinaryExpression : public Expression, public Node
{
  private:
    std::vector<Expression*> expressionList;

  public:
    // constructor
    BinaryExpression(std::vector<Expression*> e, int lineNum) : Node(lineNum)
    {
        expressionList = e;
    };
    // destructor
    ~BinaryExpression(){};
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryExpression;
    }
    // get method
    std::vector<Expression*>
    getExpressionList()
    {
        return expressionList;
    }
};
}