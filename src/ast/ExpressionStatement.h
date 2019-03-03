/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"
#include "Statement.h"
#include <string>

namespace ast
{
class ExpressionStatement : public Statement, public Node
{
  private:
    Expression* expr;

  public:
    // constructor
    ExpressionStatement(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~ExpressionStatement()
    {
        delete expr;
    };
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::ExpressionStatement;
    }
    // get methods
    Expression*
    getExpression()
    {
        return expr;
    }
};
}