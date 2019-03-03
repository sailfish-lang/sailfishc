/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class BinaryLessThan : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryLessThan(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~BinaryLessThan()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::BinaryLessThan;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}