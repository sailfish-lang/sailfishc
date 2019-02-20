/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Block.h"
#include "Loop.h"
#include "RangeVariableDefinition.h"

namespace ast
{
class LoopRange : public Loop
{
  private:
    RangeVariableDefinition* rngVar;
    Block* body;

  public:
    // constructor
    LoopRange(RangeVariableDefinition* r, Block* b)
    {
        rngVar = r;
        body = b;
    };
    // implement Loop
    /* virtual */ LoopType
    getType()
    {
        return LoopType::LoopRange;
    }
    // get methods
    RangeVariableDefinition*
    getRangeVariable()
    {
        return rngVar;
    }
    Block*
    getBody()
    {
        return body;
    }
};
}