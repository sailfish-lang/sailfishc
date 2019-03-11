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
class Assignment : public BinaryExpression, public Node
{
  public:
    // constructor
    Assignment(Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r){};
    // destructor
    ~Assignment(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::Assignment;
    }
};
}