/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Statement.h"

namespace ast
{
class Break : public Statement
{
  public:
    // constructor
    Break(){};
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::Break;
    }
};
}