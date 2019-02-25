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
    std::vector<Identifier*> args;

  public:
    // constructor
    FunctionCall(std::vector<Identifier*> i)
    {
        args = i;
    };
    // implement Expression
    /* virtual*/ ExpressionType
    getExpressionType()
    {
        return ExpressionType::FunctionCallExpression;
    }
    // get methods
    std::vector<Identifier*>
    getArguments()
    {
        return args;
    }
};
}