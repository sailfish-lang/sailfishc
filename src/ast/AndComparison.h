/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"
#include "Node.h"

namespace ast
{
class AndComparison : public BinaryExpression, public Node
{
  private:
    BinaryExpression* rightExpr;

  public:
    // constructor
    AndComparison(Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r)
    {
        rightExpr = r;
    };
    // destructor
    ~AndComparison()
    {
        delete rightExpr;
    };
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::AndComparison;
    }
    // get methods
    BinaryExpression*
    getRightExpr()
    {
        return rightExpr;
    }
};
}