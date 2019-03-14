/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "GeneralDefinition.h"
#include "Node.h"
#include "SourcePart.h"
#include "Statement.h"

namespace ast
{
class GeneralDecleration : public SourcePart, public Statement, public Node
{
  private:
    GeneralDefinition* definition;

  public:
    // constructor
    GeneralDecleration(GeneralDefinition* g, int lineNum) : Node(lineNum)
    {
        definition = g;
    };
    // destructor
    ~GeneralDecleration()
    {
        delete definition;
    };
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::GeneralDecleration;
    }
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::GeneralDecleration;
    }
    // get methods
    GeneralDefinition*
    getDefinition()
    {
        return definition;
    }
};
}