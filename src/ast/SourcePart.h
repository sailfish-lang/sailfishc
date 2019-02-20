/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class SourcePart
{
  public:
    enum SourcePartType
    {
        ExportDefinition,
        GeneralDecleration,
        FunctionDefinition,
        UserDefinedTypeDefinition,
        InitialExecutionBody,
    };
    // must defined a default value for the virtual function
    // https://stackoverflow.com/questions/307352/g-undefined-reference-to-typeinfo
    virtual SourcePartType getSourcePartType() = 0;
};
}