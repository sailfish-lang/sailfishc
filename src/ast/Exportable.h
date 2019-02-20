/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class Exportable
{
  public:
    enum ExportableType
    {
        ExportDefinition,
        FunctionDefinition,
    };

    virtual ExportableType getExportableType() = 0;
};
}