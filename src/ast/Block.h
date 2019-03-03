/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "Statement.h"
#include <vector>

namespace ast
{
class Block : public Statement, public Node
{
  private:
    std::vector<Statement*> statements;

  public:
    // constructor
    Block(std::vector<Statement*> v, int lineNum) : Node(lineNum)
    {
        statements = v;
    };
    // destructor
    ~Block(){};
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