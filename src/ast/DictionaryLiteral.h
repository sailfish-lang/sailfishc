/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "DictionaryItem.h"
#include "Primary.h"
#include <vector>

namespace ast
{
class DictionaryLiteral : public Primary
{
  private:
    std::vector<DictionaryItem*> dictionaryItems;

  public:
    // constructor
    DictionaryLiteral(std::vector<DictionaryItem*> v)
    {
        dictionaryItems = v;
    };
    // implement Primary
    /* virtual */ PrimaryType
    getType()
    {
        return PrimaryType::DictionaryLiteral;
    }
    // get methods
    std::vector<DictionaryItem*>
    getItems()
    {
        return dictionaryItems;
    }
};
}