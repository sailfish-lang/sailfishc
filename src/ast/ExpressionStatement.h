/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Node.h"
#include "Statement.h"
#include <string>

namespace ast
{
class ExpressionStatement : public Statement, public Node
{
  private:
    BinaryExpression* binExpr;

  public:
    // constructor
    ExpressionStatement(BinaryExpression* be, int lineNum) : Node(lineNum)
    {
        binExpr = be;
    };
    // destructor
    ~ExpressionStatement()
    {
        delete binExpr;
    };
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::ExpressionStatement;
    }
    // get methods
    BinaryExpression*
    getBinaryExpression()
    {
        return binExpr;
    }
};
}