/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Statement.h"

namespace ast
{
class Continue : public Statement
{
  public:
    // constructor
    Continue(){};
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::Continue;
    }
};
}