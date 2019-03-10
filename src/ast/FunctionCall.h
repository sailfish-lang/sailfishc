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
    std::vector<Primary*> args;

  public:
    // constructor
    FunctionCall(Expression* e, std::vector<Primary*> i, int lineNum)
        : Node(lineNum)
    {
        expr = e;
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
    std::vector<Primary*>
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