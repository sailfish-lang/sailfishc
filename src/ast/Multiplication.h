/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Multiplication : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Multiplication(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Multiplication()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Multiplication;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}