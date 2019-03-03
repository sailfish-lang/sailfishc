/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Modulo : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Modulo(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Modulo()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Modulo;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}