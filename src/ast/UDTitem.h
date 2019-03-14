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
class UDTitem : public Node
{
  private:
    Identifier* key;
    Primary* value;

  public:
    // constructor
    UDTitem(Identifier* k, Primary* v, int lineNum) : Node(lineNum)
    {
        key = k;
        value = v;
    };
    // destructor
    ~UDTitem()
    {
        delete key;
        delete value;
    };
    // get methods
    Identifier*
    getKey()
    {
        return key;
    }
    Primary*
    getValue()
    {
        return value;
    }
};
}