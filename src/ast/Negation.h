/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "BinaryExpression.h"
#include "Node.h"
#include "UnaryExpression.h"

namespace ast
{
class Negation : public UnaryExpression, public Node
{
  private:
    BinaryExpression* expr;

  public:
    // constructor
    Negation(BinaryExpression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~Negation()
    {
        delete expr;
    };
    // implement UnaryExpression
    /* virtual */ UnaryExpressionType
    getUnaryExpressionype()
    {
        return UnaryExpressionType::Negation;
    }
    // get methods
    BinaryExpression*
    getBinaryExpression()
    {
        return expr;
    }
};
}