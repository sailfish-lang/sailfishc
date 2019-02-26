/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Block.h"
#include "SourcePart.h"

namespace ast
{
class InitialExecutionBody : public SourcePart
{
    Block* body;

  public:
    // constructor
    InitialExecutionBody(Block* b)
    {
        body = b;
    };
    // destructor
    ~InitialExecutionBody()
    {
        delete body;
    };
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::InitialExecutionBody;
    }
    // get methods
    Block*
    getBody()
    {
        return body;
    }
};
}