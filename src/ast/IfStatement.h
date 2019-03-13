/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Block.h"
#include "Node.h"
#include "Statement.h"

namespace ast
{
class IfStatement : public Statement, public Node
{
  private:
    GroupingExpression* ifConditional;
    Block* ifStatements;
    Statement* elseStatements;

  public:
    // constructor
    IfStatement(GroupingExpression* e, Block* b1, Statement* b2, int lineNum)
        : Node(lineNum)
    {
        ifConditional = e;
        ifStatements = b1;
        elseStatements = b2;
    };
    // destructor
    ~IfStatement()
    {
        delete ifConditional;
        delete ifStatements;
        delete elseStatements;
    };
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::IfStatement;
    }
    // get methods
    GroupingExpression*
    getIfConditional()
    {
        return ifConditional;
    }
    Block*
    getIfStatements()
    {
        return ifStatements;
    }
    Statement*
    getElseStatements()
    {
        return elseStatements;
    }
};
}