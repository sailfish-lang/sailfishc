/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "MemberAccess.h"

namespace ast
{
class AttributeAccess : public MemberAccess
{
  private:
    Identifier* attribute;

  public:
    // constructor
    AttributeAccess(Identifier* i)
    {
        attribute = i;
    };
    // implement MemberAccess
    /* virtual */ MemberAccessType
    getMemberAccessType()
    {
        return MemberAccess::AttributeAccess;
    }
    // get method
    Identifier*
    getAttribute()
    {
        return attribute;
    }
};
}