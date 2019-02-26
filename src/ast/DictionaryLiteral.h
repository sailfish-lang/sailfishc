/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "DictionaryItem.h"
#include "New.h"
#include <vector>

namespace ast
{
class DictionaryLiteral : public New
{
  private:
    std::vector<DictionaryItem*> dictionaryItems;

  public:
    // constructor
    DictionaryLiteral(std::vector<DictionaryItem*> v)
    {
        dictionaryItems = v;
    };
    // destructor
    ~DictionaryLiteral(){};
    // implement New
    /* virtual */ NewType
    getNewType()
    {
        return NewType::DictionaryLiteral;
    }
    // get methods
    std::vector<DictionaryItem*>
    getItems()
    {
        return dictionaryItems;
    }
};
}