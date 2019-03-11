/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "New.h"
#include "Node.h"
#include "Primary.h"
#include <vector>

namespace ast
{
class ListLiteral : public New, public Node, public Primary
{
  private:
    std::vector<ListItem*> listItems;

  public:
    // constructor
    ListLiteral(std::vector<ListItem*> v, int lineNum) : Node(lineNum)
    {
        listItems = v;
    };
    // destructor
    ~ListLiteral(){};
    // implement New
    /* virtual */ NewType
    getNewType()
    {
        return NewType::ListLiteral;
    }
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
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