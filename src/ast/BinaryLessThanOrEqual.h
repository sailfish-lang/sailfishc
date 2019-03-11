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
class BinaryLessThanOrEqual : public BinaryExpression, public Node
{
  public:
    // constructor
    BinaryLessThanOrEqual(Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r){};
    // destructor
    ~BinaryLessThanOrEqual(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::BinaryLessThanOrEqual;
    }
};
}