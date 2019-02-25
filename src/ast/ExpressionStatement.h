/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Statement.h"
#include <string>

namespace ast
{
class ExpressionStatement : public Statement
{
  private:
    Expression* expr;

  public:
    // constructor
    ExpressionStatement(Expression* e)
    {
        expr = e;
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