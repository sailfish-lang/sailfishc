/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Identifier.h"

namespace ast
{
class MemberAccess : public Expression
{
  private:
    Identifier* member;

  public:
    // constructor
    MemberAccess(Identifier* i)
    {
        member = i;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::MemberAccess;
    }
    // get method
    Identifier*
    getMember()
    {
        return member;
    }
};
}