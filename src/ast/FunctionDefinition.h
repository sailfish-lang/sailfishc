/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Block.h"
#include "Exportable.h"
#include "Identifier.h"
#include "Input.h"
#include "Node.h"
#include "Output.h"
#include "SourcePart.h"
#include <vector>

namespace ast
{
class FunctionDefinition : public SourcePart, public Exportable, public Node
{
  private:
    Identifier* name;
    std::vector<Input*> inputList;
    std::vector<Output*> outputList;
    Block* body;

  public:
    // constructor
    FunctionDefinition(Identifier* i, std::vector<Input*> in,
                       std::vector<Output*> out, Block* b, int lineNum)
        : Node(lineNum)
    {
        name = i;
        inputList = in;
        outputList = out;
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
        return SourcePartType::FunctionDefinition;
    }
    // implementation for Exportable
    /* virtual */ ExportableType
    getExportableType()
    {
        return ExportableType::FunctionDefinition;
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
    std::vector<Output*>
    getOutputList()
    {
        return outputList;
    }
    Block*
    getBody()
    {
        return body;
    }
};
}