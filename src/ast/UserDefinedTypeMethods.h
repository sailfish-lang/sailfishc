/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "FunctionDefinition.h"
#include "Identifier.h"
#include "Node.h"
#include <vector>

namespace ast
{
class UserDefinedTypeMethods : public Node
{
  private:
    Identifier* name;
    std::vector<FunctionDefinition*> methods;

  public:
    // constructor
    UserDefinedTypeMethods(Identifier* i, std::vector<FunctionDefinition*> v,
                           int lineNum)
        : Node(lineNum)
    {
        name = i;
        methods = v;
    };
    // destructor
    ~UserDefinedTypeMethods()
    {
        delete name;
    };
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<FunctionDefinition*>
    getMethods()
    {
        return methods;
    }
};
}