/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Ast.h"
#include "AttributeAccess.h"
#include "FunctionCall.h"
#include "Node.h"
#include "Primary.h"

namespace ast
{
class AttributeMethodAccess : public Primary, public Node
{
  private:
    Identifier* name;
    AttributeAccess* attribute;
    FunctionCall* func;
    Identifier* udt;

  public:
    // constructor
    AttributeMethodAccess(Identifier* n, AttributeAccess* a, FunctionCall* f,
                          Identifier* u, int lineNum)
        : Node(lineNum)
    {
        name = n;
        attribute = a;
        func = f;
        udt = u;
    };
    // destructor
    ~AttributeMethodAccess()
    {
        delete name;
        delete func;
    };
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return Primary::AttributeMethodAccessLiteral;
    }
    // get method
    Identifier*
    getName()
    {
        return name;
    }
    AttributeAccess*
    getAttribute()
    {
        return attribute;
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