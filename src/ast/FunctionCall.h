/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Node.h"
#include <vector>

namespace ast
{
class FunctionCall : public Expression, public Node
{
  private:
    std::vector<Identifier*> args;

  public:
    // constructor
    FunctionCall(std::vector<Identifier*> i, int lineNum) : Node(lineNum)
    {
        args = i;
    };
    // destructor
    ~FunctionCall(){};
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