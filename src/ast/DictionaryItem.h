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
class DictionaryItem : public Node
{
  private:
    Identifier* key;
    Primary* value;

  public:
    // constructor
    DictionaryItem(Identifier* k, Primary* v, int lineNum) : Node(lineNum)
    {
        key = k;
        value = v;
    };
    // destructor
    ~DictionaryItem()
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