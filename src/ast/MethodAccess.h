/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Ast.h"
#include "BinaryExpression.h"
#include "Node.h"
#include "Primary.h"

namespace ast
{
class MethodAccess : public Primary, public Node
{
  private:
    Identifier* name;
    BinaryExpression* func;
    Identifier* udt;

  public:
    // constructor
    MethodAccess(Identifier* i, BinaryExpression* f, Identifier* u,
                 int lineNum)
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
    BinaryExpression*
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