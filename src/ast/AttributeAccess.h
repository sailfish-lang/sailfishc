/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "MemberAccess.h"
#include "Node.h"

namespace ast
{
class AttributeAccess : public MemberAccess, public Node
{
  private:
    Identifier* attribute;
    Expression* expr;

  public:
    // constructor
    AttributeAccess(Identifier* i, Expression* e, int lineNum) : Node(lineNum)
    {
        attribute = i;
        expr = e;
    };
    // destructor
    ~AttributeAccess(){};
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
    Expression*
    getExpression()
    {
        return expr;
    }
};
}