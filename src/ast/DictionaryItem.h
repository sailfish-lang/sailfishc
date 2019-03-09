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
    Primary* key;
    Primary* value;

  public:
    // constructor
    DictionaryItem(Primary* k, Primary* v, int lineNum) : Node(lineNum)
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
    Primary*
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