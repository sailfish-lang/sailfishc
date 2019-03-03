/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"

namespace ast
{
class UnaryExpression : public Expression
{
  public:
    enum UnaryExpressionType
    {
        Negation
    };
    virtual UnaryExpressionType getUnaryExpressionype() = 0;
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::UnaryExpression;
    }
};
}