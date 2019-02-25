/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Statement.h"
#include <vector>

namespace ast
{
class Block : public Statement
{
  private:
    std::vector<Statement*> statements;

  public:
    // constructor
    Block(std::vector<Statement*> v)
    {
        statements = v;
    };
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::BlockStatement;
    }
    // get methods
    std::vector<Statement*>
    getStatements()
    {
        return statements;
    }
};
}