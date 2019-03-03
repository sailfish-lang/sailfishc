/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Addition : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Addition(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Addition()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Addition;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}