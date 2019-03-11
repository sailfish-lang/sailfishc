/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "Primary.h"
#include <vector>

namespace ast
{
class FunctionCall : public Primary, public Node
{
  private:
    Identifier* name;
    std::vector<Primary*> args;

  public:
    // constructor
    FunctionCall(Identifier* ident, std::vector<Primary*> i, int lineNum)
        : Node(lineNum)
    {
        name = ident;
        args = i;
    };
    // destructor
    ~FunctionCall(){};
    // implement Primary
    /* virtual*/ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::FunctionCallLiteral;
    }
    // get methods
    std::vector<Primary*>
    getArguments()
    {
        return args;
    }
    Identifier*
    getName()
    {
        return name;
    }
};
}