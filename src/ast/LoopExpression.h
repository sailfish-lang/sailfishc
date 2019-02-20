/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Block.h"
#include "Expression.h"
#include "Loop.h"

namespace ast
{
class LoopExpression : public Loop
{
  private:
    Expression* loopCondition;
    Block* body;

  public:
    // constructor
    LoopExpression(Expression* e, Block* b)
    {
        loopCondition = e;
        body = b;
    };
    // implement Loop
    /* virtual */ LoopType
    getType()
    {
        return LoopType::LoopExpression;
    }
    // get methods
    Expression*
    getCondition()
    {
        return loopCondition;
    }
    Block*
    getBody()
    {
        return body;
    }
};
}