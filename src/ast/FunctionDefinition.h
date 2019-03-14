/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Block.h"
#include "Identifier.h"
#include "Input.h"
#include "Node.h"
#include "Output.h"
#include "SourcePart.h"
#include <vector>

namespace ast
{
class FunctionDefinition : public SourcePart, public Node
{
  private:
    Identifier* name;
    std::vector<Input*> inputList;
    Output* output;
    Block* body;

  public:
    // constructor
    FunctionDefinition(Identifier* i, std::vector<Input*> in, Output* out,
                       Block* b, int lineNum)
        : Node(lineNum)
    {
        name = i;
        inputList = in;
        output = out;
        body = b;
    };
    // destructor
    ~FunctionDefinition()
    {
        delete name;
    };
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::FunctionDefinitionPart;
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<Input*>
    getInputList()
    {
        return inputList;
    }
    Output*
    getOutput()
    {
        return output;
    }
    Block*
    getBody()
    {
        return body;
    }
};
}