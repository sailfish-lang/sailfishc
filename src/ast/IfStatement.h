/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Block.h"
#include "Expression.h"
#include "Statement.h"

namespace ast
{
class IfStatement : public Statement
{
  private:
    Expression* ifConditional;
    Block* ifStatements;
    Block* elseStatements;

  public:
    // constructor
    IfStatement(Expression* e, Block* b1, Block* b2)
    {
        ifConditional = e;
        ifStatements = b1;
        elseStatements = b2;
    };
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::IfStatement;
    }
    // get methods
    Expression*
    getIfConditional()
    {
        return ifConditional;
    }
    Block*
    getIfStatements()
    {
        return ifStatements;
    }
    Block*
    getElseStatements()
    {
        return elseStatements;
    }
};
}