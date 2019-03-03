/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Assignment : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Assignment(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Assignment()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Assignment;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}