/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Primary.h"

namespace ast
{
class PrimaryExpression : public Expression
{
  private:
    Primary* primary;

  public:
    // constructor
    PrimaryExpression(Primary* p)
    {
        primary = p;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::PrimaryExpression;
    }
    // get methods
    Primary*
    getPrimary()
    {
        return primary;
    }
};
}