/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Node.h"

namespace ast
{
class ListItem : public Node
{
  private:
    Identifier* name;

  public:
    // constructor
    ListItem(Identifier* i, int lineNum) : Node(lineNum)
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