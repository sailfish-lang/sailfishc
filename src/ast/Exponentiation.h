/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Exponentiation : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Exponentiation(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Exponentiation()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Exponentiation;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}