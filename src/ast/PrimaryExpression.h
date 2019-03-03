/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"
#include "Primary.h"

namespace ast
{
class PrimaryExpression : public Expression, public Node
{
  private:
    Primary* primary;

  public:
    // constructor
    PrimaryExpression(Primary* p, int lineNum) : Node(lineNum)
    {
        primary = p;
    };
    // destructor
    ~PrimaryExpression()
    {
        delete primary;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::PrimaryExpression;
    }
    // get methods
    Primary*
    getPrimary()
    {
        return primary;
    }
};
}