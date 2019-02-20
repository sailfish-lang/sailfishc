/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Exportable.h"
#include "SimpleStatement.h"
#include "SourcePart.h"
#include <string>

namespace ast
{
class ExportDefinition : public SourcePart, public SimpleStatement
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
    // implementation for SimpleStatement
    /* virutal */ std::string
    getType()
    {
        return "ExportDefinition";
    }
    // get methods
    Exportable*
    getExport()
    {
        return exprt;
    }
};
}