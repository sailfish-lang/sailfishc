/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Ast.h"
#include "FunctionCall.h"
#include "Node.h"
#include "Primary.h"

namespace ast
{
class MethodAccess : public Primary, public Node
{
  private:
    Identifier* name;
    FunctionCall* func;
    Identifier* udt;

  public:
    // constructor
    MethodAccess(Identifier* i, FunctionCall* f, Identifier* u, int lineNum)
        : Node(lineNum)
    {
        name = i;
        func = f;
        udt = u;
    };
    // destructor
    ~MethodAccess()
    {
        delete name;
        delete func;
    };
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return Primary::MethodAccessLiteral;
    }
    // get method
    Identifier*
    getName()
    {
        return name;
    }
    FunctionCall*
    getFunctionCall()
    {
        return func;
    }
    Identifier*
    getUDT()
    {
        return udt;
    }
};
}