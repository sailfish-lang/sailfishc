/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Node.h"
#include "Typename.h"

namespace ast
{
class Variable
{
  private:
    Typename* type;
    Identifier* name;

  public:
    // constructor
    Variable(Typename* t, Identifier* n)
    {
        type = t;
        name = n;
    }
    // get methods
    Typename*
    getType()
    {
        return type;
    }
    Identifier*
    getName()
    {
        return name;
    }
};
}