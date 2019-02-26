/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Exportable.h"
#include "GeneralDefinition.h"
#include "SourcePart.h"
#include "Statement.h"

namespace ast
{
class GeneralDecleration : public SourcePart,
                           public Exportable,
                           public Statement
{
  private:
    GeneralDefinition* definition;

  public:
    // constructor
    GeneralDecleration(GeneralDefinition* g)
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
    // implementation for Exportable
    /* virtual */ ExportableType
    getExportableType()
    {
        return ExportableType::GeneralDecleration;
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