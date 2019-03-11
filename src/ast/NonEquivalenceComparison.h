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
class NonEquivalenceComparison : public BinaryExpression, public Node
{
  public:
    // constructor
    NonEquivalenceComparison(Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r){};
    // destructor
    ~NonEquivalenceComparison(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::NonEquivalenceComparison;
    }
};
}