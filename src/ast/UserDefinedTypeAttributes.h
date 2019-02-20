/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Variable.h"
#include <vector>

namespace ast
{
class UserDefinedTypeAttributes
{
  private:
    Identifier* name;
    std::vector<Variable*> attributes;

  public:
    // constructor
    UserDefinedTypeAttributes(Identifier* i, std::vector<Variable*> v)
    {
        name = i;
        attributes = v;
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<Variable*>
    getAttributes()
    {
        return attributes;
    }
};
}