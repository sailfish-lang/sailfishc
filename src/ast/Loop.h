/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class Loop
{
  public:
    enum LoopType
    {
        LoopRange,
        LoopExpression,
    };
    virtual LoopType getType() = 0;
};
}