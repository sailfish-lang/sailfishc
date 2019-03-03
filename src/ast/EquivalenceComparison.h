/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class EquivalenceComparison : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    EquivalenceComparison(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~EquivalenceComparison()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::EquivalenceComparison;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}