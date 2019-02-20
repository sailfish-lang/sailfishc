/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Exportable.h"
#include "GeneralDefinition.h"
#include "SourcePart.h"

namespace ast
{
class GeneralDecleration : public SourcePart, public Exportable
{
  private:
    GeneralDefinition* definition;

  public:
    // constructor
    GeneralDecleration(GeneralDefinition* g)
    {
        definition = g;
    };
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::GeneralDecleration;
    }
    // implementation for Exportable
    /* virtual */ ExportableType
    getExportableType()
    {
        return ExportableType::GeneralDecleration;
    }
    // get methods
    GeneralDefinition*
    getDefinition()
    {
        return definition;
    }
};
}