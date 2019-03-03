/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "ExpressionStatement.h"
#include "IntegerLiteral.h"
#include "Node.h"
#include <vector>

namespace ast
{
class GroupingExpression : public Expression, public Node
{
  private:
    std::vector<ExpressionStatement*> expressionList;

  public:
    // constructor
    GroupingExpression(std::vector<ExpressionStatement*> e, int lineNum)
        : Node(lineNum)
    {
        expressionList = e;
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
    std::vector<ExpressionStatement*>
    getExpressionStatementList()
    {
        return expressionList;
    }
};
}