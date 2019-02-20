/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <vector>

namespace ast
{
class ListLiteral : public Primary
{
  private:
    std::vector<ListItem*> listItems;

  public:
    // constructor
    ListLiteral(std::vector<ListItem*> v)
    {
        listItems = v;
    };
    // implement Primary
    /* virtual */ PrimaryType
    getType()
    {
        return PrimaryType::ListLiteral;
    }
    // get methods
    std::vector<ListItem*>
    getItems()
    {
        return listItems;
    }
};
}