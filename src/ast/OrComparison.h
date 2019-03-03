/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class OrComparison : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    OrComparison(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~OrComparison()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::OrComparison;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}