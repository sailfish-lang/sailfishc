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
class Addition : public BinaryExpression, public Node
{
  private:
    Expression* leftExpr;
    ExpressionStatement* rightExpr;

  public:
    // constructor
    Addition(Expression* l, ExpressionStatement* r, int lineNum) : Node(lineNum)
    {
        leftExpr = l;
        rightExpr = r;
    };
    // destructor
    ~Addition()
    {
        delete leftExpr;
        delete rightExpr;
    };
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::Addition;
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