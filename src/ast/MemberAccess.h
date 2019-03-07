/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"

namespace ast
{
class MemberAccess : public BinaryExpression
{
  public:
    enum MemberAccessType
    {
        AttributeAccess,
        MethodAccess,
    };
    virtual MemberAccessType getMemberAccessType() = 0;
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpression::MemberAccess;
    }
};
}