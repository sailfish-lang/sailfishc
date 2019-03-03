/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Subtraction : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Subtraction(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Subtraction()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Subtraction;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}