/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class MemberAccess : public Expression
{
  public:
    enum MemberAccessType
    {
        AttributeAccess,
        MethodAccess,
    };
    virtual MemberAccessType getMemberAccessType() = 0;
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::MemberAccess;
    }
};
}