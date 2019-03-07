/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "FunctionCall.h"
#include "Identifier.h"
#include "MemberAccess.h"
#include "Node.h"

namespace ast
{
class MethodAccess : public MemberAccess, public Node
{
  private:
    Identifier* name;
    FunctionCall* func;
    Expression* expr;

  public:
    // constructor
    MethodAccess(Identifier* i, FunctionCall* f, Expression* e, int lineNum)
        : Node(lineNum)
    {
        name = i;
        func = f;
        expr = e;
    };
    // destructor
    ~MethodAccess()
    {
        delete name;
        delete func;
    };
    // implement MemberAccess
    /* virtual */ MemberAccessType
    getMemberAccessType()
    {
        return MemberAccess::MethodAccess;
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
    Expression*
    getExpression()
    {
        return expr;
    }
};
}