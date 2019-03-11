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
class Modulo : public BinaryExpression, public Node
{
  public:
    // constructor
    Modulo(Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r){};
    // destructor
    ~Modulo(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
    }
};
}