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
class NonEquivalenceComparison : public BinaryExpression, public Node
{
  private:
    Expression* leftExpr;
    ExpressionStatement* rightExpr;

  public:
    // constructor
    NonEquivalenceComparison(Expression* l, ExpressionStatement* r, int lineNum)
        : Node(lineNum)
    {
        leftExpr = l;
        rightExpr = r;
    };
    // destructor
    ~NonEquivalenceComparison()
    {
        delete leftExpr;
        delete rightExpr;
    };
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::NonEquivalenceComparison;
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