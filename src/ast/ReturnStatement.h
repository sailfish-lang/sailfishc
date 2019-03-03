/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"
#include "Statement.h"

namespace ast
{
class ReturnStatement : public Statement, public Node
{
  private:
    // optional
    Expression* expr;

  public:
    // constructor
    ReturnStatement(Expression* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~ReturnStatement()
    {
        delete expr;
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