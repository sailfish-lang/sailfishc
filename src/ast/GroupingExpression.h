/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"
#include "IntegerLiteral.h"
#include "Node.h"
#include <vector>

namespace ast
{
class GroupingExpression : public Expression, public Node
{
  private:
    BinaryExpression* expr;

  public:
    // constructor
    GroupingExpression(BinaryExpression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~GroupingExpression(){};
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::GroupingExpression;
    }
    // get method
    BinaryExpression*
    getBinaryExpressionList()
    {
        return expr;
    }
};
}