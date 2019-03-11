/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h" // hack for name conflict
#include "Node.h"
#include "Primary.h"

namespace ast
{
class AttributeAccess : public Primary, public Node
{
  private:
    Identifier* attribute;
    Identifier* udt;

  public:
    // constructor
    AttributeAccess(Identifier* i, Identifier* u, int lineNum) : Node(lineNum)
    {
        attribute = i;
        udt = u;
    };
    // destructor
    ~AttributeAccess(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return Primary::AttributeAccessLiteral;
    }
    // get method
    Identifier*
    getAttribute()
    {
        return attribute;
    }
    Identifier*
    getUDT()
    {
        return udt;
    }
};
}