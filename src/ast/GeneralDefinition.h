/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class GeneralDefinition
{
  public:
    enum GeneralDefinitionType
    {
        ListDefinition,
        DictionaryDefinition,
        NewVariableDefinition,
    };
    virtual GeneralDefinitionType getGeneralDefinitionType() = 0;
};
}