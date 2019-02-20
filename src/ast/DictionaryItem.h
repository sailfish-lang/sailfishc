/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"

namespace ast
{
class DictionaryItem
{
  private:
    Identifier* key;
    Identifier* value;

  public:
    // constructor
    DictionaryItem(Identifier* k, Identifier* v)
    {
        key = k;
        value = v;
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