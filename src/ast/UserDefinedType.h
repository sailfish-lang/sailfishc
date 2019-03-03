/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "DictionaryItem.h"
#include "Expression.h"
#include "Identifier.h"
#include "New.h"
#include "Node.h"
#include <vector>

namespace ast
{
class UserDefinedType : public New, public Node
{
  private:
    Identifier* name;
    std::vector<DictionaryItem*> attributes;

  public:
    // constructor
    UserDefinedType(Identifier* i, std::vector<DictionaryItem*> a, int lineNum)
        : Node(lineNum)
    {
        name = i;
        attributes = a;
    };
    // destructor
    ~UserDefinedType()
    {
        delete name;
    };
    // implement New
    /* virtual */ NewType
    getNewType()
    {
        return NewType::UserDefinedType;
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<DictionaryItem*>
    getAttributes()
    {
        return attributes;
    }
};
}