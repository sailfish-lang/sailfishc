/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ExpressionStatement.h"
#include "Node.h"
#include "Statement.h"

namespace ast
{
class ReturnStatement : public Statement, public Node
{
  private:
    BinaryExpression* expr;

  public:
    // constructor
    ReturnStatement(BinaryExpression* e, int lineNum) : Node(lineNum)
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
    BinaryExpression*
    getExpressionStatement()
    {
        return expr;
    }
};
}