/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Statement.h"
#include <vector>

namespace ast
{
class Block
{
  private:
    std::vector<Statement*> statements;

  public:
    // constructor
    Block(std::vector<Statement*> v)
    {
        statements = v;
    };
    // get methods
    std::vector<Statement*>
    getStatements()
    {
        return statements;
    }
};
}