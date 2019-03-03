/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Division : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Division(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Division()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Division;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}