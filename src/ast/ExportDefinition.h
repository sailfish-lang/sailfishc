/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Exportable.h"
#include "SourcePart.h"
#include <string>

namespace ast
{
class ExportDefinition : public SourcePart
{
  private:
    Exportable* exprt;

  public:
    // constructor
    ExportDefinition(Exportable* e)
    {
        exprt = e;
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
        return exprt;
    }
};
}