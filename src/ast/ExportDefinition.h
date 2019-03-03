/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Exportable.h"
#include "Node.h"
#include "SourcePart.h"
#include <string>

namespace ast
{
class ExportDefinition : public SourcePart, public Node
{
  private:
    Exportable* expr;

  public:
    // constructor
    ExportDefinition(Exportable* e, int lineNum) : Node(lineNum)
    {
        expr = e;
    };
    // destructor
    ~ExportDefinition()
    {
        delete expr;
    };
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::ExportDefinition;
    }
    // get methods
    Exportable*
    getExport()
    {
        return expr;
    }
};
}