/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "SimpleStatement.h"
#include <string>

namespace ast
{
class ExpressionStatement : public SimpleStatement
{
  private:
    Expression* Expr;

  public:
    // constructor
    ExpressionStatement(){};
    // implement SimpleStatement
    /* virtual */ SimpleStatementType
    getSimpleStatementType()
    {
        return SimpleStatementType::ExpressionStatement;
    }
};
}