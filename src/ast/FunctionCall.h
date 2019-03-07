/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Node.h"
#include <vector>

namespace ast
{
class FunctionCall : public BinaryExpression, public Node
{
  private:
    Expression* expr;
    std::vector<Identifier*> args;

  public:
    // constructor
    FunctionCall(std::vector<Identifier*> i, int lineNum) : Node(lineNum)
    {
        args = i;
    };
    // destructor
    ~FunctionCall(){};
    // implement BinaryExpression
    /* virtual*/ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::FunctionCallExpression;
    }
    // get methods
    std::vector<Identifier*>
    getArguments()
    {
        return args;
    }
    Expression*
    getExpr()
    {
        return expr;
    }
};
}