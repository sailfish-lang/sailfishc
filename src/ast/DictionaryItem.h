/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Node.h"

namespace ast
{
class DictionaryItem : public Node
{
  private:
    Identifier* key;
    Identifier* value;

  public:
    // constructor
    DictionaryItem(Identifier* k, Identifier* v, int lineNum) : Node(lineNum)
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
    Identifier*
    getValue()
    {
        return value;
    }
};
}