/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"
#include "Node.h"

namespace ast
{
class ExpressionOnlyStatement : public BinaryExpression, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    ExpressionOnlyStatement(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~ExpressionOnlyStatement()
    {
        delete expr;
    };
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::ExpressionOnlyStatement;
    }
    // get methods
    Expression*
    getExpression()
    {
        return expr;
    }
};
}