/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"

namespace ast
{
class Negation : public Expression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    Negation(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Negation()
    {
        delete expr;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::Negation;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}