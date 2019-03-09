/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Node.h"
#include "Primary.h"

namespace ast
{
class ListItem : public Node
{
  private:
    Primary* value;

  public:
    // constructor
    ListItem(Primary* v, int lineNum) : Node(lineNum)
    {
        value = v;
    };
    // destructor
    ~ListItem()
    {
        delete value;
    };
    // get methods
    Primary*
    getValue()
    {
        return value;
    }
};
}