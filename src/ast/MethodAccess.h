/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "FunctionCall.h"
#include "Identifier.h"
#include "MemberAccess.h"

namespace ast
{
class MethodAccess : public MemberAccess
{
  private:
    Identifier* name;
    FunctionCall* func;

  public:
    // constructor
    MethodAccess(Identifier* i, FunctionCall* f)
    {
        name = i;
        func = f;
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
};
}