/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"

namespace ast
{
class ListItem
{
  private:
    Identifier* name;

  public:
    // constructor
    ListItem(Identifier* i)
    {
        name = i;
    };
    // destructor
    ~ListItem()
    {
        delete name;
    };
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
};
}