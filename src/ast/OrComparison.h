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
class OrComparison : public BinaryExpression, public Node
{
  public:
    // constructor
    OrComparison(Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r){};
    // destructor
    ~OrComparison(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::OrComparison;
    }
};
}