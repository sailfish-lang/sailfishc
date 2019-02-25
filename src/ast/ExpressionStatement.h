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
    Expression* Expr;

  public:
    // constructor
    ExpressionStatement(){};
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::ExpressionStatement;
    }
};
}