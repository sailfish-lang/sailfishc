/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Identifier.h"

namespace ast
{
class VariableAssignment
{
  private:
    Identifier* name;
    Expression* expr;

  public:
    // constructor
    VariableAssignment(){};
};
}