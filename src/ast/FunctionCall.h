/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include <vector>

namespace ast
{
class FunctionCall : public Expression
{
  private:
    std::vector<Expression*> args;

  public:
    // constructor
    FunctionCall(){};
    // implement Expression
    /* virtual*/ ExpressionType
    getType()
    {
        return ExpressionType::FunctionCall;
    }
};
}