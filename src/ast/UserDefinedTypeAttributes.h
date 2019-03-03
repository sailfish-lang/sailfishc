/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Node.h"
#include "Variable.h"
#include <vector>

namespace ast
{
class UserDefinedTypeAttributes : public Node
{
  private:
    Identifier* name;
    std::vector<Variable*> attributes;

  public:
    // constructor
    UserDefinedTypeAttributes(Identifier* i, std::vector<Variable*> v,
                              int lineNum)
        : Node(lineNum)
    {
        name = i;
        attributes = v;
    }
    // destructor
    ~UserDefinedTypeAttributes()
    {
        delete name;
    };
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<Variable*>
    getAttributes()
    {
        return attributes;
    }
};
}