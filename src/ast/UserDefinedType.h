/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Identifier.h"
#include "New.h"
#include "Node.h"
#include "UDTitem.h"
#include <vector>

namespace ast
{
class UserDefinedType : public New, public Node
{
  private:
    Identifier* name;
    std::vector<UDTitem*> attributes;

  public:
    // constructor
    UserDefinedType(Identifier* i, std::vector<UDTitem*> a, int lineNum)
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
    std::vector<UDTitem*>
    getAttributes()
    {
        return attributes;
    }
};
}