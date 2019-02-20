/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Statement.h"

namespace ast
{
class ReturnStatement : public Statement
{
  private:
    // optional
    Expression* expr;

  public:
    // constructor
    ReturnStatement(Expression* e)
    {
        expr = e;
    };
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::ReturnStatement;
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}