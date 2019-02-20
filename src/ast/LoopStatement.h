/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Loop.h"
#include "Statement.h"

namespace ast
{
class LoopStatement : public Statement
{
  private:
    Loop* loop;

  public:
    // constructor
    LoopStatement(){};
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::LoopStatement;
    }
};
}