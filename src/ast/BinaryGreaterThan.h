/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class BinaryGreaterThan : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryGreaterThan(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~BinaryGreaterThan()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryGreaterThan;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}