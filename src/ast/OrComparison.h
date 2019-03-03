/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "Node.h"

namespace ast
{
class OrComparison : public BinaryExpression, public Node
{
  private:
    Expression* leftExpr;
    ExpressionStatement* rightExpr;

  public:
    // constructor
    OrComparison(Expression* l, ExpressionStatement* r, int lineNum)
        : Node(lineNum)
    {
        leftExpr = l;
        rightExpr = r;
    };
    // destructor
    ~OrComparison()
    {
        delete leftExpr;
        delete rightExpr;
    };
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::OrComparison;
    }
    // get methods
    Expression*
    getLeftExpr()
    {
        return leftExpr;
    }
    ExpressionStatement*
    getRightExpr()
    {
        return rightExpr;
    }
};
}