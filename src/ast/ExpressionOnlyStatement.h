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
class ExpressionOnlyStatement : public BinaryExpression, public Node
{
  public:
    // constructor
    ExpressionOnlyStatement(Expression* l, int lineNum)
        : Node(lineNum), BinaryExpression(l, nullptr){};
    // destructor
    ~ExpressionOnlyStatement(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::ExpressionOnlyStatement;
    }
};
}